#include "header.h"

void call_alarm() {
    if (fork() == 0) {
        execl("/usr/bin/touch", "touch", "test.txt", NULL);
    }
}