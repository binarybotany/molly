#include <iostream>

#include "window.h"

int main(int argc, char **argv) {
  window_t window;
  window.width = 1024;
  window.height = 768;
  window.title = "Molly";

  window_controller::create(window);
  window_controller::startup(window);
  window_controller::shutdown(window);
  window_controller::destroy(window);

  return 0;
}