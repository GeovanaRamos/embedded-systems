#include "gpio.h"

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"
#include "dht11.h"

#define LED 2
#define BUTTON 0
#define SENSOR 4


void init_gpio() {
    gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(BUTTON);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
    gpio_pulldown_en(BUTTON);
    gpio_pullup_dis(BUTTON);

    DHT11_init(SENSOR);
}

void keep_led_on() {
    gpio_set_level(LED, 1);
}

void keep_led_off() {
    gpio_set_level(LED, 0);
}

int get_button_state() {
    return gpio_get_level(BUTTON);
}

int get_temperature() {
    return DHT11_read().temperature;
}

int get_humidity() {
    return DHT11_read().humidity;
}

