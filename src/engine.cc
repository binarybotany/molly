#include "engine.h"

#include <stdexcept>

void on_resize(GLFWwindow *window, int width, int height) {
  engine_t *engine = (engine_t *)glfwGetWindowUserPointer(window);
  if (engine == nullptr) {
    throw std::runtime_error("Null pointer when dereferencing engine");
  }

  glViewport(0, 0, width, height);
  framebuffer_controller::set_viewport_resolution(engine->frame_buffer,
                                                  static_cast<float>(width),
                                                  static_cast<float>(height));
}

void on_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}

void engine_controller::init(engine_t &engine) {
  engine.window.title = "Molly";
  engine.window.width = 1920;
  engine.window.height = 1080;
  window_controller::init(engine.window);
  framebuffer_controller::startup(engine.frame_buffer);

  glfwSetWindowUserPointer(engine.window.glfw_window, &engine);
  glfwSetFramebufferSizeCallback(engine.window.glfw_window, on_resize);

  glfwSetKeyCallback(engine.window.glfw_window, on_key);
}

void engine_controller::deinit(engine_t &engine) {
  framebuffer_controller::deinit(engine.frame_buffer);
  window_controller::deinit(engine.window);
}

void engine_controller::startup(engine_t &engine) {
  window_controller::startup(engine.window);
  framebuffer_controller::startup(engine.frame_buffer);
}

void engine_controller::shutdown(engine_t &engine) {
  framebuffer_controller::shutdown(engine.frame_buffer);
  window_controller::shutdown(engine.window);
}

void engine_controller::mainloop(engine_t &engine) {
  while (!glfwWindowShouldClose(engine.window.glfw_window)) {
    glfwPollEvents();
    framebuffer_controller::update(engine.frame_buffer);
    framebuffer_controller::render(engine.frame_buffer);
    glfwSwapBuffers(engine.window.glfw_window);
  }
}