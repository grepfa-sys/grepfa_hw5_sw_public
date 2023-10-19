#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "fu_system.h"


extern "C" void app_main(void)
{
    FuSystem::SystemInit();

    while (1) {
        vTaskDelay(portMAX_DELAY);
    }
}
