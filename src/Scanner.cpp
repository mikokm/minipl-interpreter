#include "Scanner.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>

#include "Utils.h"

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
    case TokenType::EndOfFile:
      return "EndOfFile";
    case TokenType::String:
      return "String";
    default:
      return "Unknown";
  }
}

std::string Token::to_string() const {
	std::stringstream ss;
	ss << "(" << token_type_to_string(type()) << ",\"" << text() << "\")";
	return ss.str();
}

namespace Tokenizer {

bool is_delimiter(char c) {
  switch (c) {
    case ':':
    case ';':
    case '(':
    case ')':
    case '.':
      return true;
    default:
      return false;
  }
}

bool is_operator(char c) {
  switch (c) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '<':
    case '=':
    case '&':
    case '!':
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

struct TokenBuffer {
  void push(char c) {
    switch (state_) {
      case State::Escape:
        switch (c) {
          case 'n':
            buffer_ += '\n';
            break;
          case 't':
            buffer_ += '\t';
            break;
          case '"':
            buffer_ += '"';
            break;
          default:
            LOG_ERROR("Unexpected escape sequence \\%c\n", c);
            break;
        }
        state_ = State::StringLiteral;
        break;
      case State::StringLiteral:
        if (c == '"') {
          AddToken(Token(TokenType::String, take_buffer()));
          state_ = State::None;
          return;
        } else if (c == '\\') {
          state_ = State::Escape;
          return;
        } else {
          buffer_ += c;
        }
        break;
      case State::Integer:
        if (Tokenizer::is_digit(c)) {
          buffer_ += c;
        } else {
          AddToken(Token(TokenType::Integer, take_buffer()));
          state_ = State::None;
        }
        break;
      case State::Identifier:
        if (Tokenizer::is_letter(c) || Tokenizer::is_digit(c)) {
          buffer_ += c;
        } else {
          const bool is_keyword = Tokenizer::is_keyword(buffer_);
          AddToken(
              Token(is_keyword ? TokenType::Keyword : TokenType::Identifier,
                    take_buffer()));
          state_ = State::None;
        }
        break;
      default:
        break;
    }

    if (state_ == State::None) {
      if (Tokenizer::is_delimiter(c)) {
        AddToken(Token(TokenType::Delimiter, c));
      } else if (Tokenizer::is_operator(c)) {
        AddToken(Token(TokenType::Operator, c));
      } else if (Tokenizer::is_digit(c)) {
        buffer_ += c;
        state_ = State::Integer;
      } else if (Tokenizer::is_letter(c)) {
        buffer_ += c;
        state_ = State::Identifier;
      } else if (c == '"') {
        state_ = State::StringLiteral;
      }
    }
  }

  std::string take_buffer() {
    std::string buffer = buffer_;
    buffer_.clear();
    return buffer;
  }

  void AddToken(Token token) {
		LOG("Found token %s\n", token.to_string().c_str());
    tokens_.push_back(token);
  }

  enum class State { Escape, Integer, Identifier, StringLiteral, None };

  State state_ = State::None;
  std::string buffer_;
  std::vector<Token> tokens_;
};

std::vector<Token> Scanner::get_tokens() {
  TokenBuffer buffer;

  for (char c : file_contents_) {
    buffer.push(c);
  }

  buffer.AddToken(Token(TokenType::EndOfFile, std::string()));
  return buffer.tokens_;
}
