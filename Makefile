CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2
LDLIBS = -lm

TARGET = entropy
SOURCE = main.c

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET) $(LDLIBS)

test: $(TARGET)
	bash tests/test.sh

clean:
	rm -f $(TARGET) $(TARGET).exe
