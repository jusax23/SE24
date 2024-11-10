#include "logger.hh"

void Logger::print(std::string const &info, bool new_line) {
  std::cout << info;
  if (new_line)
    std::cout << std::endl;
}

void Logger::printStartInfo() {
  print("SE & IT-PM: Image generator -- C++ Version");
  print("==========================================");
}