#include "Button.h"
#include <Arduino.h>

Button::Button(int pin) : _pin(pin)
{
    pinMode(_pin, INPUT_PULLUP);
}

Button::~Button() {}

bool Button::isPressed()
{
    return (digitalRead(_pin) == LOW);
}
