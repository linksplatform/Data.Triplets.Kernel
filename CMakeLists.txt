cmake_minimum_required(VERSION 3.8)
project(Platform.Data.Triplets.Kernel C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_SOURCE_DIR Platform.Data.Triplets.Kernel)

add_library(Platform.Data.Triplets.Kernel STATIC
    ${CMAKE_SOURCE_DIR}/PersistentMemoryManager.c
    ${CMAKE_SOURCE_DIR}/Link.c
    ${CMAKE_SOURCE_DIR}/Timestamp.c
)

add_executable(test ${CMAKE_SOURCE_DIR}/test.c)
target_link_libraries(test PUBLIC Platform.Data.Triplets.Kernel)