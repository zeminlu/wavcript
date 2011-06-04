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
CFLAGS = -O3 -Wall -Wuninitialized -errors -fno-builtin -m32 -g -c -o
#Sets the location of the header files.
INCLUDES = -I $(INC_DIR)
#Set the libraries variables
LIBRARIES = -lxml2 -lpthread
#Sets the C compiler set up
COMPILE.c = $(CC) $(INCLUDES) $(CFLAGS) 

#Sets the linker for the project
LD = gcc
#
LDFLAGS = $(LIBRARIES) -m32 -lcrypto -g -o
#
TARGET1 = stegowav
OBJECTS1 = main.o parser.o stegAlgorithms.o wav.o definitions.o crypto.o cutils.o

###############################################################################
.SILENT:
.PHONY: clean

all: $(TARGET1)

$(TARGET1): $(OBJECTS1)
	@echo "Linking" $@"..."
	$(LD) $(LDFLAGS) $(OUTPUT_DIR)$@ $^ 
	@echo "Done."
	
%.o: %.c
	@echo "Compiling" $< "into" $@...
	$(COMPILE.c) $@ $<
	@echo "Done."

main.o: main.c main.h

parser.o: parser.c parser.h

stegAlgorithms.o: stegAlgorithms.c stegAlgorithms.h

wav.o: wav.c wav.h
    
definitions.o: definitions.c definitions.h

crypto.o: crypto.c crypto.h

cutils.o: cutils.c cutils.h

cleanObjects:
	@echo "Clearing Objects..."
	@rm -f *.o
	@echo "Done."

clean:
	@echo "Clearing" $(OUTPUT_DIR) "directory..."
	@rm -f $(OUTPUT_DIR)/*
	@echo "Clearing Objects..."
	@rm -f *.o
	@echo "Done."
