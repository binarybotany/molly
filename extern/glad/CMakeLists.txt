
cmake_minimum_required(VERSION 3.20)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

project(glad LANGUAGES C)

add_library(glad "include/glad/glad.h" "src/glad.c")
target_include_directories(glad PUBLIC "include")