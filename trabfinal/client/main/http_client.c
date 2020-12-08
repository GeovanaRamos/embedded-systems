#include "http_client.h"

#include <stdio.h>
#include <string.h>

#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"

#define TAG "HTTP"
#define WEATHER_KEY CONFIG_WEATHER_API_KEY
#define IP_KEY CONFIG_IP_API_KEY

char data[800];
int data_position = 0;

esp_err_t _http_event_handle(esp_http_client_event_t *evt) {
    switch (evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            memset(data, 0, strlen(data));
            data_position = 0;
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER");
            printf("%.*s", evt->data_len, (char *)evt->data);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            memcpy(data + data_position, evt->data, evt->data_len);
            data_position += evt->data_len;
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            data[data_position + 1] = '\0';
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            break;
    }
    return ESP_OK;
}

void http_request(char *url) {
    ESP_LOGI(TAG, "Url=%s", url);
    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handle,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Status = %d, content_length = %d",
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client));
    }
    esp_http_client_cleanup(client);
}

void make_ip_request() {
    char url[170];

    sprintf(url, "http://api.ipstack.com/check?access_key=%s", IP_KEY);

    http_request(url);
}

void make_weather_request(double lat, double lon) {
    char url[170];

    sprintf(url,
            "http://api.openweathermap.org/data/2.5/weather?lat=%lf&lon=%lf&units=metric&appid=%s",
            lat, lon, WEATHER_KEY);

    http_request(url);
}

char *get_data() {
    return data;
}