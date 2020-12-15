#include <stdio.h>

#include "esp_event.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "gpio.h"
#include "mqtt.h"
#include "wifi.h"
#include "nvs.h"

xSemaphoreHandle wifiSemaphore;
xSemaphoreHandle configSemaphore;

void watch_button(void* params) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    while (true) {
        int button_state = get_button_state();
        int previous = 0;

        if (!button_state) {
            ESP_LOGI("GPIO", "Button pressed");
            publish_readings("estado", 1);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            previous = 1;
        }

        if (previous == 1) {
            publish_readings("estado", 0);
            previous = 0;
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void read_dht11(void* params) {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    while (true) {
        publish_readings("temperatura", get_temperature());
        publish_readings("umidade", get_humidity());

        vTaskDelay(30000 / portTICK_PERIOD_MS);
    }
}

void watch_wifi(void* params) {
    while (true) {
        if (xSemaphoreTake(wifiSemaphore, portMAX_DELAY)) {
            ESP_LOGI("MAIN", "WIFI OK, Start mqtt");
            mqtt_start();

            vTaskDelete(NULL);
        }
    }
}

void app_main(void) {
    wifiSemaphore = xSemaphoreCreateBinary();
    configSemaphore = xSemaphoreCreateBinary();

    init_nvs();
    init_gpio();
    init_wifi();

    wifi_start();

    xTaskCreate(&watch_wifi, "Monitora Wifi", 4096, NULL, 1, NULL);

    if (xSemaphoreTake(configSemaphore, portMAX_DELAY)) {
        xTaskCreate(&watch_button, "Monitora Botão", 4096, NULL, 1, NULL);
        xTaskCreate(&read_dht11, "Ler dht11", 4096, NULL, 1, NULL);
    }
}
