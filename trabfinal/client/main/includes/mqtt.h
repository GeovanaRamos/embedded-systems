#ifndef MQTT_H
#define MQTT_H

void mqtt_start();

void mqtt_publish(char * topico, char * mensagem);
char get_status_topic_name(char *mode);

#endif
