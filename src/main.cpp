#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

int init_server(int port) {
  int server_fd;
  int bind_result;
  struct sockaddr_in address;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);

  if (server_fd < 0) {
    perror("Failed to create socket connection\n");
    std::cerr << "Failed to Create Socket Connection" << std::endl;
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  bind_result = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
  if (bind_result == -1) {
    std::cerr << "Failed to bind socket to fd " << server_fd
              << " To prot : " << port << std::endl;
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

void handle_client(int server_fd) {
  int pid = 0;
  struct sockaddr_in addr;
  int client_fd = -1;
  char buffer[4096];
  socklen_t addr_len = sizeof(addr);

  client_fd = accept(server_fd, (struct sockaddr *)&addr, &addr_len);
  if (client_fd < 0) {
    std::cerr << "Failed to create clinet Connection\n";
    return;
  }

  pid = fork();

  if (pid == 0) {
    read(client_fd, buffer, sizeof(buffer));

    printf("RESPONSE : \n%s\n", buffer);

    const char *success_responses =
        "HTTP/1.0 200 OK\r\n"
        "Content-Type : text\r\n"
        "Content-Length: 48\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><body><h1>Hello, World!</h1></body></html>";

    write(client_fd, success_responses, strlen(success_responses));
    close(client_fd);
    exit(EXIT_SUCCESS);
  }
  close(client_fd);
  while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
    std::cout << "Reaped Process " << pid << std::endl;
  }
}

int main(int argc, char *argv[]) {
  int port = 8080;
  int server_fd = init_server(port);
  while (true) {
    handle_client(server_fd);
  }
  return 0;
}
