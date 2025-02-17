#ifndef LED_H
#define LED_H

class Led
{
public:

    Led(int pin);
    ~Led();

    void on();
    void off();
    void toggle();
    bool isOn() const;

private:

    int _pin;
    bool _state;
};

#endif // LED_H
