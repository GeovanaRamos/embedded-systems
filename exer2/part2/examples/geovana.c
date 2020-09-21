#include <linux/i2c-dev.h>
#include <sys/ioctl.h>


/******************************************************************************/
/*!                         System header files                               */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

/******************************************************************************/
/*!                         Own header files                                  */
#include "bme280.h"

/******************************************************************************/
/*!                               Structures                                  */

/* Structure that contains identifier details used in example */
struct identifier{
    uint8_t dev_addr;
    int8_t fd;
};

/****************************************************************************/
/*!                         Functions                                       */

void user_delay_us(uint32_t period, void *intf_ptr);
void print_sensor_data(struct bme280_data *comp_data);
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr);
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr);
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev);
void create_csv(double sum_hum, double sum_pres, double sum_temp);

/*!
 * @brief This function starts execution of the program.
 */
int main(int argc, char* argv[]){

    struct bme280_dev dev;
    struct identifier id;
    int8_t rslt = BME280_OK;

    if (argc < 2){
        fprintf(stderr, "Missing argument for i2c bus.\n");
        exit(1);
    }

    if ((id.fd = open(argv[1], O_RDWR)) < 0){
        fprintf(stderr, "Failed to open the i2c bus %s\n", argv[1]);
        exit(1);
    }

    /* Make sure to select BME280_I2C_ADDR_PRIM or BME280_I2C_ADDR_SEC as needed */
    id.dev_addr = BME280_I2C_ADDR_PRIM;

    if (ioctl(id.fd, I2C_SLAVE, id.dev_addr) < 0){
        fprintf(stderr, "Failed to acquire bus access and/or talk to slave.\n");
        exit(1);
    }

    dev.intf = BME280_I2C_INTF;
    dev.read = user_i2c_read;
    dev.write = user_i2c_write;
    dev.delay_us = user_delay_us;

    /* Update interface pointer with the structure that contains both device address and file descriptor */
    dev.intf_ptr = &id;

    /* Initialize the bme280 */
    rslt = bme280_init(&dev);
    if (rslt != BME280_OK){
        fprintf(stderr, "Failed to initialize the device (code %+d).\n", rslt);
        exit(1);
    }

    rslt = stream_sensor_data_forced_mode(&dev);
    if (rslt != BME280_OK){
        fprintf(stderr, "Failed to stream sensor data (code %+d).\n", rslt);
        exit(1);
    }

    return 0;
}

/*!
 * @brief This function reading the sensor's registers through I2C bus.
 */
int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr){
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    write(id.fd, &reg_addr, 1);
    read(id.fd, data, len);

    return 0;
}

/*!
 * @brief This function provides the delay for required time (Microseconds) as per the input provided in some of the
 * APIs
 */
void user_delay_us(uint32_t period, void *intf_ptr){
    usleep(period);
}

/*!
 * @brief This function for writing the sensor's registers through I2C bus.
 */
int8_t user_i2c_write(uint8_t reg_addr, const uint8_t *data, uint32_t len, void *intf_ptr){
    uint8_t *buf;
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    buf = malloc(len + 1);
    buf[0] = reg_addr;
    memcpy(buf + 1, data, len);
    if (write(id.fd, buf, len + 1) < (uint16_t)len)
    {
        return BME280_E_COMM_FAIL;
    }

    free(buf);

    return BME280_OK;
}

/*!
 * @brief This API used to print the sensor temperature, pressure and humidity data.
 */
void print_sensor_data(struct bme280_data *comp_data){
    float temp, press, hum;

    #ifdef BME280_FLOAT_ENABLE
        temp = comp_data->temperature;
        press = 0.01 * comp_data->pressure;
        hum = comp_data->humidity;
    #else
    #ifdef BME280_64BIT_ENABLE
        temp = 0.01f * comp_data->temperature;
        press = 0.0001f * comp_data->pressure;
        hum = 1.0f / 1024.0f * comp_data->humidity;
    #else
        temp = 0.01f * comp_data->temperature;
        press = 0.01f * comp_data->pressure;
        hum = 1.0f / 1024.0f * comp_data->humidity;
    #endif
    #endif
    printf("%0.2lf deg C, %0.2lf hPa, %0.2lf%%\n", temp, press, hum);
}

/*!
 * @brief This API reads the sensor temperature, pressure and humidity data in forced mode.
 */
int8_t stream_sensor_data_forced_mode(struct bme280_dev *dev){

    int8_t rslt = BME280_OK;
    uint8_t settings_sel = 0;
    struct bme280_data comp_data;
    uint32_t req_delay;

    double sum_temp = 0, sum_pres = 0, sum_hum = 0;

    /* Recommended mode of operation: Indoor navigation */
    dev->settings.osr_h = BME280_OVERSAMPLING_1X;
    dev->settings.osr_p = BME280_OVERSAMPLING_16X;
    dev->settings.osr_t = BME280_OVERSAMPLING_2X;
    dev->settings.filter = BME280_FILTER_COEFF_16;

    settings_sel = BME280_OSR_PRESS_SEL | BME280_OSR_TEMP_SEL | BME280_OSR_HUM_SEL | BME280_FILTER_SEL;

    /* Set the sensor settings */
    rslt = bme280_set_sensor_settings(settings_sel, dev);
    if (rslt != BME280_OK){
        fprintf(stderr, "Failed to set sensor settings (code %+d).", rslt);

        return rslt;
    }

    printf("Temperature, Pressure, Humidity\n");

    req_delay = bme280_cal_meas_delay(&dev->settings);

    /* Continuously stream sensor data */
    for (int i=1; i<=100; i++) {
        sleep(1);
        
        /* Set the sensor to forced mode */
        rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
        if (rslt != BME280_OK) {
            fprintf(stderr, "Failed to set sensor mode (code %+d).", rslt);
            break;
        }

        /* Wait for the measurement to complete and print data */
        dev->delay_us(req_delay, dev->intf_ptr);
        rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
        if (rslt != BME280_OK){
            fprintf(stderr, "Failed to get sensor data (code %+d).", rslt);
            break;
        }
        
        print_sensor_data(&comp_data);
        
        sum_hum += comp_data.humidity;
        sum_pres += comp_data.pressure;
        sum_temp += comp_data.temperature;
        
        if (i%10 == 0){
            create_csv(sum_hum, sum_pres, sum_temp);
            sum_hum = 0;
            sum_pres = 0;
            sum_temp = 0;
        }

    }

    return rslt;
}

void create_csv(double sum_hum, double sum_pres, double sum_temp){
    FILE *fp;

    printf("Creating %s.csv file\n", "data.csv");

    fp=fopen("data.csv", "w");

    fprintf(fp,"Temperature,Humidity,Pressure");
    fprintf(fp,"\n%0.2lf,%0.2lf,%0.2lf", sum_temp/(double)10, sum_hum/(double)10, sum_pres/(double)100);
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(",%d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    fclose(fp);
}
