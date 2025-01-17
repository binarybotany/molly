#include "texture.h"

void texture_controller::create(texture_t &texture, uint8_t *image) {
  glGenTextures(1, &texture.id);
  glBindTexture(GL_TEXTURE_2D, texture.id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0,
               GL_RGB, GL_UNSIGNED_BYTE, image);
}

void texture_controller::destroy(texture_t &texture) {
  glBindTexture(GL_TEXTURE_2D, 0);
  glDeleteTextures(1, &texture.id);
}

void texture_controller::bind(texture_t &texture) {
  glBindTexture(GL_TEXTURE_2D, texture.id);
}

void texture_controller::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void texture_controller::update(texture_t &texture, uint8_t *image) {
  glBindTexture(GL_TEXTURE_2D, texture.id);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, texture.width, texture.height, GL_RGB,
                  GL_UNSIGNED_BYTE, image);
}