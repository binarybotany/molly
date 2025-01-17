#pragma once

#include <glad/glad.h>

#include <cstdint>

struct texture_t {
  uint32_t id;
  uint32_t width, height;
};

class texture_controller {
 public:
  static void create(texture_t &texture, uint8_t *image);
  static void destroy(texture_t &texture);
  static void bind(texture_t &texture);
  static void unbind();
  static void update(texture_t &texture, uint8_t *image);
};