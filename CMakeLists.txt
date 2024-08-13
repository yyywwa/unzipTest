cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_CXX_STANDARD 17)
project(hello VERSION 0.1.0 LANGUAGES C CXX)

include_directories(./include)
link_directories(./lib)

aux_source_directory(./src SRC_FILE)

add_executable(hello ${SRC_FILE})

target_link_libraries(hello PRIVATE archive)