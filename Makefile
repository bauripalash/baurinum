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
	@./$(TARGET)

$(TARGET): lib
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

fmt:
	clang-format -i -style=file lib/*.c lib/*.h main.c

clean:
	rm -rf *.o
	rm -rf build/

