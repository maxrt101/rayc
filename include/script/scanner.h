#ifndef _RAYC_SCRIPT_SCANNER_H_
#define _RAYC_SCRIPT_SCANNER_H_ 1

#include <string>
#include <vector>

#include <rayc/script/ast.h>

namespace rayc {
namespace script {

enum TokenType {
  TOKEN_EOF = 0,      // EOF

  TOKEN_LEFT_PAREN,   // '('
  TOKEN_RIGHT_PAREN,  // ')'
  TOKEN_LEFT_BRACE,   // '{'
  TOKEN_RIGHT_BRACE,  // '}'
  TOKEN_SEMICOLON,    // ';'
  TOKEN_PIPE,         // '|'
  TOKEN_AMPERSAND,    // '&'
  TOKEN_DOLLAR,       // '$'
  TOKEN_STRING,       // \".*\"
  TOKEN_WORD,
  TOKEN_FN,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_WHILE
};

struct Token {
  TokenType type;
  std::string str;
  int line, ch;
};

struct ScannerException : public std::runtime_error {
  ScannerException(const std::string& msg);
};

std::vector<Token> splitTokens(const std::string& str);

} /* namespace script */
} /* namespace rayc */

#endif /* _RAYC_SCRIPT_SCANNER_H_ */