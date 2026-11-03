#include <string>

enum TokenEnum {
    // special tokens
    END_OF_FILE,
    UNKNOWN,

    // literals and identifiers
    IDENTIFIER,
    INT_LITERAL,
    DOUBLE_LITERAL,
    CHAR_LITERAL,

    // keywords
    LET,
    FN,
    CONST,
    TRUE,
    FALSE,

    // operators
    PLUS,
    MINUS,
    STAR,
    SLASH,
    ASSIGN
};

struct Position{
inline static std::string path="";
int row;
int column;
};

struct Token{
    TokenEnum type;
    Position position;
    std::string_view word;
};