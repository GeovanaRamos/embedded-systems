SOURCES.c= main.c menu.c socket.c ./cjson/cJSON.c csv.c
CFLAGS=
SLIBS= -lncurses -lpthread
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
