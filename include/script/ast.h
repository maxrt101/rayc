#ifndef _RAYC_SCRIPT_AST_H_
#define _RAYC_SCRIPT_AST_H_ 1

#include <string>
#include <vector>

namespace rayc {
namespace script {
namespace ast {

enum class NodeType {
  STRING,
  VAR,
  COMMAND,
  BLOCK,
  FUNCTION,
  IF,
  WHILE,
};

struct Node {
  NodeType type;

  Node(NodeType t);
  virtual ~Node();
};

struct String : public Node {
  std::string value;

  String(const std::string& val);
  ~String();
};

struct Var : public Node {
  std::string name;

  Var(const std::string& val);
  ~Var();
};

struct Command : public Node {
  std::vector<Node*> tokens;

  Command(const std::vector<Node*>& val);
  ~Command();
};

struct Block : public Node {
  std::vector<Node*> commands;

  Block(const std::vector<Node*>& val);
  ~Block();
};

struct Function : public Node {
  std::string name;
  Node* body = nullptr;

  Function(const std::string& name, Node* body);
  ~Function();
};

struct If : public Node {
  Node* condition = nullptr;
  Node* thenBody = nullptr;
  Node* elseBody = nullptr;

  If(Node* condition, Node* thenBody, Node* elseBody = nullptr);
  ~If();
};

struct While : public Node {
  Node* condition = nullptr;
  Node* body = nullptr;

  While(Node* condition, Node* body);
  ~While();
};

} /* namespace ast */

void freeAst(ast::Node* ast, bool excludeFunctions = true);
void printAst(ast::Node* ast);

} /* namespace script */
} /* namespace rayc */

#endif /* _RAYC_SCRIPT_AST_H_ */