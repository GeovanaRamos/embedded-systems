#include <ncurses.h>
#define LINE_START 3
#define SCAN_LINE 20
#define SCAN_COL 20

#include "header.h"

void init_menu() {
    initscr();
}

void *display_menu(void *arg) {
    while (option != 0) {
        sleep(1);

        mvprintw(LINE_START, 0, "----------------LOG------------------\n");
        mvprintw(LINE_START + 1, 0, "Temperature:%.2f; Umidity:%.2f\n", readings.temperature, readings.umidity);
        mvprintw(LINE_START + 2, 0, "L1:%d; L2:%d; L3:%d; L4:%d\n", readings.l1, readings.l2, readings.l3, readings.l4);
        mvprintw(LINE_START + 3, 0, "AR1:%d; AR2:%d\n", readings.ar1, readings.ar2);
        mvprintw(LINE_START + 4, 0, "SP1:%d; SP2:%d\n", readings.sp1, readings.sp2);
        mvprintw(LINE_START + 5, 0, "SA1:%d; SA2:%d; SA3:%d; SA4:%d; SA5:%d; SA6:%d\n",
                 readings.sa1, readings.sa2, readings.sa3, readings.sa4, readings.sa5, readings.sa6);
        mvprintw(LINE_START + 6, 0, "-------------------------------------\n");
        refresh();
        move(SCAN_LINE, SCAN_COL);
    }

    return NULL;
}

void display_options_explanation(int line_delta){
    mvprintw(LINE_START + line_delta,     0, "-> Aperte 1 para lâmpada seguido do número da lâmpada.");
    mvprintw(LINE_START + line_delta + 1, 0, "-> Aperte 2 para ar condicionado seguido do número do ar condicionado.");
    mvprintw(LINE_START + line_delta + 2, 0, "-> Aperte 3 para sensor de presença seguido do número do sensor.");
    mvprintw(LINE_START + line_delta + 3, 0, "-> Aperte 4 para sensor de abertura seguido do número do sensor.");
    mvprintw(LINE_START + line_delta + 4, 0, "-> Aperte 5 para alarme.");
    mvprintw(LINE_START + line_delta + 5, 0, "Exemplo: para ligar a lâmpada 4 (L4) digite 14.");
    mvprintw(LINE_START + line_delta + 6, 0, "-------------------------------------\n");
    mvprintw(SCAN_LINE, 0, "Digite: ");
    refresh();
}

void *read_menu(void *arg) {
    int value;
    char posfix_code[3];

    while (option != 0) {
        mvprintw(LINE_START + 7, 0, "-----------------MENU----------------\n");
        mvprintw(LINE_START + 8, 0, "1-LIGAR um dispositivo ou alarme.\n");
        mvprintw(LINE_START + 9, 0, "2-DESLIGAR um dispositivo ou alarme.\n");
        mvprintw(LINE_START + 10, 0, "3-Escolher temperatura do ar condicionado.");
        mvprintw(LINE_START + 11, 0, "0-Sair.");
        mvprintw(LINE_START + 12, 0, "-------------------------------------\n");
        mvprintw(SCAN_LINE, 0, "Escolha a opção: ");
        refresh();
        mvscanw(SCAN_LINE, SCAN_COL, "%d", &option);
        
        switch (option) {
            case 0:
                break;

            case 1:
                erase();
                mvprintw(9, 0, "Escolha o dispositivo para LIGAR: ");
                display_options_explanation(7);
                
                mvscanw(SCAN_LINE, SCAN_COL, "%d", &value);

                sprintf(posfix_code, "%d", value);
                strcpy(code, "A");
                strcat(code, posfix_code);
                break;

            case 2:
                erase();
                mvprintw(18, 0, "Escolha o dispositivo para DESLIGAR: ");
                display_options_explanation(7);

                mvscanw(SCAN_LINE, SCAN_COL, "%d", &value);

                sprintf(posfix_code, "%d", value);
                strcpy(code, "B");
                strcat(code, posfix_code);
                break;

            case 3:
                erase();
                mvprintw(18, 0, "Digite a nova temperatura do ar condicionado: ");
                refresh();

                mvscanw(SCAN_LINE, SCAN_COL, "%d", &value);
                
                sprintf(posfix_code, "%d", value);
                strcpy(code, "C");
                strcat(code, posfix_code);
                break;

            default:

                break;
        }
        erase();
    }

    return NULL;
}

void shut_down_menu() {
    endwin();
}
