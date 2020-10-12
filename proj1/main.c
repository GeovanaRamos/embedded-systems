#include "header.h"

void create_csv() {
    FILE *fp;

    fp = fopen("data.csv", "w");

    fprintf(fp, "Time, TI, TE, TR");

    fclose(fp);
}

void add_to_csv(double ti, double te, double tr) {
    FILE *fp;

    printf("Adding average to data.csv file\n");

    fp = fopen("data.csv", "a+");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(fp, "\n%d-%02d-%02d %02d:%02d:%02d,", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(fp, "%0.2lf,%0.2lf,%0.2lf", ti, te, tr);

    fclose(fp);
}

int main() {
    int tr_mode, is_resistor_on = 0, is_fan_on = 0;
    double hysteresis, tr = 0, ti = 0, te = 0, limit;
    struct identifier id;
    
    struct bme280_dev dev = init_bme(&id);
    int uart0_filestream = init_uart();
    init_gpio();
    init_lcd();

    printf("Como voce deseja definir a temperatura de referência?\n");
    printf("1-Potenciometro\n");
    printf("2-Terminal\n");
    scanf("%d", &tr_mode);

    if (tr_mode == 2) {
        printf("Digite o valor da temperatura de referencia: ");
        scanf("%lf", &tr);
    } else {
        tr = get_uart_temperature(1, uart0_filestream);
    }

    printf("Digite o valor da histerese: ");
    scanf("%lf", &hysteresis);
    limit = hysteresis / 2;

    create_csv();

    for (int i=1; i<=3; i++) {
        sleep(1);
        
        //READ TEMPERATURES
        te = get_bme_temperature(dev);
        ti = get_uart_temperature(0, uart0_filestream);

        // RESISTOR CONTROL
        if (ti < tr - limit) {
            resistor_on();
            printf("Resistor On\n");
            is_resistor_on = 1;
        } else if (is_resistor_on){
            resistor_off();
            printf("Resistor Off\n");
            is_resistor_on = 0;
        }
        
        // FAN CONTROL
        if (ti > tr + limit) {
            fan_on();
            printf("Fan On\n");
            is_fan_on = 1;
        } else if (is_fan_on){
            fan_off();
            printf("Fan Off\n");
            is_fan_on = 0;
        }

        //DISPLAY TEMPERATURES ON LCD
        display_temperatures(ti, te, tr);

        //SAVE TO CSV
        add_to_csv(ti, te, tr);
        printf("TI=%0.2lf,TE=%0.2lf,TR=%0.2lf\n", ti, te, tr);
        
    }

    close(uart0_filestream);

    return 0;
}
