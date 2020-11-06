#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>

#include "cjson/cJSON.h"
#define PORT 10119
#include "header.h"

int new_socket = 0;

void init_socket() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
}

void *read_command(void *arg) {
    char buffer[1024] = {0};
    int valread;

    while (1) {
        valread = read(new_socket, buffer, 1024);

        if (valread == 0)
            break;

        int code = atoi(buffer + 1);

        if (strstr(buffer, "L") != NULL) {
            turn_on_device(code);
        } else if (strstr(buffer, "D") != NULL) {
            turn_on_device(code);
        } else {
            printf("Mudando a temperatura para %d\n", code);
        } 
    }
}

void *send_logs(void *arg) {

    struct identifier id;
    struct bme280_dev dev = init_bme(&id);
    struct bme280_data comp_data;
    int response = 1;

    while (response != 0 && response != -1) {
        sleep(1);

        comp_data = get_bme_data(dev);

        cJSON *readings = cJSON_CreateObject();
        cJSON_AddItemToObject(readings, "temperature", cJSON_CreateNumber(comp_data.temperature));
        cJSON_AddItemToObject(readings, "umidity", cJSON_CreateNumber(comp_data.humidity));
        cJSON_AddItemToObject(readings, "l1", cJSON_CreateNumber(1));
        cJSON_AddItemToObject(readings, "l2", cJSON_CreateNumber(0));
        cJSON_AddItemToObject(readings, "l3", cJSON_CreateNumber(1));
        cJSON_AddItemToObject(readings, "l4", cJSON_CreateNumber(0));
        cJSON_AddItemToObject(readings, "ar1", cJSON_CreateNumber(1));
        cJSON_AddItemToObject(readings, "ar2", cJSON_CreateNumber(0));
        cJSON_AddItemToObject(readings, "sa1", cJSON_CreateNumber(1));
        cJSON_AddItemToObject(readings, "sa2", cJSON_CreateNumber(0));
        cJSON_AddItemToObject(readings, "sa3", cJSON_CreateNumber(1));
        cJSON_AddItemToObject(readings, "sa4", cJSON_CreateNumber(1));
        cJSON_AddItemToObject(readings, "sa5", cJSON_CreateNumber(1));
        cJSON_AddItemToObject(readings, "sa6", cJSON_CreateNumber(0));
        cJSON_AddItemToObject(readings, "sp1", cJSON_CreateNumber(1));
        cJSON_AddItemToObject(readings, "sp2", cJSON_CreateNumber(0));
        char *string = cJSON_Print(readings);

        send(new_socket, string, strlen(string), 0);
        printf("Logs enviados\n");
    }

}
