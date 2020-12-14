#include "header.h"
#include <pthread.h>

struct Client* head = NULL;
struct Client* tail = NULL;
struct Client* new_client = NULL;

int main() {

    pthread_t thread_display_menu, thread_read_menu;

    init_mqtt();

    //publish("fse2020/160122180/dispositivos/1234", "testando");

    create_csv();
    init_menu();

    pthread_create(&thread_display_menu, NULL, display_logs, NULL);
    pthread_create(&thread_read_menu, NULL, read_menu, NULL);

    pthread_join(thread_display_menu, NULL);
    pthread_join(thread_read_menu, NULL);

    shut_down_menu();
    release_clients();

    return 0;
}