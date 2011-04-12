############################### Variables ######################################
#
#Sets working directories
INC_DIR = ./inc/
SRC_DIR = ./src/
OUTPUT_DIR = ./bin/
#
#- Global Variables
#Sets the location of the header files for make
vpath %.h $(INC_DIR)
#Set the location of the source files for make
vpath %.c $(SRC_DIR)
#
#
#- Compiler
#Sets the C compiler used to create object files from source code.
CC = gcc
#Sets compiler options.
CFLAGS = -O -Wall -Wuninitialized -pedantic -errors -fno-builtin -g -c -o
#Sets the location of the header files.
INCLUDES = -I $(INC_DIR)
#Set the libraries variables
LIBRARIES = -lxml2 -lpthread
#Sets the C compiler set up
COMPILE.c = $(CC) $(INCLUDES) $(CFLAGS) 

#Sets the linker for the project
LD = gcc
#
LDFLAGS = $(LIBRARIES) -g -o
#
TARGET1 = criptoWavs.bin
OBJECTS1 = main.o

###############################################################################
.SILENT:
.PHONY: clean

all: criptoWavs

criptoWavs = $(TARGET1)

$(TARGET1): $(OBJECTS1)
	@echo "Linking" $@"..."
	$(LD) $(LDFLAGS) $(OUTPUT_DIR)$@ $^ 
	@echo "Done."
	
%.o: %.c
	@echo "Compiling" $< "into" $@...
	$(COMPILE.c) $@ $<
	@echo "Done."

main.o: main.c main.h

cleanObjects:
	@echo "Clearing Objects..."
	@rm -f *.o
	@echo "Done."

clean:
	@echo "Clearing" $(OUTPUT_DIR) "directory..."
	@rm -f $(OUTPUT_DIR)*.bin
	@echo "Done."
