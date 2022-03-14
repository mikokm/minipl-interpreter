#ifndef AST_H
#define AST_H

#include <map>
#include <string>
#include <vector>

enum class SymbolType {
  Integer,
  String,
  Boolean,
  Unknown,
};

SymbolType symbol_type_from_string(const std::string& str);
std::string symbol_type_to_string(SymbolType type);

struct Symbol {
  SymbolType type_;
  std::string data_;

  int to_int() const;
  std::string to_string() const;
  bool to_bool() const;

  Symbol operator+(const Symbol& rhs);
  Symbol operator-(const Symbol& rhs);
  Symbol operator*(const Symbol& rhs);
  Symbol operator/(const Symbol& rhs);
  Symbol operator&(const Symbol& rhs);
  Symbol operator==(const Symbol& rhs);
  Symbol operator<(const Symbol& rhs);
  Symbol operator!();

  Symbol() : type_(SymbolType::Unknown) {}
  Symbol(SymbolType type, const std::string& data) : type_(type), data_(data) {}

  explicit Symbol(int data);
  explicit Symbol(const std::string& data);
  explicit Symbol(bool data);
};

struct Context {
  std::map<std::string, Symbol> symbols_;
};

struct Node {
  virtual void evaluate(Context& ctx) = 0;
};

struct ListNode : public Node {
  void evaluate(Context& ctx) override;
  std::vector<Node*> children_;
};

struct ExprNode;

struct VarNode : public Node {
  void evaluate(Context& ctx) override;
  std::string id_;
  SymbolType type_;
  ExprNode* expr_ = nullptr;
};

struct PrintNode : public Node {
  void evaluate(Context& ctx) override;
  ExprNode* expr_ = nullptr;
};

struct ReadNode : public Node {
  void evaluate(Context& ctx) override;
  std::string id_;
};

struct AssertNode : public Node {
  void evaluate(Context& ctx) override;
  ExprNode* expr_ = nullptr;
};

struct OpNode : public Node {
  void evaluate(Context& ctx) override;
  std::string op_;
  ExprNode* expr_ = nullptr;
};

struct ExprNode : public Node {
  void evaluate(Context& ctx) override;
  std::string value_;
  SymbolType type_;

  OpNode* tail_ = nullptr;
  bool negate_ = false;
  bool is_identifier_ = false;

  Symbol result_;
};

struct ForNode : public Node {
  void evaluate(Context& ctx) override;
  std::string id_;
  ExprNode* start_ = nullptr;
  ExprNode* end_ = nullptr;
  ListNode* stmts_ = nullptr;
};

#endif  // AST_H
