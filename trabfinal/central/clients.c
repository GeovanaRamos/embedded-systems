#include "header.h"

void create_client(char* mac) {
    struct Client* client = NULL;
    client = (struct Client*)malloc(sizeof(struct Client));
    client->mac = malloc(sizeof(mac));
    strcpy(client->mac, mac);

    new_client = client;
}

void config_client(char* room, char* input_name, char* output_name) {
    new_client->room = malloc(sizeof(room));
    strcpy(new_client->room, room);

    new_client->input = malloc(sizeof(input_name));
    strcpy(new_client->input, input_name);

    new_client->output = malloc(sizeof(output_name));
    strcpy(new_client->output, output_name);

    if (head == NULL)
        head = new_client;
    else
        tail->next = new_client;

    tail = new_client;
    tail->next = NULL;
    new_client = NULL;
}

struct Client* get_client(char* mac) {
    struct Client* client = head;
    while (client != NULL) {
        if (strcmp(client->mac, mac) == 0)
            return client;
        client = client->next;
    }
}

void release_clients() {
    if (head != NULL) {
        struct Client* client = head;
        struct Client* next = client->next;
        while (client != NULL) {
            next = client->next;
            free(client);
            client = next;
        }
    }
}