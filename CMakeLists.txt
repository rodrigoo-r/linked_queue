cmake_minimum_required(VERSION 3.31)
project(linked_queue C)

set(CMAKE_C_STANDARD 11)

add_library(linked_queue STATIC library.c)
