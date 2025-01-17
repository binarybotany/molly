#pragma once

#include <cstdint>
#include <vector>

struct color_t {
  uint8_t r, g, b;
};

extern color_t mode13h[256];