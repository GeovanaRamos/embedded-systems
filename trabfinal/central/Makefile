SOURCES.c= main.c menu.c mqtt.c csv.c clients.c cjson/cJSON.c
CFLAGS=
SLIBS= -lncurses -lpthread -lpaho-mqtt3c
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
