#include "models.hpp"
#include "server.hpp"

int main(int argc, char *argv[]) {
  Server server;

  server.add_routes("/", [](HttpRequest &req, HttpResponse &res) {
    res.send_html(200, "../templates/index.html");
  });

  server.add_routes("/about", [](HttpRequest &req, HttpResponse &res) {
    res.send_text(200, "Welcome to About Page");
  });

  server.add_routes("/foo", [](HttpRequest &req, HttpResponse &res) {
    res.send_text(200, "BAR");
  });

  server.run(8080);
  return 0;
}
