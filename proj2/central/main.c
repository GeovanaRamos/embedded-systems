#include "header.h"
#include <pthread.h>

int option = -1;
struct sensor_readings readings = {0};
char *code;

int main() {
    code = malloc(5);
    strcpy(code, "Z00");

    pthread_t thread_display_menu, thread_read_menu, thread_socket;
    
    int socket_ok = init_socket();
    if (socket_ok < 0)
        return 0;

    init_menu();

    pthread_create(&thread_display_menu, NULL, display_logs, NULL);
    pthread_create(&thread_read_menu, NULL, read_menu, NULL);
    pthread_create(&thread_socket, NULL, get_readings, NULL);


    pthread_join(thread_display_menu, NULL);
    pthread_join(thread_read_menu, NULL);
    pthread_join(thread_socket, NULL);

    shut_down_menu();
    free(code);

    return 0;
}