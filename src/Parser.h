#ifndef PARSER_H
#define PARSER_H

#include <cassert>

#include "Scanner.h"

class Parser {
 public:
  Parser(const std::vector<Token>& tokens) : tokens_(tokens) {
    current_ = tokens_.begin();
    end_ = tokens_.end();
  }

  void parse();

 protected:
  void assign();
  void expr();
  void opnd();
  void opnd_tail();
  void stmt();
  void stmts();
  void var_type();

  void parse_error();

  bool match(const std::string& text) const {
    return current_token().match(text);
  }

  void expect_match(const std::string& text) {
    assert(match(text));
    next_token();
  }

  bool match_type(const TokenType type) {
    return current_token().type() == type;
  }

  void expect_type(const TokenType type) {
    assert(match_type(type));
    next_token();
  }

  TokenType current_type() const { return current_token().type(); }

 private:
  Token current_token() const {
    assert(has_next());
    return *current_;
  };

  bool has_next() const { return current_ != end_; }

  void next_token() {
    std::cout << "Current token: ";
    current_token().print();
    std::cout << std::endl;

    assert(has_next());
    ++current_;
  }

  std::vector<Token> tokens_;
  std::vector<Token>::iterator current_, end_;
};

#endif  // PARSER_H
