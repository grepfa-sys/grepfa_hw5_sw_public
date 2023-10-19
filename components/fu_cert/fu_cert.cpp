#include "fu_cert.h"


const char *FuCert::RootCA() {
    char *addr;
    uint32_t len;
    esp_secure_cert_get_ca_cert(&addr, &len);

    return addr;
}

const char *FuCert::ClientCRT() {
    char *addr;
    uint32_t len;
    esp_secure_cert_get_device_cert(&addr, &len);

    return addr;
}

#ifndef CONFIG_ESP_SECURE_CERT_DS_PERIPHERAL
const char *FuCert::ClientKEY() {
    char *addr;
    uint32_t len;
    esp_secure_cert_get_priv_key(&addr, &len);
    return addr;
}
#else
esp_ds_data_ctx_t *FuCert::dsContext() {
    return esp_secure_cert_get_ds_ctx();
}
#endif