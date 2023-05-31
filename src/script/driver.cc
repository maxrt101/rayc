#include <rayc/script/driver.h>
#include <rayc/script/parser.h>
#include <rayc/script/scanner.h>
#include <rayc/script/command.h>
#include <rayc/log.h>

using namespace rayc::script;
using namespace rayc::script::ast;


static struct GlobalExecutionState {
  std::map<std::string, Function*> functions;
  std::map<std::string, rayc::Command> commands;
} state;


static void processAstNode(ExecutionContext* ctx, Node* node);


rayc::script::ExecutionException::ExecutionException(const std::string& msg)
  : std::runtime_error("Execution Exception: " + msg) {}


void rayc::registerCommand(const std::string& name, Command command) {
  state.commands[name] = command;
}


static rayc::CommandResult runCommand(ExecutionContext* ctx, const std::vector<std::string>& args) {
  if (ctx && args.size()) {
    if (state.commands.find(args[0]) != state.commands.end()) {
      return state.commands[args[0]](ctx->env, args);
    } else if (state.functions.find(args[0]) != state.functions.end()) {
      std::string result; 
      rayc::Environment env = ctx->env;

      ExecutionContext funcCtx {env, [&result](const std::string& s) { result += s + "\n"; }};

      for (size_t i = 1; i < args.size(); i++) {
        funcCtx.env["#" + std::to_string(i)] = args[i];
      }
      
      processAstNode(&funcCtx, state.functions[args[0]]->body);
      
      for (auto& [var, val] : funcCtx.env) {
        if (var.size() && var[0] != '#') {
          ctx->env[var] = val;
        }
      }

      return {0, result};
    }
  }
  return {1, "No such command"};
}


static rayc::CommandResult runCommand(ExecutionContext* ctx, ast::Command* cmd) {
  std::vector<std::string> args;
  for (auto& token : cmd->tokens) {
    switch (token->type) {
      case NodeType::STRING:
        args.push_back(((String*)token)->value);
        break;
      case NodeType::VAR:
        args.push_back(ctx->env[((Var*)token)->name]);
        break;
      case NodeType::COMMAND:
        args.push_back(runCommand(ctx, (Command*) token).output);
        break;
      default:
        // rayc::error("Unexpected node in command (%d)", token->type);
        throw ExecutionException("Unexpected node in command (" + std::to_string((int) token->type) + ")");
    }
  }

  rayc::CommandResult result = {0, ""};

  if (args[0] == "help") {
    for (auto& [cmdName, _] : state.commands) {
      result.output += cmdName + " ";
    }
  } else { 
    result = runCommand(ctx, args);
  }

  ctx->env["?"] = std::to_string(result.returnCode);

  return result;
}


static void processAstNode(ExecutionContext* ctx, Node* node) {
  if (!node) return;

  switch (node->type) {
    case NodeType::STRING:
    case NodeType::VAR:
      // rayc::error("Unexpected node outside of command (%d)", node->type);
      throw ExecutionException("Unexpected node outside of command (" + std::to_string((int) node->type) + ")");
      // break;
    case NodeType::COMMAND: {
      auto result = runCommand(ctx, (Command*) node);
      ctx->printer(result.output);
      break;
    }
    case NodeType::BLOCK: {
      Block* block = (Block*) node;
      for (auto& node : block->commands) {
        processAstNode(ctx, node);
      }
      break;
    }
    case NodeType::FUNCTION: {
      Function* f = (Function*) node;
      state.functions[f->name] = f;
      break;
    }
    case NodeType::IF: {
      If* ifNode = (If*) node;
      if (!ifNode->condition || ifNode->condition->type != NodeType::COMMAND) {
        throw ExecutionException("no condition in 'if'");
      }
      if (runCommand(ctx, (Command*) ifNode->condition).returnCode == 0) {
        if (ifNode->thenBody) {
          processAstNode(ctx, ifNode->thenBody);
        }
      } else {
        if (ifNode->elseBody) {
          processAstNode(ctx, ifNode->elseBody);
        }
      }
      break;
    }
    case NodeType::WHILE: {
        While* whileNode = (While*) node;
        if (!whileNode->condition || whileNode->condition->type != NodeType::COMMAND) {
          throw ExecutionException("no condition in 'while'");
        }
        while (runCommand(ctx, (Command*) whileNode->condition).returnCode == 0) {
          if (whileNode->body) {
            processAstNode(ctx, whileNode->body);
          }
        }
        break;
      }
    default:
      break;
  }
}

void rayc::script::run(ExecutionContext* ctx, Node* ast) {
  processAstNode(ctx, ast);
}

void rayc::script::run(ExecutionContext* ctx, const std::string& source) {
  try {
    auto ast = createAst(splitTokens(source));
#ifdef _SCRIPT_DEBUG
    printAst(ast);
#endif
    run(ctx, ast);
    freeAst(ast);
  } catch (ScannerException e) {
    error("%s", e.what());
  } catch (ParserException e) {
    error("%s", e.what());
  } catch (ExecutionException e) {
    error("%s", e.what());
  }
}

