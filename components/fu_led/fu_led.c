#include "fu_led.h"
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/*
 * | T1  | T2  | T3  | T4  |
 * | OFF | OFF | OFF | OFF |    LED_MODE_OFF
 * | ON  | ON  | ON  | ON  |    LED_MODE_ON
 * | OFF | ON  | OFF | ON  |    LED_MODE_FAST_BLINK
 * | OFF | OFF | ON  | ON  |    LED_MODE_SLOW_BLINK
 */

int led_mode[4] = {LED_MODE_OFF, LED_MODE_OFF, LED_MODE_OFF, LED_MODE_OFF};
gpio_num_t pins[4] = {
        PIN_LED_WIFI,
        PIN_LED_ETH,
        PIN_LED_PROV,
        PIN_LED_STATUS,
};

static void ledTask(void*)
{
    while (1) {
        // T1
        for (int i = 0; i < 4; ++i) {
            if (led_mode[i] == LED_MODE_FAST_BLINK || led_mode[i] == LED_MODE_SLOW_BLINK) {
                gpio_set_level(pins[i], 0);
            }
        }
        vTaskDelay(LED_BLINK_DELAY/portTICK_PERIOD_MS);

        // T2
        for (int i = 0; i < 4; ++i) {
            if (led_mode[i] == LED_MODE_FAST_BLINK) {
                gpio_set_level(pins[i], 1);
            }
        }
        vTaskDelay(LED_BLINK_DELAY/portTICK_PERIOD_MS);

        // T3
        for (int i = 0; i < 4; ++i) {
            if (led_mode[i] == LED_MODE_FAST_BLINK) {
                gpio_set_level(pins[i], 0);
            } else if(led_mode[i] == LED_MODE_SLOW_BLINK) {
                gpio_set_level(pins[i], 1);
            }
        }
        vTaskDelay(LED_BLINK_DELAY/portTICK_PERIOD_MS);

        // T4
        for (int i = 0; i < 4; ++i) {
            if (led_mode[i] == LED_MODE_FAST_BLINK) {
                gpio_set_level(pins[i], 1);
            }
        }
        vTaskDelay(LED_BLINK_DELAY/portTICK_PERIOD_MS);
    }
}

void initLed(void) {
    for (int i = 0; i < 4; ++i) {
        gpio_reset_pin(pins[i]);
        gpio_set_direction(pins[i], GPIO_MODE_OUTPUT);
        gpio_set_level(pins[i], 1);
    }

    xTaskCreate(ledTask, "led_task", LED_TASK_STACK_SIZE, NULL, LED_TASK_STACK_PRIORITY, NULL);
}

void setLedMode(FU_LED led, LED_MODE mode) {
    led_mode[led] = mode;

    if (mode == LED_MODE_OFF) {
        gpio_set_level(pins[led], 0);
    } else if (mode == LED_MODE_ON) {
        gpio_set_level(pins[led], 1);
    }
}