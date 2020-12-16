#include "header.h"

int alarm_on = 0;

void change_alarm_status(){
    alarm_on = 1 - alarm_on;
}

void play_alarm(int value){
    if (value)
        system("omxplayer --no-keys beep.mp3 > /dev/null 2>&1 & ");	
}

char* get_alarm_string_status(){
    if (alarm_on)
        return "LIGADO";
    else
        return "DESLIGADO";
}