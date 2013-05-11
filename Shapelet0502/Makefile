#########################################################################
# usage:								#						
#     make TEST=y for compiling synthesis module			#
#     make TEST=n or make will compile the main code.			#
#									#
# output:								#
#     synthesis executable is ushapelet_test				#
#     main executable is ushapelet_main					#
#    									#		
#########################################################################


CC=gcc
CFLAGS=-g -pthread -lm -std=c99
DEPS=parallel_compute.h parser.h PextractUShapelets.h

CLEAN_OBJ = ushapelet synthesis_ushapelet

#EXE=ushapelet

#OBJ = parallel_compute.o PextractUShapelets.o parser.o main_parallel_synthesis.o asm_utils.c

OBJ = parallel_compute.o PextractUShapelets.o parser.o asm_utils.o

ifeq ($(TEST),y)
OBJ += main_parallel_synthesis.o
EXE=synthesis_ushapelet
else
OBJ += main.o
EXE=ushapelet
endif

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean: 
	rm -rf *.o $(CLEAN_OBJ) *.txt
