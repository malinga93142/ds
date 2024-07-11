# Define variables
CC = gcc  # C compiler (modify if needed)

CFLAGS = -Wall -fPIC  # Compilation flags (warning messages, position-independent code)
LDFLAGS = -shared     # Flags for linking shared library

# Define directories (modify as needed)
SRC_DIR = .           # Source directory (where .c and .cpp files are)
HDR_DIR = ../include     # Header directory (where .h files are)
LIB_DIR = /usr/lib     # Target directory for shared library
INC_DIR = /usr/include  # Target directory for headers

# Define targets
TARGET = libavl.so   # Name of the shared library (modify)

# Pattern rules for compiling source files
.PHONY: all clean install uninstall

all: $(TARGET)

$(TARGET): $(SRC_DIR)/*.c
    $(CC) $(CFLAGS) -o $(TARGET) $(SRC_DIR)/*.o -L$(LIB_DIR)

$(SRC_DIR)/*.o: $(SRC_DIR)/*.c $(HDR_DIR)/*.h  # Compile each .c file into .o object file
    $(CC) $(CFLAGS) -I$(INC_DIR) -c $<


clean:
    rm -f $(TARGET) $(SRC_DIR)/*.o

install: $(TARGET) $(HDR_DIR)/*.h
    # Requires root privileges
    cp $(TARGET) $(LIB_DIR)
    cp $(HDR_DIR)/*.h $(INC_DIR)

uninstall:
    # Requires root privileges
    rm -f $(LIB_DIR)/$(TARGET)
    rm -f $(INC_DIR)/*.h

# Phony target to avoid issues with automatic rules
.PHONY: clean install uninstall
