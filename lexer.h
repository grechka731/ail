#pragma once
#include <vector>
#include <string>
#include <string_view>
#include "token.h"

struct Token;

class Lexer {
    int _start = 0; 
    int _currect = 0; 

    Position position{ 0, 1};

    std::string _file;

    char checkNextSymbol();
    void goNextSymbol();
    char getCurSymbol();
    char getSymbol(int poition);

    inline void scanSpace();
    inline Token scanIdentifier(); 
    inline Token scanNumLiteral();   
    inline Token scanStrLiteral();
    inline Token scanCharLiteral();
    inline Token scanSymbol();

    Token stringToToken(std::string_view str);
  
public:
    explicit Lexer(std::string file) : _file(std::move(file)) {};
    Token nextToken();
};