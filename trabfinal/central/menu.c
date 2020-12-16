#include <ncurses.h>
#define LINE_START 1
#define SCAN_LINE 14
#define SCAN_COL 30

#include "header.h"

WINDOW *logs;
WINDOW *menu;

int option = -1;

void init_menu() {
    initscr();

    logs = newwin(8, 800, 0, 0);
    menu = newwin(16, 800, 10, 0);
}

void *display_logs(void *arg) {
    sleep(1);

    while (option != 0) {
        wclear(logs);
        mvwprintw(logs, LINE_START, 0, "------------CLIENTES-------------------");

        struct Client *client = head;
        if (client == NULL && new_client == NULL)
            mvwprintw(logs, LINE_START + 2, 12, "SEM CLIENTES!!!");

        int col_position = 0;
        while (client != NULL) {
            mvwprintw(logs, LINE_START + 1, col_position, "|-----------------------");
            mvwprintw(logs, LINE_START + 2, col_position, "| %s", client->room, client);
            mvwprintw(logs, LINE_START + 3, col_position, "| %s: %d", client->input, client->input_value);
            mvwprintw(logs, LINE_START + 4, col_position, "| %s: %d", client->output, client->output_value);
            mvwprintw(logs, LINE_START + 5, col_position, "| Temp: %d", client->temperature);
            mvwprintw(logs, LINE_START + 6, col_position, "| Hum: %d", client->humidity);

            col_position += 26;
            client = client->next;
        }

        if (new_client != NULL) {
            mvwprintw(logs, LINE_START + 1, col_position, "|-----------------------");
            mvwprintw(logs, LINE_START + 2, col_position, "| NOVO CLIENTE");
            mvwprintw(logs, LINE_START + 3, col_position, "| DIGITE 100 PARA CONFIGURAR");
            mvwprintw(logs, LINE_START + 4, col_position, "| ");
            mvwprintw(logs, LINE_START + 5, col_position, "| ");
            mvwprintw(logs, LINE_START + 6, col_position, "| ");
        }

        mvwprintw(logs, LINE_START + 7, 0, "--------------------------------------");
        wrefresh(logs);

        sleep(1);
    }

    return NULL;
}

void config_client_menu() {
    wclear(menu);
    char room[50], input_name[50], output_name[50], log[50], *topic, *json;

    mvwprintw(menu, LINE_START + 2, 0, "ADICIONANDO NOVO CLIENTE");

    mvwprintw(menu, LINE_START + 2, 0, "Digite o nome do comodo: \n");
    wrefresh(menu);
    mvwscanw(menu, LINE_START + 2, 60, "%s", room);

    mvwprintw(menu, LINE_START + 3, 0, "Digite o nome do dispositivo de entrada: \n");
    wrefresh(menu);
    mvwscanw(menu, LINE_START + 3, 60, "%s", input_name);

    mvwprintw(menu, LINE_START + 4, 0, "Digite o nome do dispositivo de saida: \n");
    wrefresh(menu);
    mvwscanw(menu, LINE_START + 4, 60, "%s", output_name);

    config_client(room, input_name, output_name);

    sprintf(log, "Configurou cliente com mac %s", tail->mac);
    add_to_csv(log);

    topic = malloc(sizeof(tail->mac)+40);
    sprintf(topic, "fse2020/160122180/dispositivos/%s", tail->mac);
    json = malloc(sizeof(room)+20);
    sprintf(json, "{\"room\":\"%s\"}", room);
    publish(topic, json);
    free(topic);
    free(json);
}

void parse_menu_option(int option) {
    char code[4];

    sprintf(code, "%d", option);
    add_to_csv(code);

    if(option==0)
        return;
    else if (option!=100){
        int turn_on = option / 10;
        int client_index = option % 10;
        struct Client *client = get_client_by_index(client_index);

        if (client==NULL)
            return;
                    
        char json[30], topic[50];
        sprintf(json, "{\"value\":%d}", turn_on);
        sprintf(topic, "fse2020/160122180/dispositivos/%s", client->mac);
        publish(topic, json);
    } else {
        config_client_menu();
    }
    
}

void *read_menu(void *arg) {
    while (option != 0) {
        wclear(menu);
        mvwprintw(menu, LINE_START, 0, "-----------------MENU----------------\n");

        mvwprintw(menu, LINE_START + 1, 0, "Para ligar um dispostivo digite 1 + código do dispositivo.\n");
        mvwprintw(menu, LINE_START + 2, 0, "Para desligar um dispostivo digite 1 + código do dispositivo.\n");
        mvwprintw(menu, LINE_START + 3, 0, "Para sair digite 0.\n");
        mvwprintw(menu, LINE_START + 4, 0, "Ex: Para ligar (3)Comodo-Entrada digite 13.");

        struct Client *client = head;
        int i = 1;
        while (client != NULL) {
            mvwprintw(menu, LINE_START + 6 + i, 0, "(%d)%s-%s", i, client->room, client->output);
            i++;
            client = client->next;
        }

        mvwprintw(menu, LINE_START + 6 + i, 0, "-------------------------------------\n");
        mvwprintw(menu, SCAN_LINE, 0, "Digite o código da opção: ");
        wrefresh(menu);

        mvwscanw(menu, SCAN_LINE, SCAN_COL, "%d", &option);
        parse_menu_option(option);

        sleep(1);
    }

    return NULL;
}

void shut_down_menu() {
    endwin();
}