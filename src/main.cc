#include "engine.h"

int main(int argc, char **argv) {
  engine_t engine;
  engine_controller::init(engine);
  engine_controller::startup(engine);
  engine_controller::mainloop(engine);
  engine_controller::shutdown(engine);
  engine_controller::deinit(engine);

  return 0;
}