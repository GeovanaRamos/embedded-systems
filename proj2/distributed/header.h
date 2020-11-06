#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "bme_lib/bme280.h"

struct identifier {
    uint8_t dev_addr;
    int8_t fd;
};

struct bme280_dev init_bme(struct identifier *id);
struct bme280_data get_bme_data(struct bme280_dev dev);

int init_gpio();
void turn_on_device(int code);
void turn_off_device(int code);
void close_gpio();

void init_socket(); 
void *read_command(void *arg);
void *send_logs(void *arg);



