#include "Parser.h"

ExprNode* Parser::assign() {
  expect_match(":");
  expect_match("=");
  return expr();
}

ExprNode* Parser::expr() {
  if (match("!")) {
    next_token();
    ExprNode* node = opnd();
    node->negate_ = true;
    return node;
  } else {
    ExprNode* node = opnd();
    node->tail_ = opnd_tail();
    return node;
  }
}

ExprNode* Parser::opnd() {
  switch (current_type()) {
    case TokenType::Delimiter:
      if (match("(")) {
        next_token();
        auto* node = expr();
        expect_match(")");
        return node;
      } else {
        parse_error();
      }
      break;
    case TokenType::Integer: {
      auto* node = new ExprNode();
      node->value_ = current_text();
      node->type_ = SymbolType::Integer;
      next_token();
      return node;
    }
    case TokenType::String: {
      auto* node = new ExprNode();
      node->value_ = current_text();
      node->type_ = SymbolType::String;
      next_token();
      return node;
    }
    case TokenType::Identifier: {
      auto* node = new ExprNode();
      node->value_ = current_text();
      node->type_ = SymbolType::Unknown;
      node->is_identifier_ = true;
      next_token();
      return node;
    }
    default:
      parse_error();
  }

  parse_error();
  return nullptr;
}

OpNode* Parser::opnd_tail() {
  if (match_type(TokenType::Operator)) {
    auto* node = new OpNode();
    node->op_ = current_text();
    next_token();
    node->expr_ = opnd();
    return node;
  }

  return nullptr;
}

Node* Parser::stmt() {
  if (current_type() == TokenType::Identifier) {
    VarNode* node = new VarNode();
    node->id_ = current_text();
    node->type_ = SymbolType::Unknown;
    next_token();
    node->expr_ = assign();
    return node;
  }

  if (current_type() == TokenType::Keyword) {
    if (match("var")) {
      next_token();

      VarNode* node = new VarNode();
      node->id_ = current_text();
      expect_type(TokenType::Identifier);
      expect_match(":");

      if (match("int") || match("string") || match("bool")) {
        node->type_ = symbol_type_from_string(current_text());
        next_token();
      } else {
        parse_error();
      }

      if (match(":")) {
        node->expr_ = assign();
      }

      return node;
    }

    if (match("for")) {
      next_token();

      ForNode* node = new ForNode();
      node->id_ = current_text();
      expect_type(TokenType::Identifier);
      expect_match("in");
      node->start_ = expr();
      expect_match(".");
      expect_match(".");
      node->end_ = expr();
      expect_match("do");
      node->stmts_ = stmts();
      expect_match("end");
      expect_match("for");
      return node;
    }

    if (match("read")) {
      next_token();

      ReadNode* node = new ReadNode();
      node->id_ = current_text();
      expect_type(TokenType::Identifier);

      return node;
    }

    if (match("print")) {
      next_token();

      PrintNode* node = new PrintNode();
      node->expr_ = expr();

      return node;
    }

    if (match("assert")) {
      next_token();

      AssertNode* node = new AssertNode();
      expect_match("(");
      node->expr_ = expr();
      expect_match(")");

      return node;
    }
  }

  parse_error();
  return nullptr;
}

ListNode* Parser::stmts() {
  ListNode* listNode = new ListNode();

  while (!match_type(TokenType::EndOfFile) && !match("end")) {
    // Next symbol starts stmt
    Node* node = stmt();
    listNode->children_.push_back(node);
    expect_match(";");
  }

  return listNode;
}

void Parser::parse() {
  assert(has_next());
  Node* node = stmts();
  assert(current_type() == TokenType::EndOfFile);

  Context ctx;
  node->evaluate(ctx);
}

void Parser::parse_error() {
  LOG_ERROR("Parsing error encountered\n");
}
