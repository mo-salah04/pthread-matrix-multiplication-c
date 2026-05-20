CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pthread
TARGET = matrix_multiplication

all: $(TARGET)

$(TARGET): matrix_multiplication.c
	$(CC) $(CFLAGS) matrix_multiplication.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) *.o
