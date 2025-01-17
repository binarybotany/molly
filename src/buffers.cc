#include "buffers.h"

void buffers_controller::init(buffers_t &buffers) {
  const float vertices[] = {
      -1.0f, 1.0f,  0.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
      1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,  1.0f,  1.0f, 1.0f,
  };

  const uint32_t indices[] = {
      0, 1, 2, 0, 2, 3,
  };

  glGenVertexArrays(1, &buffers.vao);
  glGenBuffers(1, &buffers.vbo);
  glGenBuffers(1, &buffers.ebo);

  buffers_controller::bind(buffers);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
  glEnableVertexAttribArray(1);

  buffers_controller::unbind();
}

void buffers_controller::deinit(buffers_t &buffers) {}

void buffers_controller::bind(buffers_t &buffers) {
  glBindVertexArray(buffers.vao);
  glBindBuffer(GL_ARRAY_BUFFER, buffers.vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers.ebo);
}

void buffers_controller::unbind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}