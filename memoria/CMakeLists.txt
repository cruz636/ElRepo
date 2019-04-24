cmake_minimum_required(VERSION 3.13)
project(memoria C)

set(CMAKE_C_STANDARD 11)

set(PATH_MEMORIA "./src")

FILE(GLOB FILE_MEMORIA ${PATH_MEMORIA}/*.c ${PATH_MEMORIA}/*.h)

add_executable(memoria ${FILE_MEMORIA})

target_link_libraries(memoria -lreadline -lpthread -lcommons -lcompartidas)
