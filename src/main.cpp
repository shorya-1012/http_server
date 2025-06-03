#include "server.hpp"

int main(int argc, char *argv[]) {
  Server server;
  server.run(8080);
  return 0;
}
