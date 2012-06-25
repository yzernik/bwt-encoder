CC=gcc 
CFLAGS=-g -Wall
OBJECTS=encode.o bwt.o ks.o
build: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o encode

clean:
	rm -f encode *.o *~