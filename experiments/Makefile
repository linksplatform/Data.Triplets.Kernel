# Makefile for tree traversal performance benchmark

CC = gcc
CFLAGS = -Wall -O2 -std=c99
INCLUDE_PATH = -I../Platform.Data.Triplets.Kernel
KERNEL_PATH = ../Platform.Data.Triplets.Kernel

# Source files from the kernel
KERNEL_SOURCES = \
	$(KERNEL_PATH)/Link.c \
	$(KERNEL_PATH)/PersistentMemoryManager.c \
	$(KERNEL_PATH)/Timestamp.c

BENCHMARK_SOURCES = tree_traversal_benchmark.c

# Object files
KERNEL_OBJECTS = $(KERNEL_SOURCES:.c=.o)
BENCHMARK_OBJECTS = $(BENCHMARK_SOURCES:.c=.o)

# Target executables
TARGET1 = tree_traversal_benchmark
TARGET2 = comprehensive_benchmark

# Default target
all: $(TARGET1) $(TARGET2)

# Build the benchmark executables
$(TARGET1): tree_traversal_benchmark.o $(KERNEL_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET2): comprehensive_benchmark.o $(KERNEL_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile benchmark source
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -c $< -o $@

# Compile kernel sources
$(KERNEL_PATH)/%.o: $(KERNEL_PATH)/%.c
	$(CC) $(CFLAGS) $(INCLUDE_PATH) -c $< -o $@

# Run the benchmarks
run: $(TARGET1) $(TARGET2)
	echo "Running basic benchmark..."
	./$(TARGET1)
	echo "Running comprehensive benchmark..."
	./$(TARGET2)

run-comprehensive: $(TARGET2)
	./$(TARGET2)

# Clean build artifacts
clean:
	rm -f $(TARGET1) $(TARGET2) *.o $(KERNEL_OBJECTS)
	rm -f benchmark_db.links comprehensive_benchmark.links

# Debug build
debug: CFLAGS += -g -DDEBUG
debug: $(TARGET1) $(TARGET2)

.PHONY: all run clean debug