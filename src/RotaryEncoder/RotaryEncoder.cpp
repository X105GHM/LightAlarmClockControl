#include "RotaryEncoder.h"
#include <Arduino.h>

RotaryEncoder::RotaryEncoder(int pinA, int pinB, int buttonPin)
    : _pinA(pinA), _pinB(pinB), _buttonPin(buttonPin), _increment(0), _mode(Mode::Hours)
{
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
    pinMode(_buttonPin, INPUT_PULLUP);
}

RotaryEncoder::~RotaryEncoder() {}

void RotaryEncoder::update()
{
    static int lastState = digitalRead(_pinA);
    int currentState = digitalRead(_pinA);
    if (currentState != lastState)
    {
        if (digitalRead(_pinB) != currentState)
        {
            _increment++;
        }
        else
        {
            _increment--;
        }
        lastState = currentState;
    }
}

int RotaryEncoder::getIncrement() const
{
    return _increment;
}

void RotaryEncoder::resetIncrement()
{
    _increment = 0;
}

bool RotaryEncoder::isButtonPressed() const
{
    return (digitalRead(_buttonPin) == LOW);
}

void RotaryEncoder::switchMode()
{
    if (_mode == Mode::Hours)
        _mode = Mode::Minutes;
    else if (_mode == Mode::Minutes)
        _mode = Mode::Seconds;
    else
        _mode = Mode::Hours;
}

RotaryEncoder::Mode RotaryEncoder::getMode() const
{
    return _mode;
}