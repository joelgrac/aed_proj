CC = gcc
CFLAGS = -Wall -std=c99 -O3

all: navigate

navigate: main.o file.o tasks.o
	$(CC) $(CFLAGS) -o navigate main.o file.o tasks.o

main.o: main.c file.h tasks.h
	$(CC) $(CFLAGS) -c main.c

file.o: file.c file.h tasks.h
	$(CC) $(CFLAGS) -c file.c

tasks.o: tasks.c tasks.h
	$(CC) $(CFLAGS) -c tasks.c

clean:
	rm -f *.o navigate