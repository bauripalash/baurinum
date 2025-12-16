CC ?= clang
BIN:=baurinum
SOURCE?=sample.c baurinum.c
HEADERS?=baurinum.h

# Build Directory of CMake
CMAKE_BUILD_DIR ?= build

# CMake built binary
CMAKE_OUTPUT ?= $(CMAKE_BUILD_DIR)/$(BIN)

all: run

.PHONY: build
build:
	cmake --build build --target $(BIN)

.PHONY: run
run: build
	./$(CMAKE_OUTPUT) $(FILE)

.PHONY: fmt
fmt:
	@clang-format -i -style=file --verbose $(SOURCES) $(HEADERS)

.PHONY: valgrind
valgrind: build
	valgrind --leak-check=full --show-leak-kinds=all -s $(CMAKE_OUTPUT)

.PHONY: cmake_tcc
cmake_tcc:
	cmake -S . -B build -DCMAKE_C_COMPILER=$(TCC)

.PHONY: cmake_setup
cmake_setup:
	cmake -S . -B build -DCMAKE_C_COMPILER=clang

.PHONY: cmake_clean
cmake_clean:
	cd build && make clean

.PHONY: build_dbg
build_dbg:
	rm -rf build
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
	cmake --build build --target $(BIN)

.PHONY: build_rls
build_rls:
	rm -rf build
	cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=$(RELEASE_CC)
	cmake --build build --target $(BIN)

.PHONY: build_rld
build_rld:
	rm -rf build
	cmake -S . -B build -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_C_COMPILER=$(RELEASE_CC)
	cmake --build build --target $(BIN)

.PHONY: infer
infer: cmake_clean
	infer run --compilation-database build/compile_commands.json

.PHONY: run_clangtidy
run_clangtidy:
	clang-tidy --config-file=.clang-tidy $(SOURCES) $(HEADERS)

.PHONY: clean
clean: cmake_clean
	rm -rf .zig-cache
	rm -rf .cache
	rm -rf zig-out
