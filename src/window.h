#pragma once

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
  static void create(window_t &window);
  static void destroy(window_t &window);
  static void startup(window_t &window);
  static void shutdown(window_t &window);
};