#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

#include "./bme_lib/bme280.h"

struct identifier{
    uint8_t dev_addr;
    int8_t fd;
};

int8_t user_i2c_read(uint8_t reg_addr, uint8_t *data, uint32_t len, void *intf_ptr){
    struct identifier id;

    id = *((struct identifier *)intf_ptr);

    write(id.fd, &reg_addr, 1);
    read(id.fd, data, len);

    return 0;
}

void user_delay_us(uint32_t period, void *intf_ptr){
    usleep(period);
}

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

struct bme280_dev init_bme(){

    struct bme280_dev dev;
    struct identifier id;
    int8_t rslt = BME280_OK;

    if ((id.fd = open("/dev/i2c-1", O_RDWR)) < 0){
        fprintf(stderr, "Failed to open the i2c bus\n");
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

    return dev;
}

struct bme280_dev stream_sensor_data_forced_mode(struct bme280_dev *dev){

    int8_t rslt = BME280_OK;
    uint8_t settings_sel = 0;
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
        exit(1);
    }

    return *dev;
}

float get_bme_temperature(struct bme280_dev *dev, uint32_t req_delay){
    struct bme280_data comp_data;
    int8_t rslt = BME280_OK;

    /* Set the sensor to forced mode */
    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, dev);
    if (rslt != BME280_OK) {
        fprintf(stderr, "Failed to set sensor mode (code %+d).", rslt);
        exit(1);
    }

    /* Wait for the measurement to complete and print data */
    dev->delay_us(req_delay, dev->intf_ptr);
    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, dev);
    if (rslt != BME280_OK){
        fprintf(stderr, "Failed to get sensor data (code %+d).", rslt);
        exit(1);
    }

    return comp_data.temperature;
}

// ##########################################################
void create_csv(){
    FILE *fp;

    fp=fopen("data.csv", "w");
    
    fprintf(fp,"Time, TI, TE, TR");

    fclose(fp);
}

void add_to_csv(double ti, double te, double tr){
    FILE *fp;

    printf("Adding average to data.csv file\n");

    fp=fopen("data.csv", "a+");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(fp, ",%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(fp,"\n%0.2lf,%0.2lf,%0.2lf", ti, te, tr);

    fclose(fp);
}


int main (){
    int tr_mode; 
    double hysteresis, tr=0, ti = 0, te = 0, limit;
    
    struct bme280_dev dev;
    uint32_t req_delay;


    printf("Como voce deseja definir a temperatura de referência?\n");
    printf("1-Potenciometro\n");
    printf("2-Terminal\n");
    scanf("%d", &tr_mode);

    if (tr_mode == 2) {
        printf("Digite o valor da temperatura de referencia: ");
        scanf("%lf", &tr);
    } else {
        // tr = PEGAR DO POTENCIOMETRO VIA UART
    }

    printf("Digite o valor da histerese: ");
    scanf("%lf", &hysteresis);
    limit = hysteresis/2;

    create_csv();

    //CONFIG BME
    dev = init_bme();
    dev = stream_sensor_data_forced_mode(&dev);
    req_delay = bme280_cal_meas_delay(&dev.settings);

    while(1){
        //LER VALOR TE DO BME
        te = get_bme_temperature(&dev, req_delay);

        //LER VALOR TI DO LM35
        ti +=1;
        te +=2;
        
        // SE FORA DOS LIMITES...
        if (ti < tr - limit){
            printf("Ligando resistor\n");
        } else if (ti > tr + limit ){
            printf("Ligando ventoinha\n");
        }

        //MOSTRAR TE,TI,TR NO LCD

        //SALVAR NO CSV
        add_to_csv(ti, te, tr);
        sleep(1);
    }

    return 0;
}
