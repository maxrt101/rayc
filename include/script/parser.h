#ifndef _RAYC_SCRIPT_PARSER_H_
#define _RAYC_SCRIPT_PARSER_H_ 1

#include <rayc/script/ast.h>
#include <rayc/script/scanner.h>

#include <vector>

namespace rayc {
namespace script {

struct ParserException : public std::runtime_error {
  ParserException(const std::string& msg);
};

ast::Node* createAst(std::vector<Token> tokens);

} /* namespace script */
} /* namespace rayc */

#endif /* _RAYC_SCRIPT_PARSER_H_ */