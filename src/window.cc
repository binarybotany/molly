#include "window.h"

#include <stdexcept>

void on_resize(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void window_controller::create(window_t &window) {
  if (!glfwInit()) {
    throw std::runtime_error("Unable to intitialize GLFW");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

  GLFWwindow *glfw_window = glfwCreateWindow(
      window.width, window.height, window.title.c_str(), nullptr, nullptr);

  if (!glfw_window) {
    throw std::runtime_error("Unable to create GLFW window");
  }

  window.glfw_window = glfw_window;
  glfwMakeContextCurrent(window.glfw_window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    throw std::runtime_error("Unable to load OpenGL 3.3 context");
  }

  glfwSetFramebufferSizeCallback(window.glfw_window, on_resize);
}

void window_controller::destroy(window_t &window) {
  if (window.glfw_window != nullptr) {
    glfwDestroyWindow(window.glfw_window);
  }

  glfwTerminate();
}

void window_controller::startup(window_t &window) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  frame_buffer_controller::start_up(window.frame_buffer);

  while (!glfwWindowShouldClose(window.glfw_window)) {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);
    int32_t index = 0;
    for (uint32_t y = 0; y < window.frame_buffer.height; ++y) {
      for (uint32_t x = 0; x < window.frame_buffer.width; ++x) {
        position_t position = {x, y};
        if (index > 256) index = 0;
        color_t color = mode13h[index++];
        frame_buffer_controller::set_pixel(window.frame_buffer, position,
                                           color);
      }
    }

    frame_buffer_controller::update(window.frame_buffer);
    frame_buffer_controller::render(window.frame_buffer);

    glfwSwapBuffers(window.glfw_window);
  }

  frame_buffer_controller::shut_down(window.frame_buffer);
}

void window_controller::shutdown(window_t &window) {}