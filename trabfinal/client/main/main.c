#include <stdio.h>

#include "esp_event.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "http_client.h"
#include "led.h"
#include "nvs_flash.h"
#include "wifi.h"
#include "parse.h"

xSemaphoreHandle wifiSemaphore;
double lat, lon, temp, temp_min, temp_max, hum;

void watch_request(void *params) {
    while (true) {
        blink_led();
        ESP_LOGI("MAIN", "Request IP");
        make_ip_request();
        parse_ip_request(get_data());
        ESP_LOGI("MAIN", "Request IP OK");

        blink_led();
        ESP_LOGI("MAIN", "Request weather");
        make_weather_request(lat, lon);
        parse_weather_request(get_data());
        ESP_LOGI("MAIN", "Request weather OK");

        printf("Temp=%.2lf Min=%.2lf Max=%.2lf Hum=%.2lf\n", temp, temp_min, temp_max, hum);

        vTaskDelay(500000 / portTICK_PERIOD_MS);
    }
}

void watch_wifi(void *params) {
    while (true) {
        if (xSemaphoreTake(wifiSemaphore, portMAX_DELAY)) {
            ESP_LOGI("MAIN", "Wifi OK - Keep led on");
            keep_led_on();

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
    init_nvs();
    init_gpio();
    init_wifi();

    wifiSemaphore = xSemaphoreCreateBinary();

    xTaskCreate(&keep_blink_led, "Piscar led", 4096, NULL, 1, NULL);
    wifi_start();
    xTaskCreate(&watch_wifi, "Monitora Wifi", 4096, NULL, 1, NULL);
    xTaskCreate(&watch_request, "Monitora Request", 4096, NULL, 1, NULL);
}
