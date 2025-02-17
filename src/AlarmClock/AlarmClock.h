#ifndef ALARM_CLOCK_H
#define ALARM_CLOCK_H

#include "RotaryEncoder/RotaryEncoder.h"
#include "Button/Button.h"
#include "SwitchInput/Switchinput.h"
#include "Led/LED.h"
#include "HTTPManager/HTTPManager.h"
#include "NTPManager/NTPManager.h"

constexpr int EEPROM_SIZE = 16;
constexpr int EEPROM_ADDR_ALARM_HOUR = 0;
constexpr int EEPROM_ADDR_ALARM_MIN = 1;
constexpr int EEPROM_ADDR_ALARM_SEC = 2;
constexpr int EEPROM_ADDR_ALARM_ACTIVE = 3;

class AlarmClock
{
public:

    AlarmClock(RotaryEncoder &encoder, Button &confirmButton, Button &snoozeButton,
               SwitchInput &activeSwitch, Led &signalLed, HTTPManager &httpManager, NTPManager &ntpManager);
    ~AlarmClock();
    void processInputs();
    void processHTTP();

private:

    RotaryEncoder &_encoder;
    Button &_confirmButton;
    Button &_snoozeButton;
    SwitchInput &_activeSwitch;
    Led &_signalLed;
    HTTPManager &_httpManager;
    NTPManager &_ntpManager;

    int _alarmHours;
    int _alarmMinutes;
    int _alarmSeconds;
    bool _alarmActive;
    bool _snoozed;

    void updateAlarmTime();
    void sendDisplayUpdate();
    void checkAndActivateLight();
    void loadAlarmFromEEPROM();
    void saveAlarmToEEPROM();
};

#endif // ALARM_CLOCK_H