#include "AST.h"
#include "Utils.h"

#include <cassert>
#include <iostream>
#include <sstream>

SymbolType symbol_type_from_string(const std::string& str) {
  if (str == "int") {
    return SymbolType::Integer;
  } else if (str == "string") {
    return SymbolType::String;
  } else if (str == "bool") {
    return SymbolType::Boolean;
  }

  return SymbolType::Unknown;
}

Symbol::Symbol(int data) : type_(SymbolType::Integer) {
  std::stringstream ss;
  ss << data;
  ss >> data_;
}

Symbol::Symbol(const std::string& data)
    : type_(SymbolType::String), data_(data) {}

Symbol::Symbol(bool data) : type_(SymbolType::Boolean) {
  std::stringstream ss;
  ss << data;
  ss >> data_;
}

int Symbol::to_int() const {
  assert(type_ == SymbolType::Integer);
  std::stringstream ss(data_);
  int res;
  ss >> res;
  return res;
}

std::string Symbol::to_string() const {
  assert(type_ == SymbolType::String);
  return data_;
}

bool Symbol::to_bool() const {
  assert(type_ == SymbolType::Boolean);
  std::stringstream ss(data_);
  bool res;
  ss >> res;
  return res;
}

Symbol Symbol::operator+(const Symbol& aOther) {
  switch (type_) {
    case SymbolType::Integer:
      return Symbol(to_int() + aOther.to_int());
    case SymbolType::String:
      return Symbol(to_string() + aOther.to_string());
    default:
      break;
  }

  assert(!"Addition with unsupported type");
  return Symbol{};
}

Symbol Symbol::operator-(const Symbol& aOther) {
  assert(type_ == SymbolType::Integer);
  return Symbol(to_int() - aOther.to_int());
}

Symbol Symbol::operator*(const Symbol& aOther) {
  assert(type_ == SymbolType::Integer);
  return Symbol(to_int() * aOther.to_int());
}

void ListNode::evaluate(Context& ctx) {
  LOG("ListNode (%d children)\n", children_.size());

  for (Node* node : children_) {
    node->evaluate(ctx);
  }
}

void VarNode::evaluate(Context& ctx) {
  LOG("VarNode (%s, %s)\n", id_.c_str(), type_.c_str());

  SymbolType type = symbol_type_from_string(type_);

  if (expr_) {
    expr_->evaluate(ctx);
    assert(type == expr_->result_.type_);
    ctx.symbols_[id_] = expr_->result_;
  }
}

void PrintNode::evaluate(Context& ctx) {
  LOG("PrintNode\n");

  if (expr_) {
    expr_->evaluate(ctx);
    std::cout << expr_->result_.data_;
  }
}

void ReadNode::evaluate(Context& ctx) {
  LOG("ReadNode (%s)\n", id_.c_str());
  std::string str;
  std::cin >> str;

  ctx.symbols_[id_].data_ = str;
}

void AssertNode::evaluate(Context& ctx) {
  LOG("AssertNode\n");
  if (expr_) {
    expr_->evaluate(ctx);
  }
}

void ExprNode::evaluate(Context& ctx) {
  LOG("ExprNode (%s) (is_id: %d, negate: %d)\n", value_.c_str(), is_identifier_, negate_);

  Symbol lhs = is_identifier_ ? ctx.symbols_[value_] : Symbol(type_, value_);

  if (tail_) {
    tail_->evaluate(ctx);

    Symbol rhs = tail_->expr_->result_;
    assert(lhs.type_ == rhs.type_);

    const auto op = tail_->op_;
    if (op == "+") {
      result_ = rhs + lhs;
    } else if (op == "*") {
      result_ = rhs * lhs;
    }
  } else {
    result_ = lhs;
  }
}

void OpNode::evaluate(Context& ctx) {
  LOG("OpNode\n");
  if (expr_) {
    expr_->evaluate(ctx);
  }
}
