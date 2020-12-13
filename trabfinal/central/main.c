#include "header.h"
#include <pthread.h>

int option = -1;
struct Client* head = NULL;

int main() {

    pthread_t thread_display_menu, thread_read_menu, thread_socket;

    char *rooms[] = {
        "Sala",
        "Quarto",
        "Banheiro",
        "Cozinha",
        "Escritorio"
    };

    struct Client* previous = NULL; 

    for (int i=0; i<5; i++){
 
        struct Client* client = NULL; 
        client = (struct Client*)malloc(sizeof(struct Client));

        client->room = rooms[i];
        client->input = "lampada";
        client->input_value = 1;
        client->output = "sensor de presenca";
        client->output_value = 0;
        client->mac = 123123123;
        client->temperature = 30;
        client->humidity = 60;
        client->next = NULL;

        if(i!=0)
            previous->next = client;
        else
            head = client;
        
        previous = client;
    }
    
    init_mqtt();

    //pthread_create(&thread_socket, NULL, get_readings, NULL);
    publish("fse2020/160122180/dispositivos", "testando");

    create_csv();
    init_menu();

    pthread_create(&thread_display_menu, NULL, display_logs, NULL);
    pthread_create(&thread_read_menu, NULL, read_menu, NULL);

    pthread_join(thread_display_menu, NULL);
    pthread_join(thread_read_menu, NULL);
    //pthread_join(thread_socket, NULL);

    shut_down_menu();

    return 0;
}