#include <rayc/script/ast.h>

using namespace rayc::script::ast;

rayc::script::ast::Node::Node(NodeType t)
  : type(t) {}

rayc::script::ast::Node::~Node() {}

rayc::script::ast::String::String(const std::string& val)
  : Node(NodeType::STRING), value(val) {}

rayc::script::ast::String::~String() {}

rayc::script::ast::Var::Var(const std::string& val)
  : Node(NodeType::VAR), name(val) {}

rayc::script::ast::Var::~Var() {}

rayc::script::ast::Command::Command(const std::vector<Node*>& val)
  : Node(NodeType::COMMAND), tokens(val) {}

rayc::script::ast::Command::~Command() {}

rayc::script::ast::Block::Block(const std::vector<Node*>& val)
  : Node(NodeType::BLOCK), commands(val) {}

rayc::script::ast::Block::~Block() {}

rayc::script::ast::Function::Function(const std::string& name, Node* body)
  : Node(NodeType::FUNCTION), name(name), body(body) {}

rayc::script::ast::Function::~Function() {}

rayc::script::ast::If::If(Node* condition, Node* thenBody, Node* elseBody)
  : Node(NodeType::IF), condition(condition), thenBody(thenBody), elseBody(elseBody) {}

rayc::script::ast::If::~If() {}

rayc::script::ast::While::While(Node* condition, Node* body)
  : Node(NodeType::WHILE), condition(condition), body(body) {}

rayc::script::ast::While::~While() {}

void rayc::script::freeAst(Node* ast, bool excludeFunctions) {
  if (!ast) return;

  switch (ast->type) {
    case NodeType::STRING:
    case NodeType::VAR:
      delete ast;
      return;
    case NodeType::COMMAND:
      for (auto& node : ((Command*) ast)->tokens) {
        freeAst(node);
      }
      delete ast;
      return;
    case NodeType::BLOCK:
      for (auto& node : ((Block*) ast)->commands) {
        freeAst(node);
      }
      delete ast;
      return;
    case NodeType::FUNCTION:
      if (!excludeFunctions) {
        freeAst(((Function*) ast)->body);
        delete ast;
      }
      return;
    case NodeType::IF: {
        If* ifNode = (If*) ast;
        freeAst(ifNode->condition);
        freeAst(ifNode->thenBody);
        freeAst(ifNode->elseBody);
        delete ast;
        return;
      }
    case NodeType::WHILE: {
        While* whileNode = (While*) ast;
        freeAst(whileNode->condition);
        freeAst(whileNode->body);
        delete ast;
        return;
      }
  }
}

// Very bad
void _printAst(Node* ast, std::string ident, bool inlineBlock = false, bool inlineCommand = false, bool inlineBlockStart = false, bool inlineBlockEnd = false) {
  if (!ast) return;

  switch (ast->type) {
    case NodeType::STRING:
      printf("%s", ((String*) ast)->value.c_str());
      return;
    case NodeType::VAR:
      printf("$%s", ((Var*) ast)->name.c_str());
      return;
    case NodeType::COMMAND: {
        Command* cmd = (Command*) ast;
        if (inlineCommand) {
          printf("$(");
        } else {
          printf("%s", ident.c_str());
        }
        for (int i = 0; i < cmd->tokens.size(); i++) {
          _printAst(cmd->tokens[i], "", true, true, true, true);
          if (i + 1 < cmd->tokens.size()) {
            printf(" ");
          }
        }
        if (inlineCommand) {
          printf(")");
        }
        return;
      }
    case NodeType::BLOCK: {
        if (inlineBlock) {
          for (auto& node : ((Block*) ast)->commands) {
            _printAst(node, "", inlineBlock);
            printf("; ");
          }
        } else {
          if (!inlineBlockStart) {
            printf("%s", ident.c_str());
          }
          printf("{\n");
          for (auto& node : ((Block*) ast)->commands) {
            _printAst(node, ident + "  ", inlineBlock);
            printf("\n");
          }
          printf("%s}", ident.c_str());
          if (!inlineBlockEnd) {
            printf("\n");
          }
        }
        return;
      }
    case NodeType::FUNCTION: {
        Function* fn = (Function*) ast;
        printf("%sfn %s ", ident.c_str(), fn->name.c_str());
        _printAst(fn->body, ident + "  ", inlineBlock, false, true);
        return;
      }
    case NodeType::IF: {
        If* ifNode = (If*) ast;
        printf("%sif ", ident.c_str());
        _printAst(ifNode->condition, "", inlineBlock, true, true);
        printf("; ");
        _printAst(ifNode->thenBody, ident, inlineBlock, false, true, true);
        if (ifNode->elseBody) {
          printf(" else ");
          _printAst(ifNode->elseBody, ident, inlineBlock, false, true);
        }
        return;
      }
    case NodeType::WHILE: {
        While* whileNode = (While*) ast;
        printf("%swhile ", ident.c_str());
        _printAst(whileNode->condition, "", inlineBlock, true, true);
        printf("; ");
        _printAst(whileNode->body, ident, inlineBlock, false, true);
        return;
      }
  }
}

void rayc::script::printAst(Node* ast) {
  _printAst(ast, "", false, false, false);
}
