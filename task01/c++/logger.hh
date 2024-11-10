#pragma once
#include <iostream>

class Logger {
public:
  Logger() = default;
  static void print(std::string const &info, bool new_line = true);
  static void printStartInfo();
};