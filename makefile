CC=gcc
CFLAGS=-Wall -Werror -std=c99
TARGET=platformer

all:$(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

clean:
	rm $(TARGET)CC=gcc

