#include "Scanner.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <set>

const char* token_type_to_string(TokenType type) {
  switch (type) {
    case TokenType::Delimiter:
      return "Delimiter";
    case TokenType::Identifier:
      return "Identifier";
    case TokenType::Integer:
      return "Integer";
    case TokenType::Keyword:
      return "Keyword";
    case TokenType::Operator:
      return "Operator";
    case TokenType::TypeDecl:
      return "TypeDecl";
    case TokenType::EndOfFile:
      return "EndOfFile";
    default:
      return "Unknown";
  }
}

void Token::print() {
  std::cout << "(" << token_type_to_string(type()) << ",\"" << text() << "\")";
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
  return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool is_digit(char c) { return '0' <= c && c <= '9'; }

const std::set<std::string> kKeywords = {"var",    "for",  "end",   "in",
                                         "do",     "read", "print", "int",
                                         "string", "bool", "assert"};

bool is_keyword(const std::string& str) { return kKeywords.count(str) == 1; }

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
      assert(state == BufferState::None || state == BufferState::Integer);

      state = BufferState::Integer;
      buf += c;
    } else if (state == BufferState::Integer) {
      tokens.push_back(Token(TokenType::Integer, buf));
      buf.clear();
      state = BufferState::None;
    }

    if (Tokenizer::is_letter(c)) {
      assert(state == BufferState::None || state == BufferState::String);

      state = BufferState::String;
      buf += c;
    } else if (state == BufferState::String) {
      const bool is_keyword = Tokenizer::is_keyword(buf);
      tokens.push_back(
          Token(is_keyword ? TokenType::Keyword : TokenType::Identifier, buf));
      buf.clear();
      state = BufferState::None;
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

  tokens.push_back(Token(TokenType::EndOfFile, std::string()));
  return tokens;
}
