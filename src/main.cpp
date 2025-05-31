#include "server.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Provide a Port Number to continue.\n";
    exit(EXIT_FAILURE);
  }

  std::string p = argv[1];
  int port = atoi(p.c_str());

  int server_fd = init_server(port);
  while (true) {
    handle_client(server_fd);
  }
  return 0;
}
