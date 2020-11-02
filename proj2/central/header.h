#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct sensor_readings {
    float temperature;
    float umidity;
    int l1;
    int l2;
    int l3;
    int l4;
    int ar1;
    int ar2;
    int sp1;
    int sp2;
    int sa1;
    int sa2;
    int sa3;
    int sa4;
    int sa5;
    int sa6;
};

extern struct sensor_readings readings;
extern int option;
extern char *code;

int init_socket();
void *get_readings();

void init_menu();
void *display_logs(void *arg);
void *read_menu(void *arg);
void shut_down_menu();
