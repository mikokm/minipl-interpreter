#include "Scanner.h"

#include <fstream>
#include <iostream>

std::string read_file(const std::string &filename) {
  std::ifstream stream(filename);

  if (!stream.good()) {
    std::cout << "Error opening file \"" << filename << "\"" << std::endl;
    return std::string();
  }

  stream.seekg(0, std::ios::end);
  size_t size = stream.tellg();
  stream.seekg(0);

  std::string buffer(size, ' ');
  stream.read(&buffer[0], size);

  return buffer;
}

Scanner::Scanner(const std::string& filename) {
  file_contents_ = read_file(filename);
}
