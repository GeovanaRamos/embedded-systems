#include <ncurses.h>
#define LINE_START 1
#define SCAN_LINE 9
#define SCAN_COL 30

#include "header.h"


WINDOW *logs;
WINDOW *menu;

void init_menu() {
    initscr();

    logs = newwin(8, 150, 0, 0);
    menu = newwin(10, 150, 10, 0);
}

void *display_logs(void *arg) {
    while (option != 0) {
        sleep(1);
        mvwprintw(logs, LINE_START, 0, "-----------------LOGs------------------");
        mvwprintw(logs, LINE_START + 1, 0, "Temperature:%.2lf; Umidity:%.2lf",
                  readings.temperature->valuedouble, readings.umidity->valuedouble);
        mvwprintw(logs, LINE_START + 2, 0, "L1:%d; L2:%d; L3:%d; L4:%d",
                  readings.l1->valueint, readings.l2->valueint, readings.l3->valueint, readings.l4->valueint);
        mvwprintw(logs, LINE_START + 3, 0, "AR1:%d; AR2:%d", readings.ar1->valueint, readings.ar2->valueint);
        mvwprintw(logs, LINE_START + 4, 0, "SP1:%d; SP2:%d", readings.sp1->valueint, readings.sp2->valueint);
        mvwprintw(logs, LINE_START + 5, 0, "SA1:%d; SA2:%d; SA3:%d; SA4:%d; SA5:%d; SA6:%d",
                  readings.sa1->valueint, readings.sa2->valueint, readings.sa3->valueint, 
                  readings.sa4->valueint, readings.sa5->valueint, readings.sa6->valueint);
        mvwprintw(logs, LINE_START + 6, 0, "--------------------------------------");
        wrefresh(logs);
    }

    return NULL;
}

void display_options_explanation(int line_delta) {
    mvwprintw(menu, line_delta, 0, "-> Aperte 1 para lâmpada seguido do número da lâmpada.");
    mvwprintw(menu, line_delta + 1, 0, "-> Aperte 2 para ar condicionado seguido do número do ar condicionado.");
    mvwprintw(menu, line_delta + 2, 0, "Exemplo: para ligar a lâmpada 4 (L4) digite 14.");

    mvwprintw(menu, SCAN_LINE, 0, "Digite: ");
    wrefresh(menu);
}

void *read_menu(void *arg) {
    int value;
    char posfix_code[3];
    char *code = malloc(4);

    while (option != 0) {
        wclear(menu);
        mvwprintw(menu, LINE_START, 0, "-----------------MENU----------------\n");
        mvwprintw(menu, LINE_START + 1, 0, "1-LIGAR um dispositivo.\n");
        mvwprintw(menu, LINE_START + 2, 0, "2-DESLIGAR um dispositivo.\n");
        mvwprintw(menu, LINE_START + 3, 0, "3-Escolher temperatura do ar condicionado.");
        mvwprintw(menu, LINE_START + 4, 0, "4-Ligar ou desligar alarme.");
        mvwprintw(menu, LINE_START + 5, 0, "0-Sair.");
        mvwprintw(menu, LINE_START + 6, 0, "-------------------------------------\n");
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
                strcpy(code, "L");
                strcat(code, posfix_code);
                send_command(code);
                add_to_csv(code);
                break;

            case 2:
                mvwprintw(menu, LINE_START + 1, 0, "Escolha o dispositivo para DESLIGAR: ");
                display_options_explanation(LINE_START + 2);

                mvwscanw(menu, SCAN_LINE, SCAN_COL, "%d", &value);

                sprintf(posfix_code, "%d", value);
                strcpy(code, "D");
                strcat(code, posfix_code);
                send_command(code);
                add_to_csv(code);
                break;

            case 3:
                mvwprintw(menu, SCAN_LINE, 0, "Digite a nova temperatura: ");
                wrefresh(menu);

                mvwscanw(menu, SCAN_LINE, SCAN_COL, "%d", &value);

                sprintf(posfix_code, "%d", value);
                strcpy(code, "T");
                strcat(code, posfix_code);
                send_command(code);
                add_to_csv(code);
                break;
            
            case 4:
                mvwprintw(menu, SCAN_LINE, 0, "Digite 0 para desligar e 1 para ligar o alarme: ");
                wrefresh(menu);

                mvwscanw(menu, SCAN_LINE, SCAN_COL, "%d", &is_alarm_on);
                
            default:

                break;
        }
    }

    return NULL;
}

void shut_down_menu() {
    endwin();
}