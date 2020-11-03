#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "bme_lib/bme280.h"

// struct identifier {
//     uint8_t dev_addr;
//     int8_t fd;
// };

// struct bme280_dev init_bme(struct identifier *id);
// double get_bme_temperature(struct bme280_dev dev);

// int init_gpio();
// void resistor_on();
// void resistor_off();
// void fan_on();
// void fan_off();
// void close_gpio();

void init_socket(); 
void *read_command(void *arg);
void *send_logs(void *arg);



