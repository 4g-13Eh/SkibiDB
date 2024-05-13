#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::string generateUUID();

std::string toUpperCase(std::string str);

std::string toLowerCase(std::string str);

std::string join(const std::vector<std::string> &elements, const std::string &separator);

std::string buildSkibiPath();

std::string getCurrentTimestamp();

#endif // UTILITIES_HPP
