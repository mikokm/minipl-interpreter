#include <iostream>
#include <string>

#include "Parser.h"
#include "Scanner.h"

void print_usage() {
  std::cout << "Usage: interpreter <filename>" << std::endl;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    print_usage();
    return 1;
  }

  Scanner scanner(argv[1]);
  auto tokens = scanner.get_tokens();

  std::cout << "-- Scanner output --" << std::endl;
  for (Token t : tokens) {
    t.print();
  }
  std::cout << std::endl << "--" << std::endl;

  std::cout << std::endl << "Parsing:" << std::endl;
  Parser parser(tokens);
  parser.parse();
  std::cout << std::endl << "--" << std::endl;

  return 0;
}
