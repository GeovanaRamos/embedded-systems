#include "bcm_lib/bcm2835.h"
#include "header.h"

#define RESISTOR RPI_GPIO_P1_16
#define FAN RPI_GPIO_P1_18

int init_gpio() {
    
    if (!bcm2835_init())
        return 1;

    bcm2835_gpio_fsel(RESISTOR, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(FAN, BCM2835_GPIO_FSEL_OUTP);

}

void resistor_on(){   
    bcm2835_gpio_write(RESISTOR, LOW);
}

void resistor_off(){
    bcm2835_gpio_write(RESISTOR, HIGH);
}

void fan_on(){
    bcm2835_gpio_write(FAN, LOW);
}

void fan_off(){
    bcm2835_gpio_write(FAN, HIGH);
}

void close_gpio(){
    bcm2835_close();
}