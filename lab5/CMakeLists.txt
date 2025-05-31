cmake_minimum_required(VERSION 3.5.1)
project(lab5 C)

set(SRC src/main.c
        src/compress.c
        src/compress.h
        src/decompress.c
        src/decompress.h
        src/huffmanTree.c
        src/huffmanTree.h
        src/heap.c
        src/heap.h
        src/archiverTools.c
        src/archiverTools.h
)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
