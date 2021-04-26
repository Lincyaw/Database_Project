cmake_minimum_required(VERSION 3.17)
project(extmem_c C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(extmem_c
        extmem.c
        extmem.h
        template.c linearSearch.c linearSearch.h 2stageSort.c 2stageSort.h)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fexec-charset=GBK")
