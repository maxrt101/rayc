#include <rayc/script/parser.h>

using namespace rayc::script;
using namespace rayc::script::ast;

static Node* block();
static Node* subcommand();

static struct ParserContext {
  std::vector<Token> tokens;
  size_t index = 0;
} parser;

rayc::script::ParserException::ParserException(const std::string& msg)
  : std::runtime_error("Parser Exception: " + msg) {}

static bool isAtEnd() {
  return parser.index >= parser.tokens.size();
}

static Token peek(int index = 0) {
  return parser.tokens[parser.index + index];
}

static Token previous() {
  return parser.tokens[parser.index - 1];
}

static Token advance() {
  if (!isAtEnd()) parser.index++;
  return previous();
}

static bool check(TokenType type) {
  if (isAtEnd()) return false;
  return peek().type == type;
}

static Token consume(TokenType type, const std::string& msg = "Unexpected token") {
  if (check(type)) return advance();
  throw ParserException(msg);
}


static Node* var() {
  return new Var(advance().str);
}


static Node* command() {
  std::vector<Node*> cmd;
  int line = peek().line;

  while (!isAtEnd() && peek().line == line
      && peek().type != TOKEN_SEMICOLON
      && peek().type != TOKEN_RIGHT_PAREN
      && peek().type != TOKEN_RIGHT_BRACE) {
    if (peek().type == TOKEN_DOLLAR) {
      advance();
      if (peek().type == TOKEN_LEFT_PAREN) {
        advance();
        cmd.push_back(subcommand());
      } else {
        cmd.push_back(var());
      }
    } else {
      cmd.push_back(new String(peek().str));
      advance();
    }
  }

  if (!isAtEnd() && peek().type == TOKEN_SEMICOLON) {
    advance();
  }

  return new Command(cmd);
}


static Node* subcommand() {
  Node* cmd = nullptr;

  while (!isAtEnd() && peek().type != TOKEN_RIGHT_PAREN) {
    cmd = command();
  }

  if (!isAtEnd() && peek().type == TOKEN_RIGHT_PAREN) {
    advance();
  }

  return cmd;
}


static Node* fndecl() {
  consume(TOKEN_FN);

  Token name = consume(TOKEN_WORD);;

  consume(TOKEN_LEFT_BRACE);

  Node* body = block();

  consume(TOKEN_RIGHT_BRACE);

  return new Function(name.str, body);
}


static Node* ifstmt() {
  consume(TOKEN_IF);

  Node* condition = command();
  
  consume(TOKEN_LEFT_BRACE);

  Node* thenBody = block();

  consume(TOKEN_RIGHT_BRACE);

  Node* elseBody = nullptr;

  if (peek().type == TOKEN_ELSE) {
    consume(TOKEN_ELSE);
    consume(TOKEN_LEFT_BRACE);

    elseBody = block();

    consume(TOKEN_RIGHT_BRACE);
  }

  return new If(condition, thenBody, elseBody);
}


static Node* whilestmt() {
  consume(TOKEN_WHILE);

  Node* condition = command();

  consume(TOKEN_LEFT_BRACE);

  Node* body = block();

  consume(TOKEN_RIGHT_BRACE);

  return new While(condition, body);
}


static Node* block() {
  std::vector<Node*> nodes;

  while (1) {
    if (peek().type == TOKEN_RIGHT_BRACE) break;
    if (peek().type == TOKEN_IF) {
      nodes.push_back(ifstmt());
    } else if (peek().type == TOKEN_WHILE) {
      nodes.push_back(whilestmt());
    } else {
      nodes.push_back(command());
    }
  }

  return new Block(nodes);
}


rayc::script::ast::Node* rayc::script::createAst(std::vector<Token> tokens) {
  parser.tokens = tokens;
  parser.index = 0;

  std::vector<Node*> nodes;

  while (!isAtEnd()) {
    if (peek().type == TOKEN_RIGHT_BRACE || peek().type == TOKEN_RIGHT_PAREN) {
      throw ParserException("Unexpected token");
    }

    if (peek().type == TOKEN_EOF) break;
    if (peek().type == TOKEN_FN) {
      nodes.push_back(fndecl());
    } else if (peek().type == TOKEN_IF) {
      nodes.push_back(ifstmt());
    } else if (peek().type == TOKEN_WHILE) {
      nodes.push_back(whilestmt());
    } else {
      nodes.push_back(command());
    }
  }

  return new Block(nodes);
}
