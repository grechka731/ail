#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include <charconv>
#include <string_view>

void Parser::advance() {
    curToken = nextToken;
    nextToken = lexer.nextToken();
}

Node* Parser::parseStatement() {
    if (curToken.type == TokenEnum::LET) {
        advance();
        std::string_view name = curToken.word;
        advance();

        Node* value = nullptr;
        if (curToken.type == TokenEnum::ASSIGN) {
            advance();
            value = parseLowPrecedenceOperators();
        }
        return new LetNode(name, value);
    } else if (curToken.type == TokenEnum::CONST) {
        advance();
        std::string_view name = curToken.word;
        advance();

        Node* value = nullptr;
        if (curToken.type == TokenEnum::ASSIGN) {
            advance();
            value = parseLowPrecedenceOperators();
        }
        return new ConstNode(name, value);
    } else if (curToken.type == TokenEnum::FN) {
        advance();
        std::string_view nameFn = curToken.word;
        advance();

        std::vector<std::string_view> param;
        while (curToken.type != TokenEnum::NEWLINE) {
            if (curToken.type != TokenEnum::COMMA)
                param.push_back(curToken.word);
            advance();
        }
        advance();

        std::vector<Node*> body;
        while (true) {
            if (curToken.type == TokenEnum::END_OF_FILE)
                break;
            else if (curToken.type == TokenEnum::NEWLINE) {
                advance();
                if (curToken.type == TokenEnum::NEWLINE) {
                    advance();
                    break;
                } else
                    continue;
            } else {
                body.push_back(parseStatement());
            }
        }

        return new FnNode(nameFn, param, body);
    } else {
        Node* expr = parseLowPrecedenceOperators();
        return expr;
    }
}

Node* Parser::parseValue() {
    if (curToken.type == TokenEnum::INT_LITERAL) {
        int val;
        std::from_chars(curToken.word.data(), curToken.word.data() + curToken.word.size(), val);
        advance();
        return new IntNode(val, 32);
    } else if (curToken.type == TokenEnum::FLOAT_LITERAL) {
        float val;
        std::from_chars(curToken.word.data(), curToken.word.data() + curToken.word.size(), val);
        advance();
        return new FloatNode(val, 32);
    } else if (curToken.type == TokenEnum::CHAR_LITERAL) {
        char ch = curToken.word.data()[1];
        advance();
        return new CharNode(ch);
    } else if (curToken.type == TokenEnum::TRUE) {
        advance();
        return new BoolNode(true);
    } else if (curToken.type == TokenEnum::FALSE) {
        advance();
        return new BoolNode(false);
    } else if (curToken.type == TokenEnum::STRING_LITERAL) {
        advance();
        std::string_view buffer = curToken.word.substr(1, curToken.word.size() - 2);
        advance();
        return new StringLiteralNode(buffer);
    } else if (curToken.type == TokenEnum::IDENTIFIER) {
        std::string_view name = curToken.word;
        advance();
        return new IdentifierNode(name);
    }

    advance();
    return nullptr;
}

Node* Parser::parseLowPrecedenceOperators() {
    Node* left = parseMediumPrecedenceOperators();

    while (curToken.type == TokenEnum::PLUS || curToken.type == TokenEnum::MINUS) {
        TokenEnum op = curToken.type;
        advance();
        Node* right = parseMediumPrecedenceOperators();
        left = new BinaryNode(left, op, right);
    }

    return left;
}

Node* Parser::parseMediumPrecedenceOperators() {
    Node* left = parseHighPprecedenceOperators();

    while (curToken.type == TokenEnum::STAR || curToken.type == TokenEnum::SLASH) {
        TokenEnum op = curToken.type;
        advance();
        Node* right = parseHighPprecedenceOperators();
        left = new BinaryNode(left, op, right);
    }

    return left;
}

Node* Parser::parseHighPprecedenceOperators() {
    Node* left = parseHighestPrecedenceOperators();

    if (curToken.type == TokenEnum::POW) {
        advance();
        Node* right = parseHighPprecedenceOperators();
        return new BinaryNode(left, TokenEnum::POW, right);
    }

    return left;
}

Node* Parser::parseHighestPrecedenceOperators() {
    if (curToken.type == TokenEnum::LPAREN) {
        advance(); // '('
        Node* expr = parseLowPrecedenceOperators();
        if (curToken.type == TokenEnum::RPAREN) {
            advance(); // ')'
        }
        return expr;
    }

    return parseValue();
}

std::vector<Node*> Parser::parse() {
    std::vector<Node*> statement;

    curToken = lexer.nextToken();
    nextToken = lexer.nextToken();

    Node* buffer;
    while (curToken.type != TokenEnum::END_OF_FILE) {
        buffer = parseStatement();

        if (buffer != nullptr)
            statement.push_back(buffer);
    }

    return statement;
}
