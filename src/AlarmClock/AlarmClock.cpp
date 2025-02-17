#include "AlarmClock.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <time.h>

constexpr int EEPROM_SIZE = 16;
constexpr int EEPROM_ADDR_ALARM_HOUR = 0;
constexpr int EEPROM_ADDR_ALARM_MIN = 1;
constexpr int EEPROM_ADDR_ALARM_SEC = 2;
constexpr int EEPROM_ADDR_ALARM_ACTIVE = 3;

AlarmClock::AlarmClock(RotaryEncoder &encoder, Button &confirmButton, Button &snoozeButton,
                       SwitchInput &activeSwitch, Led &signalLed, HTTPManager &httpManager, NTPManager &ntpManager)
    : _encoder(encoder), _confirmButton(confirmButton), _snoozeButton(snoozeButton),
      _activeSwitch(activeSwitch), _signalLed(signalLed), _httpManager(httpManager), _ntpManager(ntpManager),
      _alarmHours(7), _alarmMinutes(0), _alarmSeconds(0),
      _alarmActive(false), _snoozed(false)
{
    loadAlarmFromEEPROM();
}

AlarmClock::~AlarmClock() {}

void AlarmClock::loadAlarmFromEEPROM()
{
    _alarmHours = EEPROM.read(EEPROM_ADDR_ALARM_HOUR);
    _alarmMinutes = EEPROM.read(EEPROM_ADDR_ALARM_MIN);
    _alarmSeconds = EEPROM.read(EEPROM_ADDR_ALARM_SEC);
    _alarmActive = (EEPROM.read(EEPROM_ADDR_ALARM_ACTIVE) != 0);

    Serial.printf("Geladene Alarmzeit: %02d:%02d:%02d, aktiv: %s\n",
                  _alarmHours, _alarmMinutes, _alarmSeconds, _alarmActive ? "ja" : "nein");
}

void AlarmClock::saveAlarmToEEPROM()
{
    EEPROM.write(EEPROM_ADDR_ALARM_HOUR, _alarmHours);
    EEPROM.write(EEPROM_ADDR_ALARM_MIN, _alarmMinutes);
    EEPROM.write(EEPROM_ADDR_ALARM_SEC, _alarmSeconds);
    EEPROM.write(EEPROM_ADDR_ALARM_ACTIVE, _alarmActive ? 1 : 0);
    EEPROM.commit();
    Serial.printf("Gespeicherte Alarmzeit: %02d:%02d:%02d, aktiv: %s\n",
                  _alarmHours, _alarmMinutes, _alarmSeconds, _alarmActive ? "ja" : "nein");
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
            _alarmHours += increment;
            if (_alarmHours < 0)
                _alarmHours = 23;
            if (_alarmHours > 23)
                _alarmHours = 0;
            timeChanged = true;
            break;
        case RotaryEncoder::Mode::Minutes:
            _alarmMinutes += increment;
            if (_alarmMinutes < 0)
                _alarmMinutes = 59;
            if (_alarmMinutes > 59)
                _alarmMinutes = 0;
            timeChanged = true;
            break;
        case RotaryEncoder::Mode::Seconds:
            _alarmSeconds += increment;
            if (_alarmSeconds < 0)
                _alarmSeconds = 59;
            if (_alarmSeconds > 59)
                _alarmSeconds = 0;
            timeChanged = true;
            break;
        }
        _encoder.resetIncrement();
    }

    if (_encoder.isButtonPressed())
    {
        _encoder.switchMode();
        timeChanged = true;
        vTaskDelay(50);
    }

    if (_confirmButton.isPressed())
    {
        bool prevActive = _alarmActive;
        _alarmActive = !_alarmActive;
        if (prevActive != _alarmActive)
        {
            timeChanged = true;
        }
        vTaskDelay(50);
    }

    if (_activeSwitch.isActive())
    {
        if (!_alarmActive)
        {
            _alarmActive = true;
            timeChanged = true;
        }
    }

    if (timeChanged)
    {
        saveAlarmToEEPROM();
        sendDisplayUpdate();
    }
}

void AlarmClock::sendDisplayUpdate()
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer),
             "{\"hours\": %d, \"minutes\": %d, \"seconds\": %d}",
             _alarmHours, _alarmMinutes, _alarmSeconds);
    _httpManager.sendDisplayData(buffer);
}

void AlarmClock::checkAndActivateLight()
{
    time_t nowTime = _ntpManager.getCurrentTime();
    struct tm *timeinfo = localtime(&nowTime);

    int currentSeconds = timeinfo->tm_hour * 3600 + timeinfo->tm_min * 60 + timeinfo->tm_sec;
    int alarmSeconds = _alarmHours * 3600 + _alarmMinutes * 60 + _alarmSeconds;

    int diff = alarmSeconds - currentSeconds;

    bool shouldLightBeOn = false;
    if (_alarmActive && diff > 0 && diff <= 1800)
    {
        shouldLightBeOn = true;
    }

    static bool lastLightState = false;
    if (shouldLightBeOn != lastLightState)
    {
        if (shouldLightBeOn)
        {
            _httpManager.sendLightCommand(true);
        }
        else
        {
            _httpManager.sendLightCommand(false);
        }
        lastLightState = shouldLightBeOn;
    }
}

void AlarmClock::processInputs()
{
    updateAlarmTime();
}

void AlarmClock::processHTTP()
{
    sendDisplayUpdate();
    checkAndActivateLight();
}