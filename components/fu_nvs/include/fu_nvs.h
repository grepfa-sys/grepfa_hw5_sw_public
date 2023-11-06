#pragma once

#include <esp_err.h>
#include <nvs_flash.h>
#include <nvs.h>

#define GREPFA_FACTORY_CONFIG_NAMESPACE "grepfa_factory"
#define GREPFA_USERSPACE_CONFIG_NAMESPACE "grepfa_config"

#define GREPFA_CONFIG_KEY_BROKER_EP "broker_ep"
#define GREPFA_CONFIG_KEY_DEVICE_NAME "device_name"

typedef struct{
    char broker_ep[512];
    char device_name[64];
} grepfa_factory_config_t;


class GrepfaNVS{
private:
    static grepfa_factory_config_t factory_config;
    static nvs_handle_t factory_config_handler;
    static nvs_handle_t user_config_handler;
public:
    static esp_err_t Init();
};