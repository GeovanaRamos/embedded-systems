#include "header.h"
#include <pthread.h>

int option = -1;
struct sensor_readings readings = {NULL};

int main() {

    pthread_t thread_display_menu, thread_read_menu, thread_socket;
    
    int socket_ok = init_socket();
    if (socket_ok < 0)
        return 0;

    pthread_create(&thread_socket, NULL, get_readings, NULL);
    
    while(readings.temperature == NULL)

    create_csv();
    init_menu();

    pthread_create(&thread_display_menu, NULL, display_logs, NULL);
    pthread_create(&thread_read_menu, NULL, read_menu, NULL);

    pthread_join(thread_display_menu, NULL);
    pthread_join(thread_read_menu, NULL);
    pthread_join(thread_socket, NULL);

    shut_down_menu();

    return 0;
}