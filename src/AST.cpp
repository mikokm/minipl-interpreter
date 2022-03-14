#include "AST.h"

#include <cassert>
#include <iostream>
#include <sstream>

#include "Utils.h"

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

std::string symbol_type_to_string(SymbolType type) {
  switch (type) {
    case SymbolType::Integer:
      return "Integer";
    case SymbolType::Boolean:
      return "Boolean";
    case SymbolType::String:
      return "String";
    case SymbolType::Unknown:
      return "Unknown";
  }

  return "Unknown";
}

Symbol::Symbol(int data) : type_(SymbolType::Integer) {
  std::stringstream ss;
  ss << data;
  ss >> data_;
}

Symbol::Symbol(const std::string& data)
    : type_(SymbolType::String), data_(data) {}

Symbol::Symbol(bool data) : type_(SymbolType::Boolean) {
  data_ = data ? "true" : "false";
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
  return data_ == "true";
}

Symbol Symbol::operator+(const Symbol& rhs) {
  switch (type_) {
    case SymbolType::Integer:
      return Symbol(to_int() + rhs.to_int());
    case SymbolType::String:
      return Symbol(to_string() + rhs.to_string());
    default:
      break;
  }

  assert(!"Addition with unsupported type");
  return Symbol{};
}

Symbol Symbol::operator-(const Symbol& rhs) {
  assert(type_ == SymbolType::Integer);
  return Symbol(to_int() - rhs.to_int());
}

Symbol Symbol::operator*(const Symbol& rhs) {
  assert(type_ == SymbolType::Integer);
  return Symbol(to_int() * rhs.to_int());
}

Symbol Symbol::operator/(const Symbol& rhs) {
  assert(type_ == SymbolType::Integer);
  return Symbol(to_int() / rhs.to_int());
}

Symbol Symbol::operator&(const Symbol& rhs) {
  assert(type_ == SymbolType::Boolean);
  return Symbol(to_bool() & rhs.to_bool());
}

Symbol Symbol::operator==(const Symbol& rhs) {
  return Symbol(data_ == rhs.data_);
}

Symbol Symbol::operator<(const Symbol& rhs) {
  assert(type_ == SymbolType::Integer);
  return Symbol(to_int() < rhs.to_int());
}

Symbol Symbol::operator!() {
  assert(type_ == SymbolType::Boolean);
  return data_ == "true" ? Symbol(false) : Symbol(true);
}

void ListNode::evaluate(Context& ctx) {
  LOG("ListNode (%d children)\n", children_.size());

  for (Node* node : children_) {
    node->evaluate(ctx);
  }
}

void VarNode::evaluate(Context& ctx) {
  LOG("VarNode (%s, %s)\n", id_.c_str(), symbol_type_to_string(type_));

  if (type_ == SymbolType::Unknown) {
    // Pre-existing symbol, get the type from the symbol table.
    type_ = ctx.symbols_[id_].type_;
  }

  if (expr_) {
    expr_->evaluate(ctx);
    assert(type_ == expr_->result_.type_);
    ctx.symbols_[id_] = expr_->result_;
  } else {
    // default init
    std::string value;
    switch (type_) {
      case SymbolType::Boolean:
        value = "false";
        break;
      case SymbolType::Integer:
        value = "0";
        break;
      default:
        break;
    }

    ctx.symbols_[id_] = Symbol(type_, value);
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
    if (!expr_->result_.to_bool()) {
      std::cout << "Assertion failed!" << std::endl;
    }
  }
}

void ExprNode::evaluate(Context& ctx) {
  LOG("ExprNode (%s) (is_id: %d, negate: %d)\n", value_.c_str(), is_identifier_,
      negate_);

  Symbol lhs = is_identifier_ ? ctx.symbols_[value_] : Symbol(type_, value_);
  if (negate_) {
    lhs = !lhs;
  }

  if (tail_) {
    tail_->evaluate(ctx);

    Symbol rhs = tail_->expr_->result_;
    assert(lhs.type_ == rhs.type_);

    const auto op = tail_->op_;
    if (op == "+") {
      result_ = lhs + rhs;
    } else if (op == "*") {
      result_ = lhs * rhs;
    } else if (op == "-") {
      result_ = lhs - rhs;
    } else if (op == "/") {
      result_ = lhs / rhs;
    } else if (op == "=") {
      result_ = lhs == rhs;
    } else if (op == "&") {
      result_ = lhs & rhs;
    } else if (op == "<") {
      result_ = lhs < rhs;
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

void ForNode::evaluate(Context& ctx) {
  LOG("ForNode\n");
  start_->evaluate(ctx);
  end_->evaluate(ctx);

  Symbol start = start_->result_;
  Symbol end = end_->result_;

  assert(start.type_ == SymbolType::Integer);
  assert(end.type_ == SymbolType::Integer);

  for (int i = start.to_int(); i <= end.to_int(); ++i) {
    ctx.symbols_[id_] = Symbol(i);
    stmts_->evaluate(ctx);
  }
}
