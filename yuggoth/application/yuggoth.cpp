#include "application.h"

int main(int argc, char **argv) {

  Yuggoth::GraphicsContext graphics_context;
  Yuggoth::GraphicsAllocator graphics_allocator;
  Yuggoth::Application application;

  application.Run();

  return 0;
}