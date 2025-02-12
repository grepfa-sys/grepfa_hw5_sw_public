#pragma once

#include <esp_err.h>

class FuSystem{
public:
    static void SystemInit();

private:
    static esp_err_t CriticalBooting();
    static esp_err_t ConfigurationGetter();
    static esp_err_t NetworkingInitializer();
    static esp_err_t NetworkingConnect();
    static esp_err_t IoTServerConnectionInitializer();
    static esp_err_t NetworkFixingAndRepairing();
    static void BtnRegister();

    static void IRAM_ATTR BtnRtsHandler(void*);
};