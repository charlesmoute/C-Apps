SOURCE=./
CC=gcc
CFLAGS= -lcurses
CMD=application


all: exe

exe:	$(CMD).c
	$(CC) -I $(SOURCE) -o $(CMD).exe $(CMD).c $(CFLAGS)

test:	$(CMD).exe
	./$(CMD).exe

clean: 
	rm -rf $(CMD).exe