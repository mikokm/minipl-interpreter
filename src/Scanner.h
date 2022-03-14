#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <string>
#include <vector>

enum class TokenType {
  Delimiter,
  Identifier,
  Integer,
  String,
  Keyword,
  Operator,
  EndOfFile,
};

const char* token_type_to_string(TokenType type);

class Token {
 public:
  Token(TokenType type, std::string text) : type_(type), text_(text) {}
  Token(TokenType type, char c) : type_(type) { text_ += c; }

  std::string to_string() const;
  TokenType type() const { return type_; }
  const std::string& text() const { return text_; }

  bool match(const std::string& text) const { return text_ == text; }
  bool match_type(TokenType type) const { return type_ == type; }

 private:
  TokenType type_;
  std::string text_;
};

class Scanner {
 public:
  Scanner(const std::string& filename);

  std::vector<Token> get_tokens();

 private:
  std::string file_contents_;
};

#endif  // SCANNER_H
