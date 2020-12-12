#include <stdio.h>

#include "esp_event.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "mqtt.h"
#include "gpio.h"
#include "nvs_flash.h"
#include "wifi.h"
#include "parse.h"

xSemaphoreHandle wifiSemaphore;
xSemaphoreHandle configSemaphore;

void watch_wifi(void *params) {
    while (true) {
        if (xSemaphoreTake(wifiSemaphore, portMAX_DELAY)) {
            ESP_LOGI("MAIN", "Wifi OK, init MQTT");
            mqtt_start();

            vTaskDelete(NULL);
        }
    }
}

void init_nvs() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void app_main(void) {
    wifiSemaphore = xSemaphoreCreateBinary();
    configSemaphore = xSemaphoreCreateBinary();

    init_nvs(); // verifica se configurou
    init_gpio();
    init_wifi();

    wifi_start();

    xTaskCreate(&watch_wifi, "Monitora Wifi", 4096, NULL, 1, NULL);

    if (xSemaphoreTake(configSemaphore, portMAX_DELAY)){
        xTaskCreate(&watch_button, "Monitora Botão", 4096, NULL, 1, NULL);
        xTaskCreate(&read_dht11, "Ler dht11", 4096, NULL, 1, NULL);
    }
    

}
