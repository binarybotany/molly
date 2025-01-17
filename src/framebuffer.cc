#include "framebuffer.h"

void framebuffer_controller::init(framebuffer_t &framebuffer) {}

void framebuffer_controller::deinit(framebuffer_t &framebuffer) {
  texture_controller::destroy(framebuffer.texture);
}

void framebuffer_controller::startup(framebuffer_t &framebuffer) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  buffers_controller::init(framebuffer.buffers);
  program_controller::create(framebuffer.program);
  program_controller::use(framebuffer.program);

  framebuffer.texture.width = framebuffer.width;
  framebuffer.texture.height = framebuffer.height;
  texture_controller::create(framebuffer.texture, framebuffer.buffer);

  glUniform1i(glGetUniformLocation(framebuffer.program.id, "screen_texture"),
              0);

  glUniform2f(
      glGetUniformLocation(framebuffer.program.id, "viewport_resolution"),
      1920.0f, 1080.0f);

  for (uint32_t y = 0; y < framebuffer.height; ++y) {
    for (uint32_t x = 0; x < framebuffer.width; ++x) {
      framebuffer_controller::set_pixel(framebuffer, {x, y}, mode13h[0x00]);
    }
  }
}

void framebuffer_controller::shutdown(framebuffer_t &framebuffer) {}

void framebuffer_controller::set_pixel(framebuffer_t &framebuffer,
                                       position_t position, color_t color) {
  if (position.x >= framebuffer.width || position.y >= framebuffer.height)
    return;

  uint32_t index = (position.y * framebuffer.width + position.x) * 3;
  framebuffer.buffer[index + 0] = color.r;
  framebuffer.buffer[index + 1] = color.g;
  framebuffer.buffer[index + 2] = color.b;
}

void framebuffer_controller::update(framebuffer_t &framebuffer) {
  texture_controller::update(framebuffer.texture, framebuffer.buffer);
}

void framebuffer_controller::render(framebuffer_t &framebuffer) {
  glClear(GL_COLOR_BUFFER_BIT);
  texture_controller::bind(framebuffer.texture);
  buffers_controller::bind(framebuffer.buffers);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  buffers_controller::unbind();
  texture_controller::unbind();
}

void framebuffer_controller::set_viewport_resolution(framebuffer_t &framebuffer,
                                                     float width,
                                                     float height) {
  glUniform2f(
      glGetUniformLocation(framebuffer.program.id, "viewport_resolution"),
      width, height);
}