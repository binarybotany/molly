#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <cstdint>

#include "color.h"
#include "program.h"

struct position_t {
  uint32_t x, y;
};

struct frame_buffer_t {
  static const uint32_t width = 320;
  static const uint32_t height = 200;
  uint8_t buffer[width * height * 3];
  program_t program;
  uint32_t vbo, vao, ebo, texture;
};

class frame_buffer_controller {
 public:
  static void start_up(frame_buffer_t &frame_buffer);

  static void shut_down(frame_buffer_t &frame_buffer);

  static void set_pixel(frame_buffer_t &frame_buffer, position_t position,
                        color_t color);

  static void update(frame_buffer_t &frame_buffer);

  static void render(frame_buffer_t &frame_buffer);
};