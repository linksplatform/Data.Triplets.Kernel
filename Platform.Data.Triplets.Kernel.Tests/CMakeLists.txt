cmake_minimum_required(VERSION 3.10)

project(Platform.Data.Triplets.Kernel.Tests CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the main library
find_library(PLATFORM_DATA_TRIPLETS_KERNEL_LIB 
    NAMES Platform.Data.Triplets.Kernel
    PATHS ${CMAKE_BINARY_DIR}/../Platform.Data.Triplets.Kernel
    NO_DEFAULT_PATH
)

# Option to enable/disable tests
option(BUILD_TESTS "Build tests" ON)

if(BUILD_TESTS)
    # Try to find GTest
    find_package(GTest QUIET)
    
    if(GTest_FOUND)
        # Use GTest if available
        set(TEST_SOURCES
            LinkTests.cpp
            PersistentMemoryManagerTests.cpp
            stdafx.cpp
        )

        add_executable(${PROJECT_NAME} ${TEST_SOURCES})
        
        target_include_directories(${PROJECT_NAME} PRIVATE 
            ${CMAKE_SOURCE_DIR}/Platform.Data.Triplets.Kernel
        )
        
        target_link_libraries(${PROJECT_NAME} 
            GTest::gtest
            GTest::gtest_main
        )
        
        # Link with the main library if found
        if(PLATFORM_DATA_TRIPLETS_KERNEL_LIB)
            target_link_libraries(${PROJECT_NAME} ${PLATFORM_DATA_TRIPLETS_KERNEL_LIB})
        endif()
        
        # Enable testing
        enable_testing()
        add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
        
    else()
        message(WARNING "GTest not found. Tests will not be built.")
        message(STATUS "Install GTest or use -DBUILD_TESTS=OFF to disable tests")
    endif()
endif()

# Note: The original tests use Microsoft Visual Studio C++ Unit Test Framework
# For cross-platform compatibility, they would need to be converted to GTest or similar
# Current CMakeLists.txt assumes this conversion or provides conditional building