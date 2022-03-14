#ifndef PARSER_H
#define PARSER_H

#include <cassert>
#include <map>

#include "AST.h"
#include "Scanner.h"
#include "Utils.h"

class Parser {
 public:
  Parser(const std::vector<Token>& tokens) : tokens_(tokens) {
    current_ = tokens_.begin();
    end_ = tokens_.end();
  }

  void parse();

 protected:
  ExprNode* assign();
  ExprNode* expr();
  ExprNode* opnd();
  OpNode* opnd_tail();
  Node* stmt();
  Node* stmts();

  void parse_error();

  bool match(const std::string& text) const {
    return current_token().match(text);
  }

  void expect_match(const std::string& text) {
    assert(match(text));
    next_token();
  }

  bool match_type(const TokenType type) {
    return current_token().match_type(type);
  }

  void expect_type(const TokenType type) {
    assert(match_type(type));
    next_token();
  }

  TokenType current_type() const { return current_token().type(); }
  const std::string& current_text() const { return current_token().text(); }

 private:
  const Token& current_token() const {
    assert(has_next());
    return *current_;
  };

  bool has_next() const { return current_ != end_; }

  void next_token() {
		LOG("Current token: %s\n",
				current_token().to_string().c_str());

    assert(has_next());
    ++current_;
  }

  std::vector<Token> tokens_;
  std::vector<Token>::iterator current_, end_;
};

#endif  // PARSER_H
