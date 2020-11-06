#include "header.h"
#include <pthread.h>


int main(int argc, char const *argv[]) {

    pthread_t thread_read_command, thread_send_logs;

    init_gpio();
    init_socket();
    
    pthread_create(&thread_read_command, NULL, read_command, NULL);
    pthread_create(&thread_send_logs, NULL, send_logs, NULL);

    pthread_join(thread_read_command, NULL);
    pthread_join(thread_read_command, NULL);

    close_gpio();

    printf("Conexão encerrada\n");

    return 0;
}
