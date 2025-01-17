cmake_minimum_required(VERSION 3.20)

set(molly_headers 
    "buffers.h"
    "color.h" 
    "engine.h"
    "framebuffer.h" 
    "program.h" 
    "texture.h"
    "window.h")

set(molly_sources
    "buffers.cc"
    "color.cc"
    "engine.cc" 
    "framebuffer.cc" 
    "main.cc" 
    "program.cc"
    "texture.cc" 
    "window.cc")

add_executable(molly ${molly_headers} ${molly_sources})
target_compile_definitions(molly PRIVATE $<$<CONFIG:Debug>:DEBUG=1>)
target_link_libraries(molly PRIVATE glad glfw)