#include "header.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#define PORT 2000

int sock = 0; 

int init_socket(){
    struct sockaddr_in serv_addr;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
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
    float value;
    char *code = "0x01";
    
    while(option!=0) {
        sleep(1);

        send(sock, code, strlen(code), 0);
        read(sock, &value, sizeof(float));
        readings.temperature  = value;
    }
    
    return NULL;
}