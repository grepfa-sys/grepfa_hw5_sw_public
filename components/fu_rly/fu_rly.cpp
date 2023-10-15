#include <driver/gpio.h>
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



