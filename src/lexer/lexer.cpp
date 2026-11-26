#include "lexer/token.h"
#include <cctype>
#include <lexer/lexer.h>
#include <string>
#include <string_view>
#include <unordered_map>

char Lexer::checkNextSymbol() { return (_currect + 1) < _file.size() ? _file[_currect + 1] : '\0'; }

void Lexer::goNextSymbol() {
    if (_currect < _file.size()) {
        _currect++;
        position.column++;
    }
}

char Lexer::getCurSymbol() { return _currect < _file.size() ? _file[_currect] : '\0'; }

char Lexer::getSymbol(int poition) { return poition < _file.size() ? _file[poition] : '\0'; }

inline void Lexer::scanSpace() {
    int symbol = getCurSymbol();

    while (_currect < _file.size() && (symbol == ' ' || symbol == '\t' || symbol == '\r')) {
        goNextSymbol();
    }
}

inline Token Lexer::scanSection() {
    if (getCurSymbol() == '@') {
        std::string word;
        char curSymbol = getCurSymbol();
        while (curSymbol != ':') {
            word.push_back(curSymbol);
            curSymbol = getCurSymbol();
        }

        if (word == "LOGIC")
            return { TokenEnum::AT_LOGIC, position, word.substr(1, word.size() - 1) };
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

    return stringToToken(std::string_view(_file).substr(_start, _currect - _start));
}

inline Token Lexer::scanNumLiteral() {
    int symbol = getCurSymbol();

    bool isFloat = false;

    while (_currect < _file.size()) {
        symbol = getCurSymbol();

        if (std::isdigit(symbol)) {
            goNextSymbol();
        } else if ((symbol == '.') && !isFloat && std::isdigit(checkNextSymbol())) {
            isFloat = true;
            goNextSymbol();
        } else {
            break;
        }
    }

    Position token_position = position;
    token_position.column -= (_currect - _start);

    TokenEnum type = isFloat ? TokenEnum::FLOAT_LITERAL : TokenEnum::INT_LITERAL;

    return {type, token_position, std::string_view(_file).substr(_start, _currect - _start)};
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
    return stringToToken(std::string_view(_file).substr(_start, _currect - _start));
}

Token Lexer::stringToToken(std::string_view str) {
    static const std::unordered_map<std::string_view, TokenEnum> tokenMap{
        {"let", LET}, {"fn", FN},    {"const", CONST}, {"true", TRUE}, {"false", FALSE},

        {"+", PLUS},  {"-", MINUS},  {"*", STAR},      {"/", SLASH},   {"=", ASSIGN},
        {"^", POW},   {"(", LPAREN}, {")", RPAREN},    {",", COMMA},   {"\n", NEWLINE}};

    Position token_position = position;
    token_position.column -= (_currect - _start);

    auto it = tokenMap.find(str);
    if (it != tokenMap.end()) {
        return {it->second, token_position, str};
    }

    return {IDENTIFIER, token_position, str};
}

Token Lexer::nextToken() {
    if (getCurSymbol() == '\n') {
        Position token_position = position;

        goNextSymbol();
        _start = _currect;

        position.row++;
        position.column = 0;

        return {TokenEnum::NEWLINE, token_position, "\n"};
    }
    scanSpace();

    _start = _currect;
    int symbol = getCurSymbol();

    if (symbol == '\0')
        return {END_OF_FILE, position, ""};
    if (std::isalpha(symbol) || symbol == '_')
        return scanIdentifier();
    if (std::isdigit(symbol))
        return scanNumLiteral();
    if (symbol == '"')
        return scanStrLiteral();

    return scanSymbol();
}
