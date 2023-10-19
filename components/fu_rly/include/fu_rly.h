#pragma once

#include <driver/gpio.h>

class Relay {
public:
    Relay(gpio_num_t pin, bool reverse = false);

    void on();
    void off();

    bool getStatus();
    void setReverse(bool reverse = true);
    bool isReverse();

private:
    bool status = false;
    gpio_num_t pin = GPIO_NUM_NC;
    bool reverse = false;
};

typedef enum {
    STOP = 0,
    FORWARD,
    REVERSE
}MOTOR_STATUS;

class SimpleMotor{
public:
    static void setStatus(MOTOR_STATUS st);
    static MOTOR_STATUS getStatus();

private:
    static Relay en, ph;
    static MOTOR_STATUS status;
};

