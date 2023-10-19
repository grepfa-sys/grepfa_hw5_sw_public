#pragma once

#include <esp_err.h>
#include <mqtt_client.h>

#define IOT_SERVER_ENDPOINT "mqtts://mqtt.grepfa.com"

#define DEVICE_NAME "test-device"

#define SHADOW_REQUEST_TOPIC "$aws/things/%s/shadow/name/channels/update/delta"
#define SHADOW_REPORT_TOPIC "$aws/things/%s/shadow/name/channels/update"

#define DEFAULT_QOS 1

//#define SHADOW_METADATA_TOPIC "$aws/things/%s/shadow/name/metadata/update"
//#define SHADOW_CHANNEL_TYPES_TOPIC "$aws/things/%s/shadow/name/channels_info/update"


class FuIoT {
public:
    static esp_err_t init();

private:
    static char reqTopic[512];
    static char reportTopic[512];
    static esp_mqtt_client_handle_t client;
    static void mqtt_data_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
    static void mqtt_disconnected_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

    static void mqtt_request_handler(const char *topic, const char *data);
};

