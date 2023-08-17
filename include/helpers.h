#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <iostream>
#include <vector>
#include <string>
using namespace std;

std::vector<std::string> split_string(const std::string &str);
std::string extractJsonFromResponse(const std::string &response);

#endif