#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "bme_lib/bme280.h"

#define L1 11
#define L2 12
#define L3 13
#define L4 14
#define AR1 21
#define AR2 22

#define SP1 31 
#define SP2 32
#define SA1 41
#define SA2 42
#define SA3 43
#define SA4 44
#define SA5 45
#define SA6 46

struct identifier {
    uint8_t dev_addr;
    int8_t fd;
};

extern int valread;
extern int valsend;

struct bme280_dev init_bme(struct identifier *id);
struct bme280_data get_bme_data(struct bme280_dev dev);

int init_gpio();
void turn_on_device(int code);
void turn_off_device(int code);
void close_gpio();
int get_device_status(int code);
void *gpio_watch(void *arg);

void init_socket(); 
void *read_command(void *arg);
void *send_logs(void *arg);



