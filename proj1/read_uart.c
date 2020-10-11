#include <fcntl.h>  //Used for UART
#include <termios.h>  //Used for UART
#include <unistd.h>   //Used for UART
#include "header.h"

void validadeReading(int rx_length) {
    if (rx_length < 0) {
        printf("Erro na leitura.\n");
        exit(1);
    } else if (rx_length == 0) {
        printf("Nenhum dado disponível.\n");
        exit(1);
    }
}

int init_uart() {
    int uart0_filestream = -1;
    struct termios options;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);  

    if (uart0_filestream == -1) {
        printf("UART error while opening.\n");
        exit(1);
    }
    
    tcgetattr(uart0_filestream, &options);

    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD; 
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    return uart0_filestream;
}

double get_uart_temperature(int temp_type, int *uart0_filestream) {
    unsigned char b[100];
    unsigned char *a;
    a = &b[0];

    if (!temp_type) {
        *a++ = 0xA1;

    } else {
        *a++ = 0xA2;
    }

    *a++ = 2;
    *a++ = 1;
    *a++ = 8;
    *a++ = 0;

    if (*uart0_filestream != -1) {
        int rx_length;
        float temperature;
        rx_length = read(*uart0_filestream, &temperature, 4);
        validadeReading(rx_length);
        return temperature;
    }

    return 0;
}
