#include <stdio.h>

#include "esp_event.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "gpio.h"
#include "mqtt.h"
#include "nvs.h"
#include "wifi.h"

xSemaphoreHandle wifiSemaphore;
xSemaphoreHandle configSemaphore;
xQueueHandle interruptQueue;

void watch_button(void* params) {
    int pin, previous_state = 0;

    while (true) {
        if (xQueueReceive(interruptQueue, &pin, portMAX_DELAY)) {
            int state = get_button_state(pin);
            if (state == 1) {
                disable_interrupt(pin);
                while (get_button_state(pin) == state) {
                    vTaskDelay(50 / portTICK_PERIOD_MS);
                }
                previous_state = 1 - previous_state;
                printf("Button state %d\n", previous_state);
                publish_readings("input", previous_state);
                enable_interrupt(pin);
            }
        }
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
            printf("WIFI OK, Start mqtt\n");
            mqtt_start();

            vTaskDelete(NULL);
        }
    }
}

void app_main(void) {
    wifiSemaphore = xSemaphoreCreateBinary();
    configSemaphore = xSemaphoreCreateBinary();
    interruptQueue = xQueueCreate(10, sizeof(int));

    init_nvs();
    init_gpio();
    init_wifi();

    wifi_start();

    xTaskCreate(&watch_wifi, "Monitora Wifi", 4096, NULL, 1, NULL);

    if (xSemaphoreTake(configSemaphore, portMAX_DELAY)) {
        xTaskCreate(&watch_button, "Monitora Botão", 4096, NULL, 1, NULL);
        init_interrupt();
        xTaskCreate(&read_dht11, "Ler dht11", 4096, NULL, 1, NULL);
    }
}
