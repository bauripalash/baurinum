CC :=clang
TARGET:=baurinum

all: $(TARGET)
	@./$(TARGET)

$(TARGET): main.o
	gcc main.o -o $(TARGET)


main.o: main.c
	$(CC) -std=c11 -Wall -Wextra -c main.c -o main.o

memcheck: $(TARGET)
	valgrind ./$(TARGET)

fmt:
	clang-format -style=google -i *.c

clean:
	rm -rf *.o

