#include <MQTTClient.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"
#include "cjson/cJSON.h"

MQTTClient client;

void parse_message(char* topic, char* payload) {
    
    if (strstr(topic, "dispositivos") != NULL) {
        cJSON *root = cJSON_Parse(payload);
        cJSON *mac = cJSON_GetObjectItemCaseSensitive(root, "mac");
        if(cJSON_IsString(mac) && mac->valuestring!=NULL)
            create_client(mac->valuestring);
    }

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

    MQTTClient_subscribe(client, "fse2020/160122180/dispositivos/#", 0);
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

// void *get_readings(){
//     char *buffer;

//     while(option!=0) {
//         buffer = malloc(1024);

//         read(sock , buffer, 1024);

//         cJSON *root = cJSON_Parse(buffer);
//         readings.temperature = cJSON_GetObjectItemCaseSensitive(root, "temperature");
//         readings.umidity = cJSON_GetObjectItemCaseSensitive(root, "umidity");
//         readings.l1 = cJSON_GetObjectItemCaseSensitive(root, "l1");
//         readings.l2 = cJSON_GetObjectItemCaseSensitive(root, "l2");
//         readings.l3 = cJSON_GetObjectItemCaseSensitive(root, "l3");
//         readings.l4 = cJSON_GetObjectItemCaseSensitive(root, "l4");
//         readings.ar1 = cJSON_GetObjectItemCaseSensitive(root, "ar1");
//         readings.ar2 = cJSON_GetObjectItemCaseSensitive(root, "ar2");
//         readings.sa1 = cJSON_GetObjectItemCaseSensitive(root, "sa1");
//         readings.sa2 = cJSON_GetObjectItemCaseSensitive(root, "sa2");
//         readings.sa3 = cJSON_GetObjectItemCaseSensitive(root, "sa3");
//         readings.sa4 = cJSON_GetObjectItemCaseSensitive(root, "sa4");
//         readings.sa5 = cJSON_GetObjectItemCaseSensitive(root, "sa5");
//         readings.sa6 = cJSON_GetObjectItemCaseSensitive(root, "sa6");
//         readings.sp1 = cJSON_GetObjectItemCaseSensitive(root, "sp1");
//         readings.sp2 = cJSON_GetObjectItemCaseSensitive(root, "sp2");

//         free(buffer);
//     }

//     return NULL;
// }
