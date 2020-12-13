#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct Client { 
    char *room;
    char *input;
    int input_value;
    char *output; 
    int output_value;
    int temperature;
    int humidity;
    int mac;
    struct Client* next; 
}; 

extern struct Client* head;
extern int option;

void publish(char* topic, char* payload);
void init_mqtt();

void init_menu();
void *display_logs(void *arg);
void *read_menu(void *arg);
void shut_down_menu();

void create_csv();
void add_to_csv(char *code);

#endif  