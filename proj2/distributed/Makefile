SOURCES.c= main.c socket.c ./cjson/cJSON.c #read_bme.c ./bme_lib/bme280.c write_gpio.c ./bcm_lib/bcm2835.c
CFLAGS=
SLIBS= -lpthread
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
