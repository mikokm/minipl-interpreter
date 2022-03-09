#include <iostream>
#include <string>

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
  for (Token t : scanner.get_tokens()) {
    t.print();
  }

  return 0;
}
