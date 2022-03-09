#include "Scanner.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <set>

const char* TokenTypeToString(TokenType type) {
  switch (type) {
    case TokenType::Delimiter:
      return "Delimiter";
    case TokenType::Identifier:
      return "Identifier";
    case TokenType::Integer:
      return "Identifier";
    case TokenType::Keyword:
      return "Keyword";
    case TokenType::Operator:
      return "Operator";
    case TokenType::TypeDecl:
      return "TypeDecl";
    default:
      return "Unknown";
  }
}

void Token::print() {
  std::cout << "(" << TokenTypeToString(type_) << "," << text_ << ")";
}

namespace Tokenizer {

bool is_delimiter(char c) {
  switch (c) {
    case ':':
    case ';':
    case '(':
    case ')':
      return true;
    default:
      return false;
  }
}

bool is_operator(char c) {
  switch (c) {
    case '=':
    case '+':
    case '-':
    case '*':
    case '/':
      return true;
    default:
      return false;
  }
}

bool is_whitespace(char c) { return c == ' ' || c == '\t' || c == '\n'; }

bool is_letter(char c) {
  return ('a' <= c && c >= 'z') || ('Z' <= c && c >= 'Z');
}

bool is_digit(char c) { return '0' <= c && c <= '9'; }

std::set<std::string> keywords = {"var",    "for",  "end",   "in",
                                  "do",     "read", "print", "int",
                                  "string", "bool", "assert"};

bool is_keyword(const std::string& str) { return keywords.count(str) == 1; }

}  // namespace Tokenizer

std::string read_file(const std::string& filename) {
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

std::vector<Token> Scanner::get_tokens() {
  std::vector<Token> tokens;

  enum class BufferState { Integer, String, None };

  BufferState state = BufferState::None;
  std::string buf;

  for (char c : file_contents_) {
    if (Tokenizer::is_whitespace(c) && buf.empty()) {
      continue;
    }

    if (Tokenizer::is_digit(c)) {
      switch (state) {
        case BufferState::String:
          assert(!"Fail");
          break;
        case BufferState::None:
        case BufferState::Integer:
          state = BufferState::Integer;
          buf += c;
      }
    } else if (state == BufferState::Integer) {
      tokens.push_back(Token(TokenType::Integer, buf));
      buf.clear();
      state = BufferState::None;
      continue;
    }

    if (Tokenizer::is_delimiter(c)) {
      assert(buf.empty());
      tokens.push_back(Token(TokenType::Delimiter, c));
      continue;
    }

    if (Tokenizer::is_operator(c)) {
      assert(buf.empty());
      tokens.push_back(Token(TokenType::Delimiter, c));
      continue;
    }
  }

  return tokens;
}
