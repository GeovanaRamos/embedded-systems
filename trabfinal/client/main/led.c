#include "led.h"

#include <stdio.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#define LED 2

int keep_on = 0;

void init_gpio() {
    gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
}

void keep_blink_led(void* params) {
    int estado = 0;
    while (!keep_on) {
        gpio_set_level(LED, estado);
        vTaskDelay(400 / portTICK_PERIOD_MS);
        estado = !estado;
    }
    gpio_set_level(LED, 1);
    
    vTaskDelete(NULL);
}

void keep_led_on() {
    keep_on = 1;
}

void blink_led() {
    gpio_set_level(LED, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    gpio_set_level(LED, 1);
}