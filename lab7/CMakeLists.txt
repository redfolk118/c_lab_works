cmake_minimum_required(VERSION 3.5.1)
project(lab7 C)

set(SRC src/main.c
        src/graph_units.h
        src/topological_sort.c
        src/graph_units.c
        src/topological_sort.h
)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
