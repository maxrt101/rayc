#ifndef _RAYC_SCRIPT_DRIVER_H_
#define _RAYC_SCRIPT_DRIVER_H_ 1

#include <rayc/script/ast.h>
#include <rayc/script/parser.h>
#include <rayc/script/command.h>

#include <functional>
#include <exception>
#include <map>

namespace rayc {
namespace script {

struct ExecutionException : public std::runtime_error {
  ExecutionException(const std::string& msg);
};

struct ExecutionContext {
  Environment& env;
  std::function<void(const std::string&)> printer;
};

void run(ExecutionContext* ctx, ast::Node* ast);
void run(ExecutionContext* ctx, const std::string& source);

} /* namespace script */
} /* namespace rayc */

#endif /* _RAYC_SCRIPT_DRIVER_H_ */