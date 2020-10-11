#include <fcntl.h>  //Used for UART
#include <termios.h>  //Used for UART
#include <unistd.h>   //Used for UART
#include "header.h"

int validadeReading(int rx_length) {
    if (rx_length < 0) {
        printf("Erro na leitura.\n");
        return 0;
    } else if (rx_length == 0) {
        printf("Nenhum dado disponível.\n");
        return 0;
    }

    return 1;
}

int init_uart() {
    int uart0_filestream = -1;
    struct termios options;

    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);  

    if (uart0_filestream == -1) {
        printf("Erro - Não foi possível iniciar a UART.\n");
    } else {
        printf("UART inicializada!\n");
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
        int temperature;
        rx_length = read(*uart0_filestream, &temperature, 4);
        if (validadeReading(rx_length)) {
            printf("%d\n", temperature);
        }
    }
}
