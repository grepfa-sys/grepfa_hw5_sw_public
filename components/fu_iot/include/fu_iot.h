#pragma once

#include <esp_err.h>
#include <mqtt_client.h>
#include <functional>

#define SHADOW_REQUEST_TOPIC "$aws/things/%s/shadow/name/channels/update/delta"
#define SHADOW_REPORT_TOPIC "$aws/things/%s/shadow/name/channels/update"

#define SHADOW_INIT_ACCEPT_TOPIC "$aws/things/%s/shadow/name/channels/get/accepted"
#define SHADOW_INIT_REQUEST_TOPIC "$aws/things/%s/shadow/name/channels/get"

#define DEFAULT_QOS 1

class FuIoT {
public:
    static esp_err_t init();
    static std::function<void()> HandlerDisconnected;
    static std::function<void()> HandlerConnected;

    static esp_err_t reconnect();

private:
    static char reqTopic[512];
    static char reportTopic[512];
    static char initAcceptTopic[512];
    static char initReqTopic[512];

    static esp_mqtt_client_handle_t client;
    static void mqtt_data_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
    static void mqtt_disconnected_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
    static void mqtt_connected_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

    static void mqtt_request_handler(const char *topic, const char *data);
};

