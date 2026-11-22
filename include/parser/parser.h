#pragma once
#include "lexer/lexer.h"
#include "ast.h"

class Parser{
    Lexer lexer;
    Token curToken;
    Token nextToken;

    void advance();
    Node* parseStatement();
    
    //value
    Node* parseValue();

    //lowest precedence + -  
    Node* parseLowPrecedenceOperators();

    //medium precedence * / % 
    Node* parseMediumPrecedenceOperators();

    //high precedence ** sqrt
    Node* parseHighPprecedenceOperators();
    
    //Highest precedence ()
    Node* parseHighestPrecedenceOperators();
    
    public:

    Parser(Lexer lex) : lexer(std::move(lex)) {} 

    Node* parse();
};