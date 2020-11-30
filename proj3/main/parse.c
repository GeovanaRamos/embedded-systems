
#include "parse.h"
#include "cJSON.h"
#include <stdio.h>

extern double lat, lon, temp, temp_min, temp_max, hum;

void parse_ip_request(char *buffer) {
    cJSON *root = cJSON_Parse(buffer);
    lat = cJSON_GetObjectItemCaseSensitive(root, "latitude")->valuedouble;
    lon = cJSON_GetObjectItemCaseSensitive(root, "longitude")->valuedouble;
    cJSON_Delete(root);
}

void parse_weather_request(char *buffer) {
    cJSON *root = cJSON_Parse(buffer);
    cJSON *mainn = cJSON_GetObjectItemCaseSensitive(root, "main");
    temp = cJSON_GetObjectItemCaseSensitive(mainn, "temp")->valuedouble;
    temp_min = cJSON_GetObjectItemCaseSensitive(mainn, "temp_min")->valuedouble;
    temp_max = cJSON_GetObjectItemCaseSensitive(mainn, "temp_max")->valuedouble;
    hum = cJSON_GetObjectItem(mainn, "humidity")->valuedouble;
    cJSON_Delete(root);
}