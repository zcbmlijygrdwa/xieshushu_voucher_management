cmake_minimum_required(VERSION 3.10)

project(Xieshushu)

include_directories(
include
)

add_subdirectory(
libxlsxwriter
)

add_executable(xieshushu
src/todo.cpp
)

target_link_libraries(xieshushu
xlsxwriter
)

