#include "gpio.h"

#include <stdio.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "esp_log.h"

#define LED 2
#define BOTAO 0

int keep_on = 0;

void init_gpio() {
    gpio_pad_select_gpio(LED);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);

    // Configuração do pino do Botão
    gpio_pad_select_gpio(BOTAO);
    // Configura o pino do Botão como Entrada
    gpio_set_direction(BOTAO, GPIO_MODE_INPUT);
    // Configura o resistor de Pulldown para o botão (por padrão a entrada estará em Zero)
    gpio_pulldown_en(BOTAO);
    // Desabilita o resistor de Pull-up por segurança.
    gpio_pullup_dis(BOTAO);
}

void watch_button(void* params) {
    // Testa o Botão utilizando polling
    while (true) {
        int estado_botao = gpio_get_level(BOTAO);
        if (!estado_botao){
            ESP_LOGI("GPIO", "Apertou botão");
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void keep_led_on() {
    keep_on = 1;
}

void blink_led() {
    gpio_set_level(LED, 0);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    gpio_set_level(LED, 1);
}