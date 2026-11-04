#include "lexer.h"
#include <cctype>
#include <string>
#include <string_view>
#include <unordered_map>

char Lexer::checkNextSymbol() {
  return (_currect + 1) < _file.size() ? _file[_currect + 1] : '\0';
}

void Lexer::goNextSymbol() {
  if (_currect < _file.size()) {
    _currect++;
    position.column++;
  }
}

char Lexer::getCurSymbol() {
  return _currect < _file.size() ? _file[_currect] : '\0';
}

char Lexer::getSymbol(int poition) {
  return poition < _file.size() ? _file[poition] : '\0';
}

inline void Lexer::scanSpace() {
  while (_currect < _file.size() &&
         std::isspace(static_cast<unsigned char>(getCurSymbol()))) {
    if (getCurSymbol() == '\n') {
      position.row++;
      position.column = 0;
    }
    goNextSymbol();
  }
}

inline Token Lexer::scanIdentifier() {
  int symbol = getCurSymbol();

  while (_currect < _file.size()) {
    symbol = getCurSymbol();

    if (std::isalnum(symbol) || symbol == '_') {
      goNextSymbol();
    } else {
      break;
    }
  }

  return stringToToken(
      std::string_view(_file).substr(_start, _currect - _start));
}

inline Token Lexer::scanNumLiteral() {
  int symbol = getCurSymbol();

  bool isDouble = false;

  while (_currect < _file.size()) {
    symbol = getCurSymbol();

    if (std::isdigit(symbol)) {
      goNextSymbol();
    } else if ((symbol == ',') && !isDouble &&
               std::isdigit(checkNextSymbol())) {
      isDouble = true;
      goNextSymbol();
    } else {
      break;
    }
  }

  Position token_position = position;
  token_position.column -= (_currect - _start);

  TokenEnum type =
      isDouble ? TokenEnum::DOUBLE_LITERAL : TokenEnum::INT_LITERAL;

  return {type, token_position,
          std::string_view(_file).substr(_start, _currect - _start)};
}

inline Token Lexer::scanStrLiteral() {
  Position token_position = position;
  goNextSymbol();

  while (_currect < _file.size() && getCurSymbol() != '"') {
    goNextSymbol();
  }

  if (_currect < _file.size()) {
    goNextSymbol();
  }

  return {TokenEnum::STRING_LITERAL, token_position,
          std::string_view(_file).substr(_start, _currect - _start)};
}

inline Token Lexer::scanSymbol() {
  goNextSymbol();
  return stringToToken(
      std::string_view(_file).substr(_start, _currect - _start));
}

Token Lexer::stringToToken(std::string_view str) {
  static const std::unordered_map<std::string_view, TokenEnum> tokenMap{
      {"let", LET},   {"fn", FN},       {"const", CONST},
      {"true", TRUE}, {"false", FALSE},

      {"+", PLUS},    {"-", MINUS},     {"*", STAR},
      {"/", SLASH},   {"=", ASSIGN},
  };

  Position token_position = position;
  token_position.column -= (_currect - _start);

  auto it = tokenMap.find(str);
  if (it != tokenMap.end()) {
    return {it->second, token_position, str};
  }

  return {IDENTIFIER, token_position, str};
}

Token Lexer::nextToken() {
  scanSpace();

  _start = _currect;
  int symbol = getCurSymbol();

  if (symbol == '\0')
    return {END_OF_FILE, position, ""};
  if (std::isalpha(symbol) || symbol == '_') return scanIdentifier();
  if (std::isdigit(symbol))
    return scanNumLiteral();
  if (symbol == '"')
    return scanStrLiteral();

  return scanSymbol();
}
