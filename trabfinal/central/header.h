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
    char *mac;
    struct Client* next; 
}; 

extern struct Client* head;
extern struct Client* tail;
extern struct Client* new_client;

void publish(char* topic, char* payload);
void init_mqtt();

void init_menu();
void *display_logs(void *arg);
void *read_menu(void *arg);
void shut_down_menu();

void create_csv();
void add_to_csv(char *code);

void create_client(char *mac);
void config_client(char *room, char *input_name, char *output_name);
struct Client* get_client(char* mac);
void release_clients();
struct Client* get_client_by_index(int index);

void change_alarm_status();
void play_alarm(int value);
char* get_alarm_string_status();

#endif  