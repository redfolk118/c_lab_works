cmake_minimum_required(VERSION 3.5.1)
project(lab6-3 C)

set(SRC src/main.c
        src/prefix_tree.c
        src/prefix_tree.h
        src/string_units.c
        src/string_units.h)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
