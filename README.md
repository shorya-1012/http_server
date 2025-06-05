# Simple HTTP Server in C++

This is a basic HTTP server implemented from scratch in C++ with support for serving HTML, plain text, and CSS files.

## Features

- Handle multiple routes
- Concurrent handling using `fork()`
- Serve static HTML and CSS files
- Send plain text responses


## Build Instructions

1. Clone the repository:

```bash
git clone https://github.com/shorya-1012/http_server.git
cd http-server
````

2. Build the project using CMake:

```bash
mkdir build && cd build
cmake ..
make
```

3. Run the server:

```bash
./server
```

4. Open your browser and navigate to:

* `http://localhost:8080/`

## Example Main Function

```cpp
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
```

## Dependencies

No external dependencies. Uses only standard C++ and POSIX system calls (`socket`, `fork`, `read`, `write`, etc.).
