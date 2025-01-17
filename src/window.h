#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <string>

struct window_t {
  GLFWwindow *glfw_window;
  unsigned int width;
  unsigned int height;
  std::string title;
};

class window_controller {
 public:
  static void init(window_t &window);
  static void deinit(window_t &window);
  static void startup(window_t &window);
  static void shutdown(window_t &window);
};