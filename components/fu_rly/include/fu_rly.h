#pragma once

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
    SimpleMotor(Relay enable, Relay phase);

    void setStatus(MOTOR_STATUS status);
    MOTOR_STATUS getStatus();

private:
    MOTOR_STATUS status;

};

