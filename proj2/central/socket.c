#include "header.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#define PORT 10119

int sock = 0; 

int init_socket(){
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    return 1;
}

void *get_readings(){
    char *buffer;

    while(option!=0) {
        buffer = malloc(1024);

        read(sock , buffer, 1024); 

        cJSON *root = cJSON_Parse(buffer);
        readings.temperature = cJSON_GetObjectItemCaseSensitive(root, "temperature");
        readings.umidity = cJSON_GetObjectItemCaseSensitive(root, "umidity");
        readings.l1 = cJSON_GetObjectItemCaseSensitive(root, "l1");
        readings.l2 = cJSON_GetObjectItemCaseSensitive(root, "l2");
        readings.l3 = cJSON_GetObjectItemCaseSensitive(root, "l3");
        readings.l4 = cJSON_GetObjectItemCaseSensitive(root, "l4");
        readings.ar1 = cJSON_GetObjectItemCaseSensitive(root, "ar1");
        readings.ar2 = cJSON_GetObjectItemCaseSensitive(root, "ar2");
        readings.sa1 = cJSON_GetObjectItemCaseSensitive(root, "sa1");
        readings.sa2 = cJSON_GetObjectItemCaseSensitive(root, "sa2");
        readings.sa3 = cJSON_GetObjectItemCaseSensitive(root, "sa3");
        readings.sa4 = cJSON_GetObjectItemCaseSensitive(root, "sa4");
        readings.sa5 = cJSON_GetObjectItemCaseSensitive(root, "sa5");
        readings.sa6 = cJSON_GetObjectItemCaseSensitive(root, "sa6");
        readings.sp1 = cJSON_GetObjectItemCaseSensitive(root, "sp1");
        readings.sp2 = cJSON_GetObjectItemCaseSensitive(root, "sp2");

        free(buffer);
    }

    
    return NULL;
}

void send_command(char *code){
    send(sock, code, strlen(code), 0);
}