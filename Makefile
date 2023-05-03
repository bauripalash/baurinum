CC :=clang
BUILD_DIR:=build

LIB_CFLAGS:=-Wall -g3 -std=c11 
LIB_TARGET:=baurinum
LIB_DIR:=lib
LIB_BUILDDIR=$(BUILD_DIR)/$(LIB_DIR)
LIB_SRC:=$(shell find $(LIB_DIR) -maxdepth 1 -name "*.c" )
LIB_OBJ:=$(LIB_SRC:%=$(LIB_BUILDDIR)/%.o)

LDFLAGS=-lm
TARGET:=bnmain

all: $(TARGET)
	./$(TARGET)

test: $(LIB_OBJ) tests/test.c
	$(CC) $(LIB_OBJ) tests/test.c -o testobj $(LDFLAGS);
	./testobj

$(TARGET): $(LIB_OBJ) main.c
	rm -rf $(TARGET)
	$(CC) $(LIB_OBJ) main.c -o $(TARGET) $(LDFLAGS)

lib: $(LIB_OBJ)

$(LIB_BUILDDIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(LIB_CFLAGS) -c $< -o $@

check:
	cppcheck --force --inline-suppr -I $(LIB_DIR) --enable=all $(LIB_SRC) main.c

memcheck: $(TARGET)
	valgrind -s --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

debug: $(TARGET)
	gdb ./$(TARGET)

perf_build: LIB_CFLAGS:=-O3 $(LIB_CFLAGS) -pg
perf_build: LDFLAGS+=-flto -pg
perf_build: clean lib
	$(CC) $(LIB_OBJ) main.c -o $(TARGET) $(LDFLAGS)
perf: perf_build
	perf record -g -F 999 ./$(TARGET)
	perf script -F +pid > $(TARGET).perf

prof: perf_build
	./$(TARGET)
	gprof ./$(TARGET) > baurinum.gmon.txt

fmt:
	clang-format -i -style=file lib/*.c lib/*.h main.c tests/*.c tests/*.h

clean:
	rm -rf ./testobj
	rm -rf ./$(TARGET)
	rm -rf *.o
	rm -rf *.perf
	rm -rf *.data
	rm -rf *.gmon.txt 
	rm -rf gmon.out
	rm -rf build/

