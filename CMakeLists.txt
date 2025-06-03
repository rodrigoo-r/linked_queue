include(FetchContent)

cmake_minimum_required(VERSION 3.31)
project(linked_queue C)

set(CMAKE_C_STANDARD 11)

add_library(linked_queue STATIC linked_queue.c linked_queue.h)
if(NOT FLUENT_LIBC_RELEASE) # Manually add libraries only if not in release mode
    FetchContent_Declare(
            types
            GIT_REPOSITORY https://github.com/rodrigoo-r/types
            GIT_TAG        master
    )

    FetchContent_Declare(
            stdbool
            GIT_REPOSITORY https://github.com/rodrigoo-r/std_bool
            GIT_TAG        master
    )

    FetchContent_MakeAvailable(types)
    FetchContent_MakeAvailable(stdbool)

    target_include_directories(linked_queue PRIVATE ${CMAKE_BINARY_DIR}/_deps/types-src)
    target_include_directories(linked_queue PRIVATE ${CMAKE_BINARY_DIR}/_deps/stdbool-src)
    target_link_libraries(linked_queue PRIVATE types)
    target_link_libraries(linked_queue PRIVATE stdbool)
endif ()