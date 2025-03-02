#ifndef SWITCH_INPUT_H
#define SWITCH_INPUT_H

class SwitchInput
{
public:

    SwitchInput(int pin);
    ~SwitchInput();
    bool isActive();

private:

    int _pin;
    unsigned long _lastDebounceTime;
    bool _lastSwitchState;
    static const unsigned long DEBOUNCE_DELAY = 500;
};

#endif // SWITCH_INPUT_H
