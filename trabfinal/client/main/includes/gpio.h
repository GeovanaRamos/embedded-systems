void init_gpio();
void change_led_status();
int get_button_state(int pin);
int get_temperature();
int get_humidity();
void disable_interrupt(int pin);
void enable_interrupt(int pin);
void init_interrupt();