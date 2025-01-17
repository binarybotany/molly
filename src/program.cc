#include "program.h"

#include <stdexcept>

const char *vss = R"(
#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_texture_coordinates;

out vec2 texture_coordinates;

void main() {
  gl_Position = vec4(a_position, 0.0, 1.0);
  texture_coordinates = a_texture_coordinates;
}
)";

const char *fss = R"(
#version 330 core

out vec4 out_color;

in vec2 texture_coordinates;
uniform sampler2D screen_texture;

void main() {
  vec2 uv = texture_coordinates.st;
  vec2 resolution = vec2(320, 200);

  vec3 color = texture2D(screen_texture, uv).rgb * 0.5;
  color += texture2D(screen_texture, uv + vec2(1.0 / resolution.x, 0.0)).rgb * 0.125;
  color += texture2D(screen_texture, uv - vec2(1.0 / resolution.x, 0.0)).rgb * 0.125;
  color += texture2D(screen_texture, uv + vec2(0.0, 1.0 / resolution.y)).rgb * 0.125;
  color += texture2D(screen_texture, uv - vec2(0.0, 1.0 / resolution.y)).rgb * 0.125;

  if (mod(gl_FragCoord.y, 2.0) < 1.0) {
    color *= 0.8; // Darken
  }
  
  out_color = vec4(color, 1.0);
}
)";

uint32_t add_shader(program_t &program, uint32_t type, const char *source) {
  uint32_t shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  int32_t compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    int32_t log_length = 0;
    char message[1024];
    glGetShaderInfoLog(shader, 1024, &log_length, message);
    throw std::runtime_error(message);
  }

  glAttachShader(program.id, shader);
  return shader;
}

void program_controller::create(program_t &program) {
  program.id = glCreateProgram();
  uint32_t vs = add_shader(program, GL_VERTEX_SHADER, vss);
  uint32_t fs = add_shader(program, GL_FRAGMENT_SHADER, fss);
  glLinkProgram(program.id);

  int32_t linked;
  glGetProgramiv(program.id, GL_LINK_STATUS, &linked);
  if (!linked) {
    int32_t log_length = 0;
    char message[1024];
    glGetProgramInfoLog(program.id, 1024, &log_length, message);
    throw std::runtime_error(message);
  }
}

void program_controller::destroy(program_t &program) {
  glDeleteProgram(program.id);
}

void program_controller::use(program_t &program) { glUseProgram(program.id); }