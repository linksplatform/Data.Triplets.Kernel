CC=gcc
CXX=g++
OPTS=-O3 --std=c99 -fPIC -W -Wall -Wextra -pedantic
CXXOPTS=-O3 --std=c++14 -fPIC -W -Wall -Wextra -pedantic
DEFS=-DLINKS_DLL_EXPORT -D_XOPEN_SOURCE=700

# Default target: build library and simple test
all: Platform.Data.Triplets.Kernel test

# Build the main library
Platform.Data.Triplets.Kernel: Link.h Link.c
	${CC} ${OPTS} ${DEFS} -c -o Timestamp.o Timestamp.c
	${CC} ${OPTS} ${DEFS} -c -o Link.o Link.c
	${CC} ${OPTS} ${DEFS} -c -o PersistentMemoryManager.o PersistentMemoryManager.c
	${CC} -shared -o Platform.Data.Triplets.Kernel Link.o PersistentMemoryManager.o Timestamp.o

# Build simple test
test: Platform.Data.Triplets.Kernel
	${CC} ${OPTS} -o test test.c Platform.Data.Triplets.Kernel

# Build Google Test unit tests (requires googletest library)
# To use this target, make sure googletest is installed:
# On Ubuntu/Debian: sudo apt-get install libgtest-dev libgtest-dev
# On Arch: sudo pacman -S gtest
# On macOS with Homebrew: brew install googletest
unit_tests: Platform.Data.Triplets.Kernel
	${CXX} ${CXXOPTS} -I. -I../Platform.Data.Triplets.Kernel.Tests \
		../Platform.Data.Triplets.Kernel.Tests/PersistentMemoryManagerTests.cpp \
		../Platform.Data.Triplets.Kernel.Tests/LinkTests.cpp \
		-o unit_tests Platform.Data.Triplets.Kernel -lgtest -lgtest_main -lpthread

# Clean build artifacts
clean:
	rm -f *.o Platform.Data.Triplets.Kernel test unit_tests

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build library and simple test (default)"
	@echo "  test       - Build simple test only"
	@echo "  unit_tests - Build Google Test unit tests (requires libgtest)"
	@echo "  clean      - Remove build artifacts"
	@echo "  help       - Show this help message"

.PHONY: all test unit_tests clean help
