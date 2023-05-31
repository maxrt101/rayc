#include <rayc/script/scanner.h>

#include <cstring>
#include <cctype>

using namespace rayc::script;

static struct ScannerContext {
  const char* src;
  const char* start;
  const char* current;
  int line;
} scanner;

rayc::script::ScannerException::ScannerException(const std::string& msg)
  : std::runtime_error("Scanner Exception: " + msg) {}

static bool isAtEnd() {
  return *scanner.current == '\0';
}

static void skipToCurrent() {
  scanner.start = scanner.current;
}

static Token makeToken(TokenType type) {
  return Token {
    type, std::string(scanner.start, scanner.current - scanner.start),
    scanner.line, (int)(scanner.start - scanner.src)
  };
}

static char advance() {
  return *(scanner.current++);
}

static bool match(char expected) {
  if (isAtEnd()) return false;
  if (*scanner.current != expected) return false;

  advance();
  return true;
}

static char peek() {
  return *scanner.current;
}

static char peekNext() {
  return scanner.current[1];
}

static void skipWhitespace() {
  while (1) {
    switch (peek()) {
      case '\n':
        scanner.line++;
      case ' ':
      case '\r':
      case '\t':
        advance();
        break;
      case '\\':
        advance();
        advance();
        break;
      default:
        return;
    }
  }
}

static Token quotedString(char quote) {
  skipToCurrent();

  while (peek() != quote && !isAtEnd()) {
    advance();
  }

  if (isAtEnd()) throw ScannerException("Unterminated string");

  Token token = makeToken(TOKEN_STRING);
  advance();
  skipToCurrent();
  return token;
}

static Token word() {
  if (!strncmp("fn", scanner.start, 2)) {
    scanner.current += 1;
    return makeToken(TOKEN_FN);
  } else if (!strncmp("if", scanner.start, 2)) {
    scanner.current += 1;
    return makeToken(TOKEN_IF);
  } else if (!strncmp("else", scanner.start, 4)) {
    scanner.current += 3;
    return makeToken(TOKEN_ELSE);
  } else if (!strncmp("while", scanner.start, 5)) {
    scanner.current += 4;
    return makeToken(TOKEN_WHILE);
  }

  char c = peek();
  while (!isAtEnd()
    && c != ' ' && c != '\n' && c != '\t'
    && c != '|' && c != '&'  && c != ';'
    && c != '(' && c != ')'
    && c != '{' && c != '}'
  ) {
    advance();
    c = peek();
  }

  return makeToken(TOKEN_WORD);
}

static Token scanToken() {
  skipWhitespace();
  skipToCurrent();

  if (isAtEnd()) return makeToken(TOKEN_EOF);

  char c = advance();

  if (isalpha(c)) return word();

  switch (c) {
    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
    case '{': return makeToken(TOKEN_LEFT_BRACE);
    case '}': return makeToken(TOKEN_RIGHT_BRACE);
    case ';': return makeToken(TOKEN_SEMICOLON);
    case '|': return makeToken(TOKEN_PIPE);
    case '&': return makeToken(TOKEN_AMPERSAND);
    case '$': return makeToken(TOKEN_DOLLAR);
    case '"': return quotedString('"');
    case '\'': return quotedString('\'');
    case '\\':
      advance();
      advance();
    default:
      return word();
  }
}

std::vector<rayc::script::Token> rayc::script::splitTokens(const std::string& str) {
  scanner.src = str.c_str();
  scanner.start = str.c_str();
  scanner.current = str.c_str();
  scanner.line = 1;

  std::vector<rayc::script::Token> tokens;

  while (!isAtEnd()) {
    auto token = scanToken();
    // if (token.type == TOKEN_EOF) break;
    // printf("(%d '%s') ", (int) token.type, token.str.c_str());
    tokens.push_back(token);
  }

  // printf("\n");

  return tokens;
}
