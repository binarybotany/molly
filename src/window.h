#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <string>

#include "color.h"
#include "frame_buffer.h"

struct window_t {
  GLFWwindow *glfw_window;
  unsigned int width;
  unsigned int height;
  std::string title;
  frame_buffer_t frame_buffer;
};

class window_controller {
 public:
  static void create(window_t &window);
  static void destroy(window_t &window);
  static void startup(window_t &window);
  static void shutdown(window_t &window);
};