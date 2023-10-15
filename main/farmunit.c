#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "fu_led.h"

void app_main(void)
{
    initLed();

    setLedMode(LED_WIFI, LED_MODE_OFF);
    setLedMode(LED_ETH, LED_MODE_ON);
    setLedMode(LED_PROV, LED_MODE_FAST_BLINK);
    setLedMode(LED_STATUS, LED_MODE_SLOW_BLINK);

    while (1) {
        vTaskDelay(portMAX_DELAY);
    }
}
