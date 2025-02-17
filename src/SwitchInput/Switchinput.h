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
};

#endif // SWITCH_INPUT_H
