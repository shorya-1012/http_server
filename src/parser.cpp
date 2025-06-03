#include "parser.hpp"
#include "models.hpp"
#include <cstddef>
#include <vector>

std::vector<std::string> split_str(const std::string &buffer,
                                   const std::string delimiter) {
  std::vector<std::string> tokens;
  size_t pos, start = 0;
  while ((pos = buffer.find(delimiter, start)) != std::string::npos) {
    tokens.emplace_back(buffer.substr(start, pos - start));
    start = pos + delimiter.size();
  }
  tokens.push_back(buffer.substr(start));
  return tokens;
}

void parse_request_line(std::vector<std::string> &tokens, HttpRequest &req) {
  std::string req_line = tokens[0];
  auto split_line = split_str(req_line, " ");
  std::string method = split_line[0];
  req.method = (method == "GET"      ? HttpMethods::GET
                : method == "POST"   ? HttpMethods::POST
                : method == "PUT"    ? HttpMethods::PUT
                : method == "PATCH"  ? HttpMethods::PATCH
                : method == "DELETE" ? HttpMethods::DELETE
                                     : HttpMethods::UNKNOWN);

  req.path = split_line[1];
  req.version = split_line[2];
}

void parse_headers(std::vector<std::string> &tokens, HttpRequest &req) {
  for (int i = 1; i < tokens.size(); i++) {
    if (tokens[i] == "")
      break;
    size_t partition = tokens[i].find(":");
    if (partition == std::string::npos)
      continue;
    std::string key = tokens[i].substr(0, partition);
    std::string value = tokens[i].substr(partition + 1);
    req.headers[key] = value;
  }
}

void parse_body(std::vector<std::string> &tokens, HttpRequest &req) {
  bool flag = false;
  std::string body = "";
  for (std::string &token : tokens) {
    if (flag)
      body.append(token);
    if (token == "")
      flag = true;
  }
  req.body = body;
}

HttpRequest parse_tokens(std::string buffer) {

  auto tokens = split_str(buffer, "\r\n");
  // build HttpRequest from tokens
  HttpRequest req;
  parse_request_line(tokens, req);
  parse_headers(tokens, req);
  parse_body(tokens, req);

  return req;
}
