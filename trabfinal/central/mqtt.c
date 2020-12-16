#include <MQTTClient.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cjson/cJSON.h"
#include "header.h"

MQTTClient client;

void parse_message(char* topic, char* payload) {
    cJSON* root = cJSON_Parse(payload);
    cJSON* mac = cJSON_GetObjectItemCaseSensitive(root, "mac");

    if (!cJSON_IsString(mac) || mac->valuestring == NULL)
        return;

    cJSON* value = cJSON_GetObjectItemCaseSensitive(root, "value");

    if (cJSON_IsNumber(value)) {
        struct Client* client = get_client(mac->valuestring);
        if (client==NULL)
            return;
        if (strstr(topic, "temperatura") != NULL)
            client->temperature = value->valueint;
        else if (strstr(topic, "umidade") != NULL)
            client->humidity = value->valueint;
        else if (strstr(topic, "estado")) {
            char *mode = cJSON_GetObjectItemCaseSensitive(root, "mode")->valuestring;
            if (strcmp(mode, "input") == 0)
                client->input_value = value->valueint;
            else if (strcmp(mode, "output") == 0)
                client->output_value = value->valueint;
        }
    } else {
        create_client(mac->valuestring);
    }

    cJSON_Delete(root);
}

int on_message(void* context, char* topic, int topic_len, MQTTClient_message* message) {
    char* payload = message->payload;

    parse_message(topic, payload);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topic);
    return 1;
}

void init_mqtt() {
    int rc;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&client, "tcp://broker.emqx.io", "160122180", MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, NULL, on_message, NULL);

    rc = MQTTClient_connect(client, &conn_opts);

    if (rc != MQTTCLIENT_SUCCESS) {
        printf("\nFailed to connect to MQTT broker. Error: %d\n", rc);
        exit(-1);
    }

    MQTTClient_subscribe(client, "fse2020/160122180/#", 0);
}

void publish(char* topic, char* payload) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;

    pubmsg.payload = payload;
    pubmsg.payloadlen = strlen(pubmsg.payload);
    pubmsg.qos = 2;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;
    MQTTClient_publishMessage(client, topic, &pubmsg, &token);
    MQTTClient_waitForCompletion(client, token, 1000L);
}