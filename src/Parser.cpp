#include "Parser.h"

using iterator = std::vector<Token>::iterator;

void stmt(iterator begin, iterator end) {
  switch(begin->type()) {
    default: return;
  }
}

void stmts(iterator begin, iterator end) {
  stmt(begin, end);
}

void Parser::parse() {
  std::cout << "Hello world" << std::endl;

  auto begin = tokens_.begin();
  auto end = tokens_.end();

  stmts(begin, end);
}
