#include "header.h"
#include <time.h>

void create_csv() {
    FILE *fp;

    fp = fopen("data.csv", "w");
    
    fprintf(fp, "DataHora, Comando");

    fclose(fp);
}

void add_to_csv(char *code) {
    FILE *fp;

    fp = fopen("data.csv", "a+");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(fp, "\n%d-%02d-%02d %02d:%02d:%02d,", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(fp, "%s", code);

    fclose(fp);
}