#include "nvs.h"

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"

#define KEY "ROOM"

void init_nvs() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

int read_room_from_nvs(char **room) {

    nvs_handle partition_handle;

    size_t size;
    esp_err_t res_nvs = nvs_open("storage", NVS_READONLY, &partition_handle);
    int is_configured = 0;

    if (res_nvs == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGE("NVS", "Namespace: storage not found");
        return 0;
    } else {
        nvs_get_str(partition_handle, KEY, NULL, &size);
        *room = malloc(size);

        esp_err_t res = nvs_get_str(partition_handle, KEY, *room, &size);

        switch (res) {
            case ESP_OK:
                ESP_LOGI("NVS","String found: %s", *room);
                is_configured = 1;
                break;
            case ESP_ERR_NOT_FOUND:
                free(*room);
                ESP_LOGE("NVS", "String not found");
                break;
            default:
                ESP_LOGE("NVS", "Error accessing NVS (%s)", esp_err_to_name(res));
                free(*room);
                break;
        }

        nvs_close(partition_handle);
    }

    return is_configured;
}

void write_room_to_nvs(char *room) {
    ESP_ERROR_CHECK(nvs_flash_init());

    nvs_handle partition_handle;

    esp_err_t res_nvs = nvs_open("storage", NVS_READWRITE, &partition_handle);

    if (res_nvs == ESP_ERR_NVS_NOT_FOUND) {
        ESP_LOGE("NVS", "Namespace: storage not found");
    }
    esp_err_t res = nvs_set_str(partition_handle, KEY, room);
    if (res != ESP_OK) {
        ESP_LOGE("NVS", "Error writing to NVS (%s)", esp_err_to_name(res));
    }
    nvs_commit(partition_handle);
    nvs_close(partition_handle);
}