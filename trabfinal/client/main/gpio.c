#include "gpio.h"

#include <stdio.h>

#include "dht11.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "sdkconfig.h"

#define LED 2
#define BUTTON 0
#define SENSOR 4

extern xQueueHandle interruptQueue;

void IRAM_ATTR gpio_isr_handler(void *args) {
    int pin = (int)args;
    xQueueSendFromISR(interruptQueue, &pin, NULL);
}

void init_gpio() {
    gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(BUTTON);
    gpio_set_direction(BUTTON, GPIO_MODE_INPUT);
    gpio_pulldown_en(BUTTON);
    gpio_pullup_dis(BUTTON);
    gpio_set_intr_type(BUTTON, GPIO_INTR_POSEDGE);

    DHT11_init(SENSOR);
}

void init_interrupt() {
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON, gpio_isr_handler, (void *)BUTTON);
}

void change_led_status(int status) {
    gpio_set_level(LED, status);
}

int get_button_state(int pin) {
    return gpio_get_level(pin);
}

int get_temperature() {
    return DHT11_read().temperature;
}

int get_humidity() {
    return DHT11_read().humidity;
}

void disable_interrupt(int pin) {
    gpio_isr_handler_remove(pin);
}

void enable_interrupt(int pin) {
    vTaskDelay(50 / portTICK_PERIOD_MS);
    gpio_isr_handler_add(pin, gpio_isr_handler, (void *)pin);
}
