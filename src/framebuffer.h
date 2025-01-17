#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <cstdint>

#include "buffers.h"
#include "color.h"
#include "program.h"
#include "texture.h"

struct position_t {
  uint32_t x, y;
};

struct framebuffer_t {
  static const uint32_t width = 480;
  static const uint32_t height = 270;
  uint8_t buffer[width * height * 3];
  buffers_t buffers;
  program_t program;
  texture_t texture;
  uint32_t id;
};

class framebuffer_controller {
 public:
  static void init(framebuffer_t &frame_buffer);
  static void deinit(framebuffer_t &frame_buffer);
  static void startup(framebuffer_t &frame_buffer);
  static void shutdown(framebuffer_t &frame_buffer);

  static void set_pixel(framebuffer_t &frame_buffer, position_t position,
                        color_t color);

  static void update(framebuffer_t &frame_buffer);
  static void render(framebuffer_t &frame_buffer);

  static void set_viewport_resolution(framebuffer_t &framebuffer, float width,
                                      float height);
};