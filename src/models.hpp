#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

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
  std::string status;
  std::string content_type;
  std::string content;
};
