#include <iostream>

#include "window.h"

int main(int argc, char **argv) {
  window_t window;
  window.width = 1280;
  window.height = 800;
  window.title = "Molly";

  window_controller::create(window);
  window_controller::startup(window);
  window_controller::shutdown(window);
  window_controller::destroy(window);

  return 0;
}