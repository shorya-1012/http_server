#pragma once

#include <cstdint>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>

const std::unordered_map<int, std::string> response_codes = {
    {200, "OK"},
    {201, "Created"},
    {204, "No Content"},
    {301, "Moved Permanently"},
    {302, "Found"},
    {400, "Bad Request"},
    {401, "Unauthorized"},
    {403, "Forbidden"},
    {404, "Not Found"},
    {500, "Internal Server Error"},
    {501, "Not Implemented"},
    {503, "Service Unavailable"}};

enum HttpMethods {
  GET,
  POST,
  PUT,
  PATCH,
  DELETE,
  UNKNOWN,
};

template <typename T> struct DataPair {
  bool has_error;
  T data;
  DataPair<T>(bool has_error, T data) : has_error(has_error), data(data) {}
};

struct HttpRequest {
  HttpMethods method;
  std::string path;
  std::string version;
  std::unordered_map<std::string, std::string> headers;
  std::string body;
  friend std::ostream &operator<<(std::ostream &out, const HttpRequest &req) {
    HttpMethods method = req.method;
    switch (method) {
    case GET:
      out << "GET\n";
      break;
    case POST:
      out << "POST\n";
      break;
    case PUT:
      out << "PUT\n";
      break;
    case PATCH:
      out << "PATCH\n";
      break;
    case DELETE:
      out << "DELETE\n";
      break;
    default:
      out << "UNKNOWN\n";
    }
    out << req.path << std::endl;
    out << req.version << std::endl;
    for (auto &[k, v] : req.headers) {
      out << k << " : " << v << std::endl;
    }
    out << req.body << std::endl;
    return out;
  }
};

struct HttpResponse {
  void send_text(int client_fd, uint16_t status, std::string content) {
    auto response = construct_response(status, "text", content);
    write(client_fd, response.c_str(), response.size());
  }
  void send_html(int client_fd, uint16_t status, std::string path) {
    DataPair file_data = read_file(path);
    if (file_data.has_error) {
      send_text(client_fd, 404, file_data.data);
      return;
    }
    auto response = construct_response(status, "html", file_data.data);
    write(client_fd, response.c_str(), response.size());
  }

private:
  std::string construct_response(uint16_t status, std::string type,
                                 std::string &content) {
    std::stringstream res;

    res << "HTTP/1.1 " << status << " " << response_codes.at(status) << "\r\n";
    res << "Content-Type : " << type << "\r\n";
    res << "Content-Length: " << content.size() << "\r\n";
    res << "Connection: close\r\n";
    res << "\r\n";
    res << content << "\r\n";

    return res.str();
  }
  DataPair<std::string> read_file(std::string &path) {
    std::ifstream infile(path);
    if (!infile) {
      return DataPair<std::string>(true, "Unable to find HTML file");
    }
    std::ostringstream file_data;
    file_data << infile.rdbuf();

    return DataPair<std::string>(false, file_data.str());
  }
};
// const char *success_responses =
//     "HTTP/1.0 200 OK\r\n"
//     "Content-Type : text\r\n"
//     "Content-Length: 48\r\n"
//     "Connection: close\r\n"
//     "\r\n"
//     "<html><body><h1>Hello, World!</h1></body></html>";
