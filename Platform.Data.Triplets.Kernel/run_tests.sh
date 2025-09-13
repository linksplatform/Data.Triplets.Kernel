#!/usr/bin/env bash

# Build and run tests script for Platform.Data.Triplets.Kernel
# This script supports both simple test and Google Test unit tests

set -e

echo "Building Platform.Data.Triplets.Kernel library and tests..."

# Build the main library and simple test
make all

echo ""
echo "Running simple test..."
LD_LIBRARY_PATH=`pwd` ./test

echo ""
echo "Checking if Google Test is available..."

# Check if Google Test is available
if command -v pkg-config &> /dev/null && pkg-config --exists gtest; then
    echo "Google Test found via pkg-config, building unit tests..."
    make unit_tests
    echo ""
    echo "Running Google Test unit tests..."
    LD_LIBRARY_PATH=`pwd` ./unit_tests
elif [ -f /usr/lib/libgtest.a ] || [ -f /usr/local/lib/libgtest.a ] || [ -f /usr/lib/x86_64-linux-gnu/libgtest.a ]; then
    echo "Google Test library found, building unit tests..."
    make unit_tests
    echo ""
    echo "Running Google Test unit tests..."
    LD_LIBRARY_PATH=`pwd` ./unit_tests
else
    echo "Google Test not found. You can install it with:"
    echo "  Ubuntu/Debian: sudo apt-get install libgtest-dev cmake && cd /usr/src/gtest && sudo cmake . && sudo make && sudo cp *.a /usr/lib"
    echo "  Arch Linux: sudo pacman -S gtest"
    echo "  macOS: brew install googletest"
    echo ""
    echo "Alternatively, use CMake to automatically fetch Google Test:"
    echo "  mkdir build && cd build && cmake .. && make && ./unit_tests"
    echo ""
    echo "Only simple test was run."
fi

echo ""
echo "Done!"