#include <ncurses.h>
#define LINE_START 1
#define SCAN_LINE 10
#define SCAN_COL 30

#include "header.h"

WINDOW *logs;
WINDOW *menu;

void init_menu() {

    initscr();

    logs = newwin(9, 100, 0, 0);
    menu = newwin(11, 100, 16, 0);
}

void *display_logs(void *arg) {
    while (option != 0) {
        sleep(1);

        mvwprintw(logs, LINE_START, 0, "-----------------LOGs------------------");
        mvwprintw(logs, LINE_START + 1, 0, "Temperature:%.2f; Umidity:%.2f", readings.temperature, readings.umidity);
        mvwprintw(logs, LINE_START + 2, 0, "L1:%d; L2:%d; L3:%d; L4:%d", readings.l1, readings.l2, readings.l3, readings.l4);
        mvwprintw(logs, LINE_START + 3, 0, "AR1:%d; AR2:%d", readings.ar1, readings.ar2);
        mvwprintw(logs, LINE_START + 4, 0, "SP1:%d; SP2:%d", readings.sp1, readings.sp2);
        mvwprintw(logs, LINE_START + 5, 0, "SA1:%d; SA2:%d; SA3:%d; SA4:%d; SA5:%d; SA6:%d",
                 readings.sa1, readings.sa2, readings.sa3, readings.sa4, readings.sa5, readings.sa6);
        mvwprintw(logs, LINE_START + 6, 0, "--------------------------------------");
        wrefresh(logs);
    }

    return NULL;
}

void display_options_explanation(int line_delta) {
    mvwprintw(menu, line_delta, 0, "-> Aperte 1 para lâmpada seguido do número da lâmpada.");
    mvwprintw(menu, line_delta + 1, 0, "-> Aperte 2 para ar condicionado seguido do número do ar condicionado.");
    mvwprintw(menu, line_delta + 2, 0, "-> Aperte 3 para sensor de presença seguido do número do sensor.");
    mvwprintw(menu, line_delta + 3, 0, "-> Aperte 4 para sensor de abertura seguido do número do sensor.");
    mvwprintw(menu, line_delta + 4, 0, "-> Aperte 5 para alarme.");
    mvwprintw(menu, line_delta + 5, 0, "Exemplo: para ligar a lâmpada 4 (L4) digite 14.");

    mvwprintw(menu, SCAN_LINE, 0, "Digite: ");
    wrefresh(menu);
}

void *read_menu(void *arg) {
    int value;
    char posfix_code[3];

    while (option != 0) {
        wclear(menu);
        mvwprintw(menu, LINE_START, 0, "-----------------MENU----------------\n");
        mvwprintw(menu, LINE_START + 1, 0, "1-LIGAR um dispositivo ou alarme.\n");
        mvwprintw(menu, LINE_START + 2, 0, "2-DESLIGAR um dispositivo ou alarme.\n");
        mvwprintw(menu, LINE_START + 3, 0, "3-Escolher temperatura do ar condicionado.");
        mvwprintw(menu, LINE_START + 4, 0, "0-Sair.");
        mvwprintw(menu, LINE_START + 5, 0, "-------------------------------------\n");
        mvwprintw(menu, SCAN_LINE, 0, "Escolha a opção: ");
        wrefresh(menu);
        mvwscanw(menu, SCAN_LINE, SCAN_COL, "%d", &option);
        wclear(menu);

        switch (option) {
            case 0:
                break;

            case 1:
                mvwprintw(menu, LINE_START + 1, 0, "Escolha o dispositivo para LIGAR: ");
                display_options_explanation(LINE_START + 2);

                mvwscanw(menu, SCAN_LINE, SCAN_COL, "%d", &value);

                sprintf(posfix_code, "%d", value);
                strcpy(code, "A");
                strcat(code, posfix_code);
                break;

            case 2:
                mvwprintw(menu, LINE_START + 1, 0, "Escolha o dispositivo para DESLIGAR: ");
                display_options_explanation(LINE_START + 2);

                mvwscanw(menu, SCAN_LINE, SCAN_COL, "%d", &value);

                sprintf(posfix_code, "%d", value);
                strcpy(code, "B");
                strcat(code, posfix_code);
                break;

            case 3:
                mvwprintw(menu, SCAN_LINE, 0, "Digite a nova temperatura: ");
                wrefresh(menu);

                mvwscanw(menu, SCAN_LINE, SCAN_COL, "%d", &value);

                sprintf(posfix_code, "%d", value);
                strcpy(code, "C");
                strcat(code, posfix_code);
                break;

            default:

                break;
        }
        
    }

    return NULL;
}

void shut_down_menu() {
    endwin();
}
