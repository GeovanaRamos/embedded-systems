#include "bcm_lib/bcm2835.h"
#include "header.h"

#define L1 RPI_GPIO_P1_11   //17
#define L2 RPI_GPIO_P1_12   //18
#define L3 RPI_GPIO_P1_13   //27
#define L4 RPI_GPIO_P1_15   //22
#define AR1 RPI_GPIO_P1_16  //23
#define AR2 RPI_GPIO_P1_18  //24

struct status {
    int l1;
    int l2;
    int l3;
    int l4;
    int ar1;
    int ar2;
};

struct status pins = {0};

int init_gpio() {
    if (!bcm2835_init()) {
        printf("Erro gpio");
        exit(1);
    }

    bcm2835_gpio_fsel(L1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(L2, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(L3, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(L4, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(AR1, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(AR2, BCM2835_GPIO_FSEL_OUTP);

}

void change_status(int code, char *mode, uint8_t on) {
    switch (code) {
        case 11:
            printf("%s Lampada 1\n", mode);
            bcm2835_gpio_write(L1, on);
            pins.l1 = (on == HIGH) ? 0 : 1;
            break;
        case 12:
            printf("%s Lampada 2\n", mode);
            bcm2835_gpio_write(L2, on);
            pins.l2 = (on == HIGH) ? 0 : 1;
            break;
        case 13:
            printf("%s Lampada 3\n", mode);
            bcm2835_gpio_write(L3, on);
            pins.l3 = (on == HIGH) ? 0 : 1;
            break;
        case 14:
            printf("%s Lampada 4\n", mode);
            bcm2835_gpio_write(L4, on);
            pins.l4 = (on == HIGH) ? 0 : 1;
            break;
        case 21:
            printf("%s Ar 1\n", mode);
            bcm2835_gpio_write(AR1, on);
            pins.ar1 = (on == HIGH) ? 0 : 1;
            break;
        case 22:
            printf("%s Ar 2\n", mode);
            bcm2835_gpio_write(AR2, on);
            pins.ar2 = (on == HIGH) ? 0 : 1;
            break;
        default:
            break;
    }
}

void turn_on_device(int code) {
    change_status(code, "Ligando", LOW);
}

void turn_off_device(int code) {
    change_status(code, "Desligando", HIGH);
}

void close_gpio() {
    turn_off_device(11);
    turn_off_device(12);
    turn_off_device(13);
    turn_off_device(14);
    turn_off_device(21);
    turn_off_device(22);
    bcm2835_close();
}

int get_device_status(int code) {
    switch (code) {
        case 11:
            return pins.l1;
            break;
        case 12:
            return pins.l2;
            break;
        case 13:
            return pins.l3;
            break;
        case 14:
            return pins.l4;
            break;
        case 21:
            return pins.ar1;
            break;
        case 22:
            return pins.ar2;
            break;
        default:
            break;
    }
}