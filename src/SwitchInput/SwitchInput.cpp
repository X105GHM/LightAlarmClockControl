#include "SwitchInput.h"
#include <Arduino.h>

SwitchInput::SwitchInput(int pin) : _pin(pin)
{
    pinMode(_pin, INPUT_PULLUP);
}

SwitchInput::~SwitchInput() {}

bool SwitchInput::isActive()
{
    return (digitalRead(_pin) == LOW);
}
