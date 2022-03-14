#include <iostream>
#include <string>

#include "Parser.h"
#include "Scanner.h"

bool gLog = false;

void print_usage() {
  std::cout << "Usage: interpreter <filename> [-d]" << std::endl;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    print_usage();
    return 1;
  }

  if (argc > 2) {
    gLog = true;
  }

  LOG("Interpreting file %s\n", argv[1]);
  Scanner scanner(argv[1]);

  LOG("- Start scanning\n");
  auto tokens = scanner.get_tokens();

  LOG("- Start parsing\n");
  Parser parser(tokens);
  parser.parse();

  return 0;
}
