#include <driver/gpio.h>
#include <fu_pin.h>
#include "fu_rly.h"

Relay::Relay(gpio_num_t pin, bool reverse) : pin(pin), reverse(reverse) {
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    gpio_set_level(pin, reverse);
    status = reverse;
}

void Relay::on() {
    gpio_set_level(pin, !reverse);
    status = !reverse;
}

void Relay::off() {
    gpio_set_level(pin, reverse);
    status = reverse;
}

bool Relay::getStatus() {
    return status;
}

void Relay::setReverse(bool reverse) {
    this->reverse = reverse;
}

bool Relay::isReverse() {
    return reverse;
}

void SimpleMotor::setStatus(MOTOR_STATUS st) {
    status = st;
    switch (st) {
        case STOP:
            en.off();
            ph.off();
            break;
        case FORWARD:
            ph.off();
            en.on();
            break;
        case REVERSE:
            ph.on();
            en.on();
            break;
    }
}

MOTOR_STATUS SimpleMotor::getStatus() {
    return status;
}

Relay SimpleMotor::en = Relay(static_cast<gpio_num_t>(PIN_RLY_EN));
Relay SimpleMotor::ph = Relay(static_cast<gpio_num_t>(PIN_RLY_PH));
MOTOR_STATUS SimpleMotor::status = MOTOR_STATUS::STOP;