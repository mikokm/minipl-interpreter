#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <string>
#include <vector>

enum class TokenType {
  Delimiter,
  Identifier,
  Integer,
  Keyword,
  Operator,
  TypeDecl,
};

const char* TokenTypeToString(TokenType type);

struct Token {
  TokenType type_;
  std::string text_;

  Token(TokenType type, std::string text) : type_(type), text_(text) {}
  Token(TokenType type, char c) : type_(type) { text_ += c; }

  void print();
};

class Scanner {
 public:
  Scanner(const std::string& filename);

  std::vector<Token> get_tokens();

 private:
  std::string file_contents_;
};

#endif  // SCANNER_H
