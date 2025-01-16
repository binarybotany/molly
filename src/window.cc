#include "window.h"

#include <stdexcept>

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
}

void window_controller::destroy(window_t &window) {
  if (window.glfw_window != nullptr) {
    glfwDestroyWindow(window.glfw_window);
  }

  glfwTerminate();
}

unsigned int texture;

void setup_texture() {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  const int tex_width = 256;
  const int tex_height = 256;
  unsigned char *pixels = (unsigned char *)malloc(tex_width * tex_height * 3);
  for (int y = 0; y < tex_height; ++y) {
    for (int x = 0; x < tex_width; ++x) {
      int index = (y * tex_width + x) * 3;
      pixels[index + 0] = (unsigned char)x;
      pixels[index + 1] = (unsigned char)y;
      pixels[index + 2] = 128;
    }
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, pixels);

  free(pixels);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);

  glBegin(GL_QUADS);
  glTexCoord2f(0.0f, 0.0f);
  glVertex2f(-1.0f, -1.0f);  // Bottom-left

  glTexCoord2f(1.0f, 0.0f);
  glVertex2f(1.0f, -1.0f);  // Bottom-right

  glTexCoord2f(1.0f, 1.0f);
  glVertex2f(1.0f, 1.0f);  // Top-right

  glTexCoord2f(0.0f, 1.0f);
  glVertex2f(-1.0f, 1.0f);  // Top-left
  glEnd();
}

void window_controller::startup(window_t &window) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  setup_texture();
  while (!glfwWindowShouldClose(window.glfw_window)) {
    glfwPollEvents();
    display();
    glfwSwapBuffers(window.glfw_window);
  }
}

void window_controller::shutdown(window_t &window) {}