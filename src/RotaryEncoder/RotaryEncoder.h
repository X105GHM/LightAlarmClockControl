#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <Arduino.h>
#include <cstdint>

class RotaryEncoder
{
public:
    enum class Mode
    {
        Hours,
        Minutes,
        Seconds
    };

    RotaryEncoder(int pinA, int pinB, int buttonPin);
    ~RotaryEncoder();
    void update();
    int getIncrement();
    void resetIncrement();
    bool isButtonPressed();
    void switchMode();
    Mode getMode() const;

private:
    int _pinA;
    int _pinB;
    int _buttonPin;
    int _lastStateA;
    int _lastStateB;
    unsigned long _lastRotationDebounceTime;
    static constexpr unsigned long ROTARY_DEBOUNCE_DELAY = 5;
    int _lastButtonState;
    unsigned long _lastButtonPressTime;
    static constexpr unsigned long BUTTON_DEBOUNCE_DELAY = 1000;
    bool _buttonPressed;

    int _increment;
    Mode _mode;
};

#endif // ROTARY_ENCODER_H