#include "header.h"
#include <ncurses.h>

void init_menu(){
    initscr();
}

void *display_menu(void *arg) {

    while(option!=0) {
        sleep(1);
    
        mvprintw(1, 0, "----------------LOG------------------\n");
        mvprintw(2, 0, "Temperature:%.2f; Umidity:%.2f\n", readings.temperature, readings.umidity);
        mvprintw(3, 0, "L1:%d; L2:%d; L3:%d; L4:%d\n", readings.l1, readings.l2, readings.l3, readings.l4);
        mvprintw(4, 0, "AR1:%d; AR2:%d\n", readings.ar1, readings.ar2);
        mvprintw(5, 0, "SP1:%d; SP2:%d\n", readings.sp1, readings.sp2);
        mvprintw(6, 0, "SA1:%d; SA2:%d; SA3:%d; SA4:%d; SA5:%d; SA6:%d\n", 
            readings.sa1, readings.sa2, readings.sa3, readings.sa4, readings.sa5, readings.sa6);
        mvprintw(7, 0, "-------------------------------------\n");
        mvprintw(8, 0, "----------------MENU-----------------\n");
        mvprintw(9, 0, "1-Escolher uma lâmpada para acionar ou desligar.\n");
        mvprintw(10, 0, "2-Escolher uma ar condicionado para ou desligar.\n");
        mvprintw(11, 0, "3-Escolher temperatura do ar condicionado.");
        mvprintw(12, 0, "0-Sair.");
        mvprintw(13, 0, "-------------------------------------\n");
        refresh();
    }

    return NULL;
}

void *read_menu(void *arg) {

    while(option!=0) {
        mvprintw(18, 0, "Escolha a opção: ");
        refresh();
        mvscanw(18, 20, "%d", &option);
    }

    return NULL;
}

void shut_down_menu(){
    endwin();
}