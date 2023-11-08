static const char* TAG = "SYSTEM";

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <nvs_flash.h>
#include <esp_event.h>
#include <esp_log.h>
#include <fu_led.h>
#include <fu_system.h>
#include <fu_iot.h>
#include <fu_nvs.h>
#include <grepfa_net.h>
#include <driver/gpio.h>


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

    BtnRegister();

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

    setLedMode(LED_STATUS, LED_MODE_SLOW_BLINK);
    ESP_LOGI(TAG, "device initialized");

    return ESP_OK;
}

esp_err_t FuSystem::NetworkingInitializer() {
    ESP_LOGI(TAG, "network interface initialized");
    char buf[100];
    sprintf(buf, "grepfa_%s", FuNVS::GetDeviceName());
    GrepfaNet::SetDeviceServiceName(buf);
    return GrepfaNet::Init(SPI2_HOST, PIN_ETH_MOSI, PIN_ETH_MISO, PIN_ETH_SCK, PIN_ETH_CS, PIN_ETH_RST, PIN_ETH_INT);
}

esp_err_t FuSystem::NetworkingConnect() {
    return GrepfaNet::Start();;
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
    vTaskDelay(1000/portTICK_PERIOD_MS);
    FuIoT::reconnect();
    return 0;
}



void FuSystem::BtnRegister() {
//    gpio_config_t io_conf = {};
//
//    io_conf.intr_type = GPIO_INTR_NEGEDGE;
//    io_conf.mode = GPIO_MODE_INPUT;
//    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
//    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
//    io_conf.pin_bit_mask = 1ULL < PIN_BTN_RST;
//
//    gpio_config(&io_conf);
    gpio_set_intr_type(static_cast<gpio_num_t>(PIN_BTN_RST), GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(0);

    gpio_isr_handler_add(static_cast<gpio_num_t>(PIN_BTN_RST), BtnRtsHandler, nullptr);
}

void IRAM_ATTR FuSystem::BtnRtsHandler(void *) {
    xTaskCreate([](void*){
        gpio_set_intr_type(static_cast<gpio_num_t>(PIN_BTN_RST), GPIO_INTR_DISABLE);
        ESP_LOGI(TAG, "SYSTEM RESTART");
        GrepfaNet::ResetProvisioning();
        esp_restart();
        }, "reboot task", 2048, nullptr, 21, nullptr);
}
