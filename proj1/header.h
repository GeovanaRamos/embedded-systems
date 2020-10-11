#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "bme_lib/bme280.h"

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
void user_delay_us(uint32_t period, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
struct bme280_dev init_bme();
double get_bme_temperature(struct bme280_dev *dev);
int validadeReading(int rx_length);
int init_uart();
double get_uart_temperature(int temp_type, int *uart0_filestream);



