#include "bcm_lib/bcm2835.h"
#include "header.h"

#define L1_PIN RPI_V2_GPIO_P1_11   //17
#define L2_PIN RPI_V2_GPIO_P1_12   //18
#define L3_PIN RPI_V2_GPIO_P1_13   //27
#define L4_PIN RPI_V2_GPIO_P1_15   //22
#define AR1_PIN RPI_V2_GPIO_P1_16  //23
#define AR2_PIN RPI_V2_GPIO_P1_18  //24

#define SP1_PIN RPI_V2_GPIO_P1_22  //25
#define SP2_PIN RPI_V2_GPIO_P1_37  //26
#define SA1_PIN RPI_V2_GPIO_P1_29  //5
#define SA2_PIN RPI_V2_GPIO_P1_31  //06
#define SA3_PIN RPI_V2_GPIO_P1_32  //12
#define SA4_PIN RPI_V2_GPIO_P1_36  //16
#define SA5_PIN RPI_V2_GPIO_P1_38  //20
#define SA6_PIN RPI_V2_GPIO_P1_40  //21

struct status {
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

struct status pins = {0};

int init_gpio() {
    if (!bcm2835_init()) {
        printf("Erro gpio");
        exit(1);
    }

    bcm2835_gpio_fsel(L1_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(L2_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(L3_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(L4_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(AR1_PIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(AR2_PIN, BCM2835_GPIO_FSEL_OUTP);
}

void change_status(int code, char *mode, uint8_t on) {
    switch (code) {
        case L1:
            printf("%s Lampada 1\n", mode);
            bcm2835_gpio_write(L1_PIN, on);
            pins.l1 = (on == HIGH) ? 0 : 1;
            break;
        case L2:
            printf("%s Lampada 2\n", mode);
            bcm2835_gpio_write(L2_PIN, on);
            pins.l2 = (on == HIGH) ? 0 : 1;
            break;
        case L3:
            printf("%s Lampada 3\n", mode);
            bcm2835_gpio_write(L3_PIN, on);
            pins.l3 = (on == HIGH) ? 0 : 1;
            break;
        case L4:
            printf("%s Lampada 4\n", mode);
            bcm2835_gpio_write(L4_PIN, on);
            pins.l4 = (on == HIGH) ? 0 : 1;
            break;
        case AR1:
            printf("%s Ar 1\n", mode);
            bcm2835_gpio_write(AR1_PIN, on);
            pins.ar1 = (on == HIGH) ? 0 : 1;
            break;
        case AR2:
            printf("%s Ar 2\n", mode);
            bcm2835_gpio_write(AR2_PIN, on);
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
        case L1:
            return pins.l1;
            break;
        case L2:
            return pins.l2;
            break;
        case L3:
            return pins.l3;
            break;
        case L4:
            return pins.l4;
            break;
        case AR1:
            return pins.ar1;
            break;
        case AR2:
            return pins.ar2;
            break;
        case SA1:
            return pins.sa1;
            break;
        case SA2:
            return pins.sa2;
            break;
        case SA3:
            return pins.sa3;
            break;
        case SA4:
            return pins.sa4;
            break;
        case SA5:
            return pins.sa5;
            break;
        case SA6:
            return pins.sa6;
            break;
        default:
            break;
    }
}

void *gpio_watch(void *arg) {
    while (valread != 0 && valsend != 0 && valread != -1 && valsend != -1) {
        pins.sp1 = bcm2835_gpio_lev(SP1_PIN);
        pins.sp2 = bcm2835_gpio_lev(SP2_PIN);
        pins.sa1 = bcm2835_gpio_lev(SA1_PIN);
        pins.sa2 = bcm2835_gpio_lev(SA2_PIN);
        pins.sa3 = bcm2835_gpio_lev(SA3_PIN);
        pins.sa4 = bcm2835_gpio_lev(SA4_PIN);
        pins.sa5 = bcm2835_gpio_lev(SA5_PIN);
        pins.sa6 = bcm2835_gpio_lev(SA6_PIN);
    }
}