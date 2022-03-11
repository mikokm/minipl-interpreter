#ifndef PARSER_H
#define PARSER_H

#include "Scanner.h"

class Parser {
 public:
  Parser(std::vector<Token> tokens) : tokens_(tokens) {}
  void parse();

 private:
  std::vector<Token> tokens_;
};

#endif // PARSER_H
