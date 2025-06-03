include(FetchContent)

cmake_minimum_required(VERSION 3.31)
project(linked_queue C)

set(CMAKE_C_STANDARD 11)

add_library(linked_queue STATIC library.c)
if(NOT FLUENT_LIBC_RELEASE) # Manually add libraries only if not in release mode
    FetchContent_Declare(
            types
            GIT_REPOSITORY https://github.com/rodrigoo-r/types
            GIT_TAG        master
    )

    FetchContent_MakeAvailable(types)

    target_include_directories(linked_queue PRIVATE ${CMAKE_BINARY_DIR}/_deps/types-src)
    target_link_libraries(linked_queue PRIVATE types)
endif ()