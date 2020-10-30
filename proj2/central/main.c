#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PORT 2000

int option=2;

int init_socket(){
    int sock = 0;
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

    return sock;
}

void *display_menu(void *arg) {
    int i = 0;
    while(option!=0) {
        sleep(1);
    
        mvprintw(1, 0, "----------------LOG------------------\n");
        mvprintw(2, 0, "L1:ON; L2:OFF; L3:ON; L4:%d\n", i);
        mvprintw(3, 0, "AR1:ON; AR2:%d\n", option);
        mvprintw(4, 0, "SP1:ON; SP2:%d\n", option);
        mvprintw(5, 0, "SA1:ON; SA2:OFF; SA3:ON; SA4:%d; SA5:OFF; SA6:ON\n", option);
        mvprintw(6, 0, "-------------------------------------\n");
        mvprintw(7, 0, "----------------MENU-----------------\n");
        mvprintw(8, 0, "1-Escolher uma lâmpada para acionar ou desligar.\n");
        mvprintw(9, 0, "2-Escolher uma ar condicionado para ou desligar.\n");
        mvprintw(10, 0, "3-Escolher temperatura do ar condicionado.");
        mvprintw(11, 0, "0-Sair.");
        mvprintw(12, 0, "-------------------------------------\n");
        refresh();
        i++;
    }

    return NULL;
}

void *read_menu(void *arg) {
    int i = 0;
    while(option!=0) {
        mvprintw(18, 0, "Escolha a opção: ");
        refresh();
        mvscanw(18, 20, "%d", &option);
        i++;
    }

    return NULL;
}



int main() {
    pthread_t thread_display_menu, thread_read_menu;

    initscr();

    pthread_create(&thread_display_menu, NULL, display_menu, NULL);
    pthread_create(&thread_read_menu, NULL, read_menu, NULL);

    sleep(3);
    int sock = init_socket();
    int valread;
    char *hello = "Want Temperature";
    char buffer[1024] = {0};
    
    send(sock, hello, strlen(hello), 0);
    valread = read(sock, buffer, 1024);
    mvprintw(22, 0, "%s-----------------\n", buffer);
    refresh();

    pthread_join(thread_display_menu, NULL);
    pthread_join(thread_read_menu, NULL);

    endwin();

    return 0;
}

//gcc main.c -lncurses -lpthread