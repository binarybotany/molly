cmake_minimum_required(VERSION 3.20)

find_package(OpenGL REQUIRED)

set(molly_headers "window.h")

set(molly_sources "main.cc" "window.cc")

add_executable(molly ${molly_headers} ${molly_sources})
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")
target_link_libraries(molly PRIVATE OpenGL::GL glfw)