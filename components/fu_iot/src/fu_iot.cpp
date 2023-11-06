//
// Created by vl0011 on 23. 10. 19.
//

static const char* TAG = "MQTT";
#include <esp_log.h>
#include <fu_cert.h>
#include <fu_rly.h>
#include <fu_nvs.h>
#include "fu_iot.h"
#include "ArduinoJson.h"

esp_mqtt_client_handle_t FuIoT::client = nullptr;
char FuIoT::reqTopic[512];
char FuIoT::reportTopic[512];

esp_err_t FuIoT::init() {

    esp_mqtt_client_config_t cfg = {
            .broker = {
                    .address = {
                            .uri = FuNVS::GetBrokerEndpoint()
                    },
                    .verification = {
                            .certificate = FuCert::RootCA()
                    }
            },
            .credentials = {
                    .authentication = {
                            .certificate = FuCert::ClientCRT(),
#ifndef CONFIG_ESP_SECURE_CERT_DS_PERIPHERAL
                            .key = FuCert::ClientKEY(),
#else
                            .key = nullptr,
                            .ds_data = FuCert::dsContext(),
#endif
                    }
            }
    };

    sprintf(reqTopic, SHADOW_REQUEST_TOPIC, FuNVS::GetDeviceName());
    sprintf(reportTopic, SHADOW_REPORT_TOPIC, FuNVS::GetDeviceName());

    client = esp_mqtt_client_init(&cfg);
    esp_mqtt_client_register_event(client, MQTT_EVENT_DATA, mqtt_data_handler, nullptr);
    esp_mqtt_client_register_event(client, MQTT_EVENT_DISCONNECTED, mqtt_disconnected_handler, nullptr);
    auto err = esp_mqtt_client_start(client);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "start mqtt fail");
        return err;
    }

    ESP_LOGI(TAG, "sub - %s", reqTopic);
    esp_mqtt_client_subscribe(client, reqTopic, DEFAULT_QOS);

    return ESP_OK;
}

void FuIoT::mqtt_data_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    auto event = static_cast<esp_mqtt_event_handle_t>(event_data);
//    ESP_LOGI(TAG, "%s", event->data);
//    ESP_LOGI(TAG, "%s", event->topic);

    if (!strncmp(event->topic, reqTopic, event->topic_len)) {
        mqtt_request_handler(event->topic, event->data);
    }
}

void FuIoT::mqtt_request_handler(const char *topic, const char *data) {
    ESP_LOGI(TAG, "%s", data);
    DynamicJsonDocument doc(2048), report(2048);
    deserializeJson(doc, data);

    auto x = doc["state"]["actuator-1"]["state"].as<double>();

    report["state"]["reported"]["actuator-1"]["model"] = 1;

    if (x == 0) {
        SimpleMotor::setStatus(MOTOR_STATUS::STOP);
        report["state"]["reported"]["actuator-1"]["state"] = 0;
    }
    else if (x > 0) {
        SimpleMotor::setStatus(MOTOR_STATUS::FORWARD);
        report["state"]["reported"]["actuator-1"]["state"] = 1;
    }
    else {
        SimpleMotor::setStatus(MOTOR_STATUS::REVERSE);
        report["state"]["reported"]["actuator-1"]["state"] = -1;
    }

    std::string str;
    serializeJson(report, str);

    ESP_LOGI(TAG, "%s", str.c_str());

    esp_mqtt_client_publish(client, reportTopic, str.c_str(), str.length(), 1, 0);
    ESP_LOGI(TAG, "reported - %s", str.c_str());
}

void FuIoT::mqtt_disconnected_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGW(TAG, "server disconnected");
}