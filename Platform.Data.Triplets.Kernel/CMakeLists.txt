cmake_minimum_required(VERSION 3.10)

project(Platform.Data.Triplets.Kernel C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler flags
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR CMAKE_C_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fPIC -W -Wall -Wextra -pedantic")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

# Platform-specific definitions
if(WIN32)
    add_definitions(-DWIN32 -D_WINDOWS -D_USRDLL -DCORE_EXPORTS)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_definitions(-D_DEBUG)
    else()
        add_definitions(-DNDEBUG)
    endif()
else()
    add_definitions(-DLINKS_DLL_EXPORT -D_XOPEN_SOURCE=700)
endif()

# Source files
set(SOURCES
    Link.c
    PersistentMemoryManager.c
    Timestamp.c
)

# Header files
set(HEADERS
    Common.h
    Link.h
    LinkLowLevel.h
    PersistentMemoryManager.h
    SizeBalancedTree.h
    Timestamp.h
)

# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})

# Set library properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION 1.0.0
    SOVERSION 1
    PUBLIC_HEADER "${HEADERS}"
)

# Install targets
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

# Create test executable (simple test from test.c)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/test.c")
    add_executable(test test.c)
    target_link_libraries(test ${PROJECT_NAME})
endif()