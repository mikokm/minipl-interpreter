#include "Parser.h"

void Parser::assign() {
  expect_match(":");
  expect_match("=");
  expr();
}

void Parser::var_type() {
  if (match_type(TokenType::Keyword)) {
    bool is_var_type = match("int") || match("string") || match("bool");
    assert(is_var_type);
    next_token();
  } else {
    parse_error();
  }
}

void Parser::expr() {
  if (match("!")) {
    next_token();
    opnd();
  } else {
    opnd();
    opnd_tail();
  }
}

void Parser::opnd() {
  switch (current_type()) {
    case TokenType::Delimiter:
      if (match("(")) {
        next_token();
        expr();
        expect_match(")");
      } else {
        parse_error();
      }
      break;
    case TokenType::Integer:
    case TokenType::String:
    case TokenType::Identifier:
      next_token();
      break;
    default:
      parse_error();
  }
}

void Parser::opnd_tail() {
  if (match_type(TokenType::Operator)) {
    next_token();
    opnd();
  }
}

void Parser::stmt() {
  switch (current_type()) {
    case TokenType::Identifier:
      next_token();
      assign();
      break;
    case TokenType::Keyword:
      if (match("var")) {
        next_token();
        expect_type(TokenType::Identifier);
        expect_match(":");
        var_type();
        if (match(":")) {
          assign();
        }
        break;
      }

      if (match("for")) {
        next_token();
        expect_type(TokenType::Identifier);
        expect_match("in");
        expr();
        expect_match(".");
        expect_match(".");
        expr();
        expect_match("do");
        stmts();
        expect_match("end");
        expect_match("for");
        break;
      }

      if (match("read")) {
        next_token();
        expect_type(TokenType::Identifier);
        break;
      }

      if (match("print")) {
        next_token();
        expr();
        break;
      }

      if (match("assert")) {
        next_token();
        expect_match("(");
        expr();
        expect_match(")");
      }
      break;
    default:
      parse_error();
  }
}

void Parser::stmts() {
  while (!match_type(TokenType::EndOfFile) && !match("end")) {
    // Next symbol starts stmt
    stmt();
    expect_match(";");
  }
}

void Parser::parse() {
  assert(has_next());
  while (!match_type(TokenType::EndOfFile)) {
    stmts();
  }
  assert(current_type() == TokenType::EndOfFile);
}

void Parser::parse_error() {
  std::cout << "Error encountered" << std::endl;
  abort();
}
