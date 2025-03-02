#include "Button.h"
#include <Arduino.h>

Button::Button(int pin1, int pin2)
    : _pin1(pin1), _pin2(pin2), _lastDebounceTime1(0), _lastDebounceTime2(0), _lastButtonState1(HIGH), _lastButtonState2(HIGH)
{
    pinMode(_pin1, INPUT_PULLUP);
    pinMode(_pin2, INPUT_PULLUP);
}

Button::~Button() {}

bool Button::isPressed(int button)
{
    int pin = (button == 1) ? _pin1 : _pin2;
    unsigned long &lastDebounceTime = (button == 1) ? _lastDebounceTime1 : _lastDebounceTime2;
    bool &lastButtonState = (button == 1) ? _lastButtonState1 : _lastButtonState2;
    
    bool reading = (digitalRead(pin) == LOW);
    unsigned long currentTime = millis();

    if (reading != lastButtonState)
    {
        lastDebounceTime = currentTime;
        lastButtonState = reading;
    }

    if ((currentTime - lastDebounceTime) > DEBOUNCE_DELAY)
    {
        return reading;
    }
    
    return false;
}
