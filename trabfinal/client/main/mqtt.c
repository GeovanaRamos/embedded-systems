#include "mqtt.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "cJSON.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "mqtt_client.h"

#define TAG "MQTT"

extern xSemaphoreHandle configSemaphore;

esp_mqtt_client_handle_t client;
int msg_id;
int is_configured = 0; 
char *room;

void esp_init_config() {
    ESP_LOGI(TAG, "Init esp as topic's client");

    char topic[50];
    sprintf(topic, "fse2020/160122180/dispositivos/%s", "1234");
    
    mqtt_publish(topic, "{\"command\":\"init\"}");
    msg_id = esp_mqtt_client_subscribe(client, topic, 0);
}

void save_room_name(char *json) {
    ESP_LOGI(TAG, "Save room name");
    
    // "{\"room\":\"sala\"}"
    cJSON *root = cJSON_Parse(json);
    char *json_room = cJSON_GetObjectItemCaseSensitive(root, "room")->valuestring;
    room = malloc(sizeof(json_room));
    strcpy(room, json_room);
    cJSON_Delete(root);

    is_configured = 1;
    xSemaphoreGive(configSemaphore);
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event) {
    esp_mqtt_client_handle_t client = event->client;

    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            if (!is_configured) {
                esp_init_config();
            }
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            if (is_configured)
                ESP_LOGI(TAG, "LIgar led");
            else
                save_room_name(event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void mqtt_start() {
    esp_mqtt_client_config_t mqtt_config = {
       // .uri = "mqtt://192.168.15.9:1883",
        .uri="mqtt://broker.emqx.io"
        //.uri="mqtt://test.mosquitto.org"
        //.uri = "mqtt://mqtt.eclipse.org",
    };
    client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void mqtt_publish(char *topic, char *message) {
    int message_id = esp_mqtt_client_publish(client, topic, message, 0, 1, 0);
    ESP_LOGI(TAG, "Mensagem enviada, ID: %d, T: %s", message_id, topic);
}

void publish_readings(char *mode, int data){
    char topic[50];
    char json[50];

    printf("%s", room);
    
    sprintf(topic, "fse2020/160122180/%s/%s", room, mode);
    sprintf(json, "{\"%s\":%d}", mode, data);
    
    mqtt_publish(topic, json);
}
