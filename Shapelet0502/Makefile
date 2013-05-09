CC=gcc
CFLAGS=-g -pthread -lm -std=c99
DEPS=parallel_compute.h parser.h PextractUShapelets.h
EXE=ushapelet

OBJ = parallel_compute.o PextractUShapelets.o parser.o main_parallel_synthesis.o asm_utils.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean: 
	rm -rf *.o $(EXE) *.txt
