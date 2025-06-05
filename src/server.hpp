#pragma once

#include "models.hpp"
#include <string>
#include <unordered_map>

typedef void (*RouteHandlerPtr)(HttpRequest &req, HttpResponse &res);
struct Server {
  void add_routes(std::string path, RouteHandlerPtr handler_fn);
  void run(int port);

private:
  std::unordered_map<std::string, RouteHandlerPtr> routes;
  int init_server(int port);
  void handle_client(int server_fd);
};
