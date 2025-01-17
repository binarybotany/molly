#include "frame_buffer.h"

void setup_buffers(frame_buffer_t &frame_buffer) {
  float vertices[] = {
      -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
      1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f,
  };

  uint32_t indices[] = {
      0, 1, 2, 0, 2, 3,
  };

  glGenVertexArrays(1, &frame_buffer.vao);
  glGenBuffers(1, &frame_buffer.vbo);
  glGenBuffers(1, &frame_buffer.ebo);

  glBindVertexArray(frame_buffer.vao);

  glBindBuffer(GL_ARRAY_BUFFER, frame_buffer.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, frame_buffer.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void setup_texture(frame_buffer_t &frame_buffer) {
  glGenTextures(1, &frame_buffer.texture);
  glBindTexture(GL_TEXTURE_2D, frame_buffer.texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame_buffer.width,
               frame_buffer.height, 0, GL_RGB, GL_UNSIGNED_BYTE,
               frame_buffer.buffer);
}

void frame_buffer_controller::start_up(frame_buffer_t &frame_buffer) {
  setup_buffers(frame_buffer);
  program_controller::create(frame_buffer.program);
  program_controller::use(frame_buffer.program);
  glUniform1i(glGetUniformLocation(frame_buffer.program.id, "screen_texture"),
              0);
  setup_texture(frame_buffer);
}

void frame_buffer_controller::shut_down(frame_buffer_t &frame_buffer) {}

void frame_buffer_controller::set_pixel(frame_buffer_t &frame_buffer,
                                        position_t position, color_t color) {
  if (position.x >= frame_buffer.width || position.y >= frame_buffer.height)
    return;

  uint32_t index = (position.y * frame_buffer.width + position.x) * 3;
  frame_buffer.buffer[index + 0] = color.r;
  frame_buffer.buffer[index + 1] = color.g;
  frame_buffer.buffer[index + 2] = color.b;
}

void frame_buffer_controller::update(frame_buffer_t &frame_buffer) {
  glBindTexture(GL_TEXTURE_2D, frame_buffer.texture);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame_buffer.width,
                  frame_buffer.height, GL_RGB, GL_UNSIGNED_BYTE,
                  frame_buffer.buffer);
}

void frame_buffer_controller::render(frame_buffer_t &frame_buffer) {
  glBindTexture(GL_TEXTURE_2D, frame_buffer.texture);
  glBindVertexArray(frame_buffer.vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}