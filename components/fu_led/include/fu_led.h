#pragma once

#include <fu_pin.h>
#include <esp_err.h>

#define LED_BLINK_DELAY 500
#define LED_TASK_STACK_SIZE 1024
#define LED_TASK_STACK_PRIORITY 4

typedef enum {
    LED_MODE_OFF = 0,
    LED_MODE_ON,
    LED_MODE_FAST_BLINK,
    LED_MODE_SLOW_BLINK
}
LED_MODE;

typedef enum {
    LED_WIFI = 0,
    LED_ETH,
    LED_PROV,
    LED_STATUS,
} FU_LED;

void initLed(void);
void setLedMode(FU_LED led, LED_MODE mode);

