CC = gcc
TARGET = asciiplay
CCFLAGS =

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) -o $(TARGET) $(CCFLAGS) $(TARGET).c