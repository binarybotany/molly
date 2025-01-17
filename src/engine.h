#pragma once

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>

#include "framebuffer.h"
#include "window.h"

struct engine_t {
  window_t window;
  framebuffer_t frame_buffer;
};

class engine_controller {
 public:
  static void init(engine_t &engine);
  static void deinit(engine_t &engine);
  static void startup(engine_t &engine);
  static void shutdown(engine_t &engine);
  static void mainloop(engine_t &engine);
};