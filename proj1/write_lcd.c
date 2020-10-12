#include <wiringPi.h>
#include <wiringPiI2C.h>

#include "header.h"

#define I2C_ADDR 0x27       // I2C device address
#define LCD_CHR 1           // Mode - Sending data
#define LCD_CMD 0           // Mode - Sending command
#define LINE1 0x80          // 1st line
#define LINE2 0xC0          // 2nd line
#define LCD_BACKLIGHT 0x08  // On
#define ENABLE 0b00000100   // Enable bit

int fd;

void lcd_toggle_enable(int bits) {
    delayMicroseconds(500);
    wiringPiI2CReadReg8(fd, (bits | ENABLE));
    delayMicroseconds(500);
    wiringPiI2CReadReg8(fd, (bits & ~ENABLE));
    delayMicroseconds(500);
}

void lcd_byte(int bits, int mode) {
    int bits_high;
    int bits_low;

    bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
    bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT;

    wiringPiI2CReadReg8(fd, bits_high);
    lcd_toggle_enable(bits_high);

    wiringPiI2CReadReg8(fd, bits_low);
    lcd_toggle_enable(bits_low);
}

void typeln(const char *s) {
    while (*s) lcd_byte(*(s++), LCD_CHR);
}

void init_lcd() {
    if (wiringPiSetup() == -1) exit(1);

    fd = wiringPiI2CSetup(I2C_ADDR);

    lcd_byte(0x33, LCD_CMD);  // Initialise
    lcd_byte(0x32, LCD_CMD);  // Initialise
    lcd_byte(0x06, LCD_CMD);  // Cursor move direction
    lcd_byte(0x0C, LCD_CMD);  // 0x0F On, Blink Off
    lcd_byte(0x28, LCD_CMD);  // Data length, number of lines, font size
    lcd_byte(0x01, LCD_CMD);  // Clear display
    delayMicroseconds(500);
}

void typeDouble(double myDouble) {
    char buffer[20];
    sprintf(buffer, "%0.2lf", myDouble);
    typeln(buffer);
}

void ClrLcd(void) {
    lcd_byte(0x01, LCD_CMD);
    lcd_byte(0x02, LCD_CMD);
}

void lcdLoc(int line) {
    lcd_byte(line, LCD_CMD);
}

void display_temperatures(double ti, double te, double tr) {
    ClrLcd(); 
    
    lcdLoc(LINE1);
    typeln("TI:");
    typeDouble(ti);
    typeln("TE:");
    typeDouble(te);

    lcdLoc(LINE2);
    typeln("TR:");
    typeDouble(tr);
}