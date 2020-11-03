#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "cjson/cJSON.h"

struct sensor_readings {
    cJSON *temperature;
    cJSON *umidity;
    cJSON *l1;
    cJSON *l2;
    cJSON *l3;
    cJSON *l4;
    cJSON *ar1;
    cJSON *ar2;
    cJSON *sp1;
    cJSON *sp2;
    cJSON *sa1;
    cJSON *sa2;
    cJSON *sa3;
    cJSON *sa4;
    cJSON *sa5;
    cJSON *sa6;
};

extern struct sensor_readings readings;
extern int option;

int init_socket();
void *get_readings();

void init_menu();
void *display_logs(void *arg);
void *read_menu(void *arg);
void shut_down_menu();
