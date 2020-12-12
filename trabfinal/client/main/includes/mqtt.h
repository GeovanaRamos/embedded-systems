#ifndef MQTT_H
#define MQTT_H

void mqtt_start();
void mqtt_publish(char * topico, char * mensagem);
void publish_readings(char *mode, int data);

#endif
