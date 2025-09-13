cmake_minimum_required(VERSION 3.10)

project(Platform.Data.Triplets.Kernel)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Add the main library
add_subdirectory(Platform.Data.Triplets.Kernel)

# Add tests if enabled
if(BUILD_TESTS)
    add_subdirectory(Platform.Data.Triplets.Kernel.Tests)
endif()

# Print build summary
message(STATUS "")
message(STATUS "==== Build Configuration Summary ====")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Build shared libs: ${BUILD_SHARED_LIBS}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "=====================================")
message(STATUS "")