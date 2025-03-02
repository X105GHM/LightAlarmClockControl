#include <Arduino.h>
#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(int pinA, int pinB, int buttonPin)
    : _pinA(pinA), _pinB(pinB), _buttonPin(buttonPin),
      _lastStateA(HIGH), _lastStateB(HIGH), _lastRotationDebounceTime(0),
      _lastButtonState(HIGH), _lastButtonPressTime(0), _buttonPressed(false),
      _increment(0), _mode(Mode::Hours)
{
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
    pinMode(_buttonPin, INPUT_PULLUP);
}

RotaryEncoder::~RotaryEncoder() {}

void RotaryEncoder::update()
{
    unsigned long currentTime = millis();

    int currentStateA = digitalRead(_pinA);
    int currentStateB = digitalRead(_pinB);

    if (currentStateA != _lastStateA && currentStateA == LOW)
    {
        if (currentStateB == HIGH)
        {
            _increment++;
        }
        else
        {
            _increment--;
        }
        _lastRotationDebounceTime = currentTime;
    }
    _lastStateA = currentStateA;

    int currentButtonState = digitalRead(_buttonPin);
    if (currentButtonState != _lastButtonState)
    {
        if (currentButtonState == LOW && (currentTime - _lastButtonPressTime >= BUTTON_DEBOUNCE_DELAY))
        {
            _buttonPressed = true;
            _lastButtonPressTime = currentTime;
        }
        _lastButtonState = currentButtonState;
    }
}

int RotaryEncoder::getIncrement()
{
    int temp = _increment;
    _increment = 0;
    return temp;
}

void RotaryEncoder::resetIncrement()
{
    _increment = 0;
}

bool RotaryEncoder::isButtonPressed()
{
    bool wasPressed = _buttonPressed;
    _buttonPressed = false;
    return wasPressed;
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
