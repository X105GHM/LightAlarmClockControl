#ifndef ALARM_CLOCK_H
#define ALARM_CLOCK_H

#include "RotaryEncoder/RotaryEncoder.h"
#include "Button/Button.h"
#include "SwitchInput/Switchinput.h"
#include "Led/LED.h"
#include "HTTPManager/HTTPManager.h"
#include "NTPManager/NTPManager.h"
#include <Preferences.h>

class AlarmClock
{
public:
    AlarmClock(RotaryEncoder &encoder, Button &button,
               SwitchInput &activeSwitch, Led &signalLed, HTTPManager &httpManager, NTPManager &ntpManager, Preferences &preferences);
    ~AlarmClock();
    void processInputs();
    void processHTTP();
    void loadAlarmFromPreferences();

private:
    RotaryEncoder &_encoder;
    Button &_button;
    SwitchInput &_activeSwitch;
    Led &_signalLed;
    HTTPManager &_httpManager;
    NTPManager &_ntpManager;
    Preferences &_preferences;

    int _alarmHours;
    int _alarmMinutes;
    int _alarmSeconds;
    bool _alarmActive;
    bool _snoozed;

    void updateAlarmTime();
    void updateAlarmActiveState();
    void sendDisplayUpdate();
    void checkAndActivateLight(); 
    void saveAlarmToPreferences();
};

#endif // ALARM_CLOCK_H
