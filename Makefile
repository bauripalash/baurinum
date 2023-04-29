CC :=clang
TARGET:=baurinum
LDFLAGS=-lm

all: $(TARGET)
	@./$(TARGET)

$(TARGET): main.o
	$(CC) main.o -o $(TARGET) $(LDFLAGS)


main.o: main.c
	$(CC) -std=c11 -Wall -Wextra -c main.c -o main.o

memcheck: $(TARGET)
	valgrind ./$(TARGET)

fmt:
	clang-format -style=google -i *.c

clean:
	rm -rf *.o

