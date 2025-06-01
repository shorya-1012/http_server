#pragma once

#include <cstdint>
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
  uint16_t status;
  std::string content_type;
  std::string content;
  HttpResponse(uint16_t status, std::string content) {
    this->status = status;
    this->content = "text";
    this->content = content;
  }
  std::string construct_text_response() {
    std::stringstream res;

    res << "HTTP/1.1 " << status << " " << response_codes.at(status) << "\r\n";
    res << "Content-Type : text\r\n";
    res << "Content-Length: " << content.size() << "\r\n";
    res << "Connection: close\r\n";
    res << "\r\n";
    res << content << "\r\n";

    return res.str();
  }
  void send(int client_fd) {
    auto response = construct_text_response();
    write(client_fd, response.c_str(), response.size());
  }
};
// const char *success_responses =
//     "HTTP/1.0 200 OK\r\n"
//     "Content-Type : text\r\n"
//     "Content-Length: 48\r\n"
//     "Connection: close\r\n"
//     "\r\n"
//     "<html><body><h1>Hello, World!</h1></body></html>";
