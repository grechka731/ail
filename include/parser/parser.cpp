#include "parser.h"
#include "lexer/lexer.h"
#include "parser/ast.h"
#include <charconv>
#include <cstddef>
#include <string_view>

void Parser::advance() {
  curToken = nextToken;
  nextToken = lexer.nextToken();
}

Node *Parser::parseStatement() {
  if (curToken.type == TokenEnum::LET) {
    advance();
    std::string_view name = curToken.word;
    advance();

    Node *value = nullptr;
    if (curToken.type == TokenEnum::ASSIGN) {
      advance();
      value = parseLowPrecedenceOperators();
    }
    return new LetNode(name, value);
  } else if (curToken.type == TokenEnum::CONST) {
    advance();
    std::string_view name = curToken.word;
    advance();

    Node *value = nullptr;
    if (curToken.type == TokenEnum::ASSIGN) {
      advance();
      value = parseLowPrecedenceOperators();
    }
    return new ConstNode(name, value);
  } else {
    Node *expr = parseLowPrecedenceOperators();
    return expr;
  }
}

Node *Parser::parseValue() {
  if (curToken.type == TokenEnum::INT_LITERAL) {
    int val;
    std::from_chars(curToken.word.data(),
                    curToken.word.data() + curToken.word.size(), val);
    advance();
    return new IntNode(val, 32);
  } else if (curToken.type == TokenEnum::FLOAT_LITERAL) {
    float val;
    std::from_chars(curToken.word.data(),
                    curToken.word.data() + curToken.word.size(), val);
    advance();
    return new FloatNode(val, 32);
  } else if (curToken.type == TokenEnum::CHAR_LITERAL) {
    char ch = curToken.word.data()[0];
    advance();
    return new CharNode(ch);
  } else if (curToken.type == TokenEnum::TRUE) {
    advance();
    return new BoolNode(1);
  } else if (curToken.type == TokenEnum::FALSE) {
    advance();
    return new BoolNode(0);
  } else if (curToken.type == TokenEnum::STRING_LITERAL) {
    char string[256];
    size_t i = 0;
    while (curToken.word.data()[0] != '"') {
      string[i] = curToken.word.data()[0];
      i++;
          advance();
    }
    advance();

    return new StringLiteralNode(string);
  }

  advance();
  return nullptr;
}

Node *Parser::parseLowPrecedenceOperators() {
  Node *left = parseMediumPrecedenceOperators();

  while (curToken.type == TokenEnum::PLUS ||
         curToken.type == TokenEnum::MINUS) {
    char op = (curToken.type == TokenEnum::PLUS) ? '+' : '-';
    advance();
    Node *right = parseMediumPrecedenceOperators();
    left = new BinaryNode(left, op, right);
  }

  return left;
}

Node *Parser::parseMediumPrecedenceOperators() {
  Node *left = parseHighPprecedenceOperators();

  while (curToken.type == TokenEnum::STAR ||
         curToken.type == TokenEnum::SLASH) {
    char op = (curToken.type == TokenEnum::STAR) ? '*' : '/';
    advance();
    Node *right = parseHighPprecedenceOperators();
    left = new BinaryNode(left, op, right);
  }

  return left;
}

Node *Parser::parseHighPprecedenceOperators() {
  Node *left = parseHighestPrecedenceOperators();

  if (curToken.type == TokenEnum::POW) {
    advance();
    Node *right = parseHighPprecedenceOperators();
    return new BinaryNode(left, '^', right);
  }

  return left;
}

Node *Parser::parseHighestPrecedenceOperators() {
  if (curToken.type == TokenEnum::LPAREN) {
    advance(); // '('
    Node *expr = parseLowPrecedenceOperators();
    if (curToken.type == TokenEnum::RPAREN) {
      advance(); // ')'
    }
    return expr;
  }

  return parseValue();
}

Node* Parser::parse() {
  curToken = lexer.nextToken();
  nextToken = lexer.nextToken();
  while (curToken.type != TokenEnum::END_OF_FILE) {
    parseStatement();
  }
}