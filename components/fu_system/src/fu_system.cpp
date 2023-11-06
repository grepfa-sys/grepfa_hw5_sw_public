static const char* TAG = "SYSTEM";

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <nvs_flash.h>
#include <esp_event.h>

#include <eth.h>
#include <wifi.h>
#include <fu_led.h>
#include <fu_system.h>
#include <fu_iot.h>
#include <fu_nvs.h>

void FuSystem::SystemInit() {

    // get system config
    ESP_LOGI(TAG, "system start");
    esp_err_t err;

    // critical system initialization
    err = CriticalBooting();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "System Initializer Error Occurred");
        esp_restart();
    }
    ESP_LOGI(TAG, "System initialized");

    ESP_LOGI(TAG, "get system configuration");
    err = ConfigurationGetter();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "System Configuration Manager Error Occurred");
        esp_restart();
    }
    ESP_LOGI(TAG, "Get system settings");

    // net interface initialization
    err = NetworkingInitializer();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Net Interface Initializer Error Occurred");
        esp_restart();
    }
    ESP_LOGI(TAG, "Net Interface initialized");

    setLedMode(LED_STATUS, LED_MODE_OFF);

    NetworkingConnect();

    IoTServerConnectionInitializer();
}

esp_err_t FuSystem::ConfigurationGetter() {
    esp_err_t err = FuNVS::Init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Get NVS Fail");
        return err;
    }

    return ESP_OK;
}

esp_err_t FuSystem::CriticalBooting() {
    ESP_LOGI(TAG, "device initializer start");
    initLed();

    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ret = nvs_flash_erase();
        if (ret != ESP_OK) {
            return ret;
        }
        ret = nvs_flash_init();
        if (ret != ESP_OK) {
            return ret;
        }
    }

    ret = esp_event_loop_create_default();

    if (ret != ESP_OK) {
        return ret;
    }

    ret = GrepfaNetInterface::Init();
    if (ret != ESP_OK) {
        return ret;
    }

    setLedMode(LED_STATUS, LED_MODE_SLOW_BLINK);
    ESP_LOGI(TAG, "device initialized");

    return ESP_OK;
}

esp_err_t FuSystem::NetworkingInitializer() {
    // ETH INIT
    ESP_LOGI(TAG, "eth initializer start");
    GrepfaETH::SetClock(10);
    GrepfaETH::HandlerStop = [](){ setLedMode(LED_ETH, LED_MODE_OFF); };
    GrepfaETH::HandlerDisconnected = [](){ setLedMode(LED_ETH, LED_MODE_SLOW_BLINK); };
    GrepfaETH::HandlerConnected = [](){ setLedMode(LED_ETH, LED_MODE_ON); };

    esp_err_t err = GrepfaETH::Init(SPI2_HOST, PIN_ETH_MOSI, PIN_ETH_MISO, PIN_ETH_SCK, PIN_ETH_CS, PIN_ETH_RST, PIN_ETH_INT);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "ethernet init fail");
        return err;
    }

    // WIFI INIT
    ESP_LOGI(TAG, "wifi initializer start");
    GrepfaWIFI::HandlerStop = [](){ setLedMode(LED_ETH, LED_MODE_OFF); };
    GrepfaWIFI::HandlerDisconnected = [](){ setLedMode(LED_WIFI, LED_MODE_SLOW_BLINK); };
    GrepfaWIFI::HandlerConnected = [](){ setLedMode(LED_WIFI, LED_MODE_ON); };

    err = GrepfaWIFI::Init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "wifi init fail");
        esp_restart();
    }

    ESP_LOGI(TAG, "network interface initialized");
    return ESP_OK;
}

esp_err_t FuSystem::NetworkingConnect() {
    GrepfaETH::Connect();
    GrepfaWIFI::AddAP("sys24g", "shin0114");
    GrepfaWIFI::AddAP("grepfa", "12345678");

    GrepfaWIFI::Connect();
    return 0;
}

esp_err_t FuSystem::IoTServerConnectionInitializer() {
    FuIoT::HandlerConnected = [](){
        setLedMode(LED_STATUS, LED_MODE_OFF);
    };

    FuIoT::HandlerDisconnected = [](){
        setLedMode(LED_STATUS, LED_MODE_FAST_BLINK);
        FuSystem::NetworkFixingAndRepairing();
    };

    return FuIoT::init();
}

esp_err_t FuSystem::NetworkFixingAndRepairing() {
    vTaskDelay(500/portTICK_PERIOD_MS);
    FuIoT::reconnect();
    return 0;
}


