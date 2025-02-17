#ifndef BUTTON_H
#define BUTTON_H

class Button
{
public:

    Button(int pin);
    ~Button();

    bool isPressed();

private:

    int _pin;
};

#endif // BUTTON_H
