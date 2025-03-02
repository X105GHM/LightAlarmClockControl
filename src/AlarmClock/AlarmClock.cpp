#include "Button/Button.h"
#include "AlarmClock.h"
#include <Arduino.h>
#include <Preferences.h>
#include <time.h>

AlarmClock::AlarmClock(RotaryEncoder &encoder, Button &button,
                       SwitchInput &activeSwitch, Led &signalLed, HTTPManager &httpManager, NTPManager &ntpManager, Preferences &preferences)
    : _encoder(encoder), _button(button),
      _activeSwitch(activeSwitch), _signalLed(signalLed), _httpManager(httpManager), _ntpManager(ntpManager), _preferences(preferences),
      _alarmActive(false), _snoozed(false)
{
 
}

AlarmClock::~AlarmClock() {
    _preferences.end();
}

void AlarmClock::loadAlarmFromPreferences()
{
    if (_preferences.isKey("alarmHour"))
    {
        _alarmHours = _preferences.getInt("alarmHour");
        _alarmMinutes = _preferences.getInt("alarmMin");
        _alarmSeconds = _preferences.getInt("alarmSec");
        _alarmActive = _preferences.getBool("alarmActive");
    }
    else
    {
        _alarmHours = 6;
        _alarmMinutes = 0;
        _alarmSeconds = 0;
        _alarmActive = false;
        saveAlarmToPreferences();
    }
    Serial.printf("Geladene Alarmzeit: %02d:%02d:%02d, aktiv: %s\n",
                  _alarmHours, _alarmMinutes, _alarmSeconds, _alarmActive ? "ja" : "nein");
}

void AlarmClock::saveAlarmToPreferences()
{
    _preferences.putInt("alarmHour", _alarmHours);
    _preferences.putInt("alarmMin", _alarmMinutes);
    _preferences.putInt("alarmSec", _alarmSeconds);
    _preferences.putBool("alarmActive", _alarmActive);
    _preferences.end(); 
    _preferences.begin("alarmClock", false);
    sendDisplayUpdate();
    
    Serial.printf("Gespeicherte Alarmzeit: %02d:%02d:%02d, aktiv: %s\n",
                  _alarmHours, _alarmMinutes, _alarmSeconds, _alarmActive ? "ja" : "nein");
}

void AlarmClock::sendDisplayUpdate()
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer),
             "{\"hours\": %d, \"minutes\": %d, \"seconds\": %d, \"active\": %s}",
             _alarmHours, _alarmMinutes, _alarmSeconds, _alarmActive ? "true" : "false");
    _httpManager.sendDisplayData(buffer);
}

void AlarmClock::updateAlarmTime()
{
    _encoder.update();
    int increment = _encoder.getIncrement();
    bool timeChanged = false;

    if (increment != 0)
    {
        switch (_encoder.getMode())
        {
        case RotaryEncoder::Mode::Hours:
            _alarmHours = (_alarmHours + increment + 24) % 24;
            timeChanged = true;
            break;
        case RotaryEncoder::Mode::Minutes:
            _alarmMinutes = (_alarmMinutes + increment + 60) % 60;
            timeChanged = true;
            break;
        case RotaryEncoder::Mode::Seconds:
            _alarmSeconds = (_alarmSeconds + increment + 60) % 60;
            timeChanged = true;
            break;
        }
        _encoder.resetIncrement();
    }

    if (_encoder.isButtonPressed())
    {
        _encoder.switchMode();
        timeChanged = true;
    }

    if (_button.isPressed(1))
    {
        saveAlarmToPreferences();
    }

    if (timeChanged)
    {
        sendDisplayUpdate();
        Serial.printf("Display: %02d %02d %02d\n", _alarmHours, _alarmMinutes, _alarmSeconds);
    }
}

void AlarmClock::updateAlarmActiveState()
{
    bool switchState = _activeSwitch.isActive();
    if (switchState != _alarmActive)
    {
        _alarmActive = switchState;
        _preferences.putBool("alarmActive", _alarmActive);
        _preferences.end(); 
        _preferences.begin("alarmClock", false);

        Serial.printf("Alarm aktiviert: %s\n", _alarmActive ? "ja" : "nein");
        
        if (_alarmActive)
        {
            _signalLed.on();
            sendDisplayUpdate();
        }
        else
        {
            _signalLed.off();
            _httpManager.sendDirectOff();
        }
    }
}

void AlarmClock::checkAndActivateLight()
{
    time_t nowTime = _ntpManager.getCurrentTime();
    struct tm *timeinfo = localtime(&nowTime);

    int currentSeconds = timeinfo->tm_hour * 3600 + timeinfo->tm_min * 60 + timeinfo->tm_sec;
    
    int storedAlarmHours = _preferences.getInt("alarmHour", 6);
    int storedAlarmMinutes = _preferences.getInt("alarmMin", 0);
    int storedAlarmSeconds = _preferences.getInt("alarmSec", 0);
    bool storedAlarmActive = _preferences.getBool("alarmActive", false);
    
    int alarmSeconds = storedAlarmHours * 3600 + storedAlarmMinutes * 60 + storedAlarmSeconds;
    int diff = alarmSeconds - currentSeconds;

    bool shouldLightBeOn = (storedAlarmActive && diff > 0 && diff <= 1800);
    static bool lastLightState = false;

    if (shouldLightBeOn != lastLightState)
    {
        if (shouldLightBeOn)
        {
            _httpManager.sendLightCommand(true);
            sendDisplayUpdate();
        }
        else
        {
            //_httpManager.sendDirectOff();
        }
        lastLightState = shouldLightBeOn;
    }
}

void AlarmClock::processInputs()
{
    updateAlarmTime();
    updateAlarmActiveState();
    if (_button.isPressed(2))
    {
        if (!_snoozed)
        {
            _httpManager.sendDirectOn();
            _snoozed = true;
            Serial.println("Button 2 gedrückt: sendDirectOn()");
        }
        else
        {
        _httpManager.sendDirectOff();
        _snoozed = false;
        Serial.println("Button 2 gedrückt: sendDirectOff()");
        }
    }
}

void AlarmClock::processHTTP()
{
    checkAndActivateLight();
}
