SOURCES.c= main.c read_bme.c ./bme_lib/bme280.c read_uart.c write_gpio.c ./bcm_lib/bcm2835.c write_lcd.c
CFLAGS=
SLIBS= -lwiringPi
PROGRAM= main
CC = gcc

OBJECTS= $(SOURCES.c:.c=.o)

.KEEP_STATE:

debug := CFLAGS= -g

all debug: $(PROGRAM)

$(PROGRAM): $(INCLUDES) $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(SLIBS)


run:
	./$(PROGRAM)

clean:
	rm -f $(PROGRAM) $(OBJECTS)
