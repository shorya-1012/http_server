#pragma once

struct Server {
  void run(int port);

private:
  int init_server(int port);
  void handle_client(int server_fd);
};
