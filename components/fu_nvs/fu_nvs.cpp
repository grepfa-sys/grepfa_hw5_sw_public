#include "fu_nvs.h"
#include <esp_log.h>

const char* TAG = "config";

grepfa_factory_config_t GrepfaNVS::factory_config = {};
nvs_handle_t GrepfaNVS::factory_config_handler;
nvs_handle_t GrepfaNVS::user_config_handler;

esp_err_t GrepfaNVS::Init() {
    esp_err_t err = nvs_open(GREPFA_FACTORY_CONFIG_NAMESPACE, NVS_READONLY, &factory_config_handler);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "open factory config fail");
        return err;
    }
    err = nvs_open(GREPFA_USERSPACE_CONFIG_NAMESPACE, NVS_READWRITE, &user_config_handler);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "open user config fail");
        return err;
    }

    size_t size = 512;
    err = nvs_get_str(
            factory_config_handler,
            GREPFA_CONFIG_KEY_BROKER_EP,
            factory_config.broker_ep,
            &size
    );

    factory_config.broker_ep[size] = '\0';

    if(err != ESP_OK) {
        ESP_LOGE(TAG, "get broker endpoint fail");
    }
    size = 512;
    err = nvs_get_str(
            factory_config_handler,
            GREPFA_CONFIG_KEY_DEVICE_NAME,
            factory_config.device_name,
            &size
    );
    if(err != ESP_OK) {
        ESP_LOGE(TAG, "get device name fail");
    }
    factory_config.device_name[size] = '\0';
        return 0;
}
