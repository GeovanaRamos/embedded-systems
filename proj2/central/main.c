#include "header.h"
#include <pthread.h>



int main() {
    int option = -1;
    struct sensor_readings empty = {0};
    readings = empty;

    pthread_t thread_display_menu, thread_read_menu, thread_socket;
    
    int socket_ok = init_socket();
    if (socket_ok < 0)
        return 0;

    init_menu();

    pthread_create(&thread_display_menu, NULL, display_menu, NULL);
    pthread_create(&thread_read_menu, NULL, read_menu, NULL);
    pthread_create(&thread_socket, NULL, get_readings, NULL);


    pthread_join(thread_display_menu, NULL);
    pthread_join(thread_read_menu, NULL);
    pthread_join(thread_socket, NULL);

    shut_down_menu();

    return 0;
}