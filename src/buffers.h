#pragma once

#include <glad/glad.h>

#include <cstdint>

struct buffers_t {
  uint32_t vao, vbo, ebo;
};

class buffers_controller {
 public:
  static void init(buffers_t &buffers);
  static void deinit(buffers_t &buffers);
  static void bind(buffers_t &buffers);
  static void unbind();
};