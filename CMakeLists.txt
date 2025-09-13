cmake_minimum_required(VERSION 3.14)
project(Platform.Data.Triplets.Kernel)

# Set C standard
set(CMAKE_C_STANDARD 99)

# Main library
add_library(Platform.Data.Triplets.Kernel SHARED
    Platform.Data.Triplets.Kernel/Link.c
    Platform.Data.Triplets.Kernel/PersistentMemoryManager.c
    Platform.Data.Triplets.Kernel/Timestamp.c
)

target_include_directories(Platform.Data.Triplets.Kernel PUBLIC
    Platform.Data.Triplets.Kernel
)

target_compile_definitions(Platform.Data.Triplets.Kernel PRIVATE
    LINKS_DLL_EXPORT
    _XOPEN_SOURCE=700
)

if(CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(Platform.Data.Triplets.Kernel PRIVATE
        -O3 -fPIC -W -Wall -Wextra -pedantic
    )
endif()

# Test executable using main library
add_executable(test_main Platform.Data.Triplets.Kernel/test.c)
target_link_libraries(test_main Platform.Data.Triplets.Kernel)

# Google Test setup for unit tests
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50f33f9142032e2f8725b1ee7b49a0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Unit test executable
add_executable(unit_tests
    Platform.Data.Triplets.Kernel.Tests/PersistentMemoryManagerTests.cpp
    Platform.Data.Triplets.Kernel.Tests/LinkTests.cpp
)

target_link_libraries(unit_tests
    Platform.Data.Triplets.Kernel
    gtest_main
)

target_include_directories(unit_tests PRIVATE
    Platform.Data.Triplets.Kernel
)

# Enable testing
enable_testing()

include(GoogleTest)
gtest_discover_tests(unit_tests)