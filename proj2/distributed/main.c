#include "header.h"
#include <pthread.h>

int valread = 1;
int valsend = 1;

int main(int argc, char const *argv[]) {

    pthread_t thread_read_command, thread_send_logs, thread_gpio_watch;

    init_gpio();
    init_socket();
    
    pthread_create(&thread_read_command, NULL, read_command, NULL);
    pthread_create(&thread_send_logs, NULL, send_logs, NULL);
    pthread_create(&thread_gpio_watch, NULL, gpio_watch, NULL);

    pthread_join(thread_read_command, NULL);
    pthread_join(thread_read_command, NULL);
    pthread_join(thread_gpio_watch, NULL);

    close_gpio();

    printf("Conexão encerrada\n");

    return 0;
}
