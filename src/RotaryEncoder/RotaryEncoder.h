#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

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
    int getIncrement() const;
    void resetIncrement();
    bool isButtonPressed() const;
    void switchMode();
    Mode getMode() const;

private:
    int _pinA;
    int _pinB;
    int _buttonPin;
    int _increment;
    Mode _mode;
};

#endif // ROTARY_ENCODER_H
