#pragma once

#include <glad/glad.h>

#include <cstdint>

struct program_t {
  uint32_t id;
};

class program_controller {
 public:
  static void create(program_t &program);
  static void destroy(program_t &program);
  static void use(program_t &program);
};