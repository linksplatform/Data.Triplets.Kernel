# CMakeLists.txt for Platform.Data.Triplets.Kernel Tests
cmake_minimum_required(VERSION 3.14)

# Google Test setup
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50f33f9142032e2f8725b1ee7b49a0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Test sources
set(TEST_SOURCES
    PersistentMemoryManagerTests.cpp
    LinkTests.cpp
)

# Create test executable
add_executable(unit_tests ${TEST_SOURCES})

# Link with Google Test and the main library
target_link_libraries(unit_tests
    gtest_main
    Platform.Data.Triplets.Kernel
)

# Include directories
target_include_directories(unit_tests PRIVATE
    ../Platform.Data.Triplets.Kernel
)

# Enable testing
enable_testing()

# Discover and add tests
include(GoogleTest)
gtest_discover_tests(unit_tests)