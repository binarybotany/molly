cmake_minimum_required(VERSION 3.20)

set(molly_headers 
    "color.h" 
    "frame_buffer.h" 
    "program.h" 
    "window.h")

set(molly_sources 
    "color.cc" 
    "frame_buffer.cc" 
    "main.cc" 
    "program.cc" 
    "window.cc")

add_executable(molly ${molly_headers} ${molly_sources})
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")
target_link_libraries(molly PRIVATE glad glfw)