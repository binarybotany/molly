#include "window.h"

#include <stdexcept>

void window_controller::init(window_t &window) {
  if (!glfwInit()) {
    throw std::runtime_error("Unable to intitialize GLFW");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  GLFWmonitor *monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode *video_mode = glfwGetVideoMode(monitor);
  window.width = video_mode->width;
  window.height = video_mode->height;
  GLFWwindow *glfw_window = glfwCreateWindow(
      window.width, window.height, window.title.c_str(), monitor, nullptr);

  if (!glfw_window) {
    throw std::runtime_error("Unable to create GLFW window");
  }

  window.glfw_window = glfw_window;
  glfwMakeContextCurrent(window.glfw_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Unable to load OpenGL 3.3 context");
  }
}

void window_controller::deinit(window_t &window) {
  if (window.glfw_window != nullptr) {
    glfwDestroyWindow(window.glfw_window);
  }

  glfwTerminate();
}

void window_controller::startup(window_t &window) {}

void window_controller::shutdown(window_t &window) {}