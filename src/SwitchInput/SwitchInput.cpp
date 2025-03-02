#include "SwitchInput.h"
#include <Arduino.h>

SwitchInput::SwitchInput(int pin)
    : _pin(pin), _lastDebounceTime(0), _lastSwitchState(HIGH)
{
    pinMode(_pin, INPUT_PULLUP);
}

SwitchInput::~SwitchInput() {}

bool SwitchInput::isActive()
{
    bool reading = (digitalRead(_pin) == LOW);
    unsigned long currentTime = millis();

    if (reading != _lastSwitchState)
    {
        _lastDebounceTime = currentTime;
        _lastSwitchState = reading;
    }

    if ((currentTime - _lastDebounceTime) > DEBOUNCE_DELAY)
    {
        return reading;
    }

    return false;
}
