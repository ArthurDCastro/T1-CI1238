CC = gcc
CFLAGS = -Wall -O2 -Ilib

OBJ = main.o lib/input.o lib/lpwriter.o lib/utils.o

carga: $(OBJ)
	$(CC) -o carga $(OBJ)

main.o: main.c lib/structs.h lib/input.h lib/lpwriter.h
lib/input.o: lib/input.c lib/input.h lib/structs.h
lib/lpwriter.o: lib/lpwriter.c lib/lpwriter.h lib/structs.h

clean:
	rm -f *.o lib/*.o carga
