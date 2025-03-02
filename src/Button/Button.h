#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

class Button
{
public:
    Button(int pin1, int pin2);
    ~Button();
    bool isPressed(int button);

private:
    int _pin1;
    int _pin2;
    unsigned long _lastDebounceTime1;
    unsigned long _lastDebounceTime2;
    bool _lastButtonState1;
    bool _lastButtonState2;
    static const unsigned long DEBOUNCE_DELAY = 200;
};

#endif // BUTTON_H
