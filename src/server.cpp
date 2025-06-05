#include "server.hpp"
#include "models.hpp"
#include "utils.hpp"
#include <asm-generic/socket.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

void Server::add_routes(std::string path, RouteHandlerPtr handler_fn) {
  this->routes[path] = handler_fn;
}

int Server::init_server(int port) {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {
    perror("Failed to create socket connection\n");
    std::cerr << "Failed to Create Socket Connection" << std::endl;
    exit(EXIT_FAILURE);
  }

  int sock_opt = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt,
                 sizeof(sock_opt)) == -1) {
    std::cerr << "setsockopt Failed\n";
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
    std::cerr << "Failed to bind socket to fd " << server_fd
              << " To port : " << port << std::endl;
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 10) < 0) {
    perror("Failed to listen");
    std::cerr << "Failed to listen\n";
    exit(EXIT_FAILURE);
  }

  std::cout << "Listening To Server At Port : " << port << std::endl;

  return server_fd;
}

void Server::handle_client(int server_fd) {
  int pid = 0;
  struct sockaddr_in addr;
  int client_fd = -1;
  // char buffer[4096];
  std::string buffer(4096, '\0');
  socklen_t addr_len = sizeof(addr);

  client_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
  if (client_fd < 0) {
    std::cerr << "Failed to create clinet Connection\n";
    return;
  }

  pid = fork();

  if (pid == 0) {
    int bytes_read = read(client_fd, &buffer[0], buffer.size());
    if (bytes_read > 0) {
      buffer.resize(bytes_read);
    }

    HttpRequest req = parse_tokens(buffer);
    std::cout << "Request Object : " << std::endl;
    std::cout << req << std::endl;

    HttpResponse res(client_fd);

    std::string path = req.path;
    if (str_ends_with(path, ".css")) {
      res.send_css(200, path);
    } else if (routes.find(path) == routes.end()) {
      res.send_text(404, "Not Found");
    } else {
      routes[path](req, res);
    }

    close(client_fd);
    exit(EXIT_SUCCESS);
  }
  close(client_fd);
  while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
    std::cout << "Reaped Process " << pid << std::endl;
  }
}

void Server::run(int port) {
  int server_fd = init_server(port);
  while (true) {
    handle_client(server_fd);
  }
}
