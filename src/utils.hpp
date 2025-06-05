#pragma once

#include "models.hpp"
#include <string>

HttpRequest parse_tokens(std::string request);
bool str_ends_with(std::string &str, std::string pattern);
