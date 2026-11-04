#include "lexer.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>

using namespace std;

string tokenTypeToString(TokenEnum type) {
    switch (type) {
        case END_OF_FILE:    return "END_OF_FILE";
        case UNKNOWN:        return "UNKNOWN";
        case IDENTIFIER:     return "IDENTIFIER";
        case INT_LITERAL:    return "INT_LITERAL";
        case DOUBLE_LITERAL: return "DOUBLE_LITERAL";
        case CHAR_LITERAL:   return "CHAR_LITERAL";
        case STRING_LITERAL: return "STRING_LITERAL";
        case LET:            return "LET";
        case FN:             return "FN";
        case CONST:          return "CONST";
        case TRUE:           return "TRUE";
        case FALSE:          return "FALSE";
        case PLUS:           return "PLUS";
        case MINUS:          return "MINUS";
        case STAR:           return "STAR";
        case SLASH:          return "SLASH";
        case ASSIGN:         return "ASSIGN";
        default:             return "OTHER";
    }
}

int main() {
    string sampleCode = R"(
let x = 100
const pi = 3,1415926
let flag_1 = true
let _hidden_flag = false

fn calculate_tax = x * pi + 250 / 2 - 10,5

let message = "привет мир это длинная строка с пробелами и цифрами 12345"
let empty_str = ""

let glue_test = a+b-c*d/e=42,99
let weird_num = 12,34,56
let strange_symbols = @ # $ % ^ & ; ( )
let broken_ident = 999abc_def
const final_result = "конец блока"
)";

    const int REPEAT_COUNT = 1000;
    string hugeFile;
    hugeFile.reserve(sampleCode.size() * REPEAT_COUNT);
    for (int i = 0; i < REPEAT_COUNT; i++) {
        hugeFile += sampleCode;
    }

    Lexer benchLexer(hugeFile);
    size_t benchTokenCount = 0;

    auto startTime = chrono::high_resolution_clock::now();

    while (true) {
        Token t = benchLexer.nextToken();
        if (t.type == END_OF_FILE) {
            break;
        }
        benchTokenCount++;
    }

    auto endTime = chrono::high_resolution_clock::now();
    auto durationUs = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    double durationMs = durationUs / 1000.0;

    cout << "================ ДЕТАЛЬНЫЙ РАЗБОР ТОКЕНОВ ================" << endl;

    Lexer printLexer(sampleCode);
    vector<Token> parsedTokens;

    while (true) {
        Token t = printLexer.nextToken();
        if (t.type == END_OF_FILE) {
            break;
        }
        parsedTokens.push_back(t);
    }

    for (const Token& t : parsedTokens) {
        cout << "Слово: " << t.word
             << " | Тип: " << tokenTypeToString(t.type)
             << " | Позиция: строка " << t.position.row
             << ", столбец " << t.position.column << endl;
    }

    cout << "\n================ РЕЗУЛЬТАТЫ ТЕСТА СКОРОСТИ ================" << endl;
    cout << "Токенов в одном тестовом блоке: " << parsedTokens.size() << endl;
    cout << "Размер огромного файла (стресс-тест): " << hugeFile.size() << " байт" << endl;
    cout << "Успешно разобрано токенов (стресс-тест): " << benchTokenCount << endl;
    cout << "Честное время работы лексера (без cout): " << durationMs << " мс (" << durationUs << " мкс)" << endl;

    if (durationUs > 0) {
        double tokensPerSec = (benchTokenCount * 1000000.0) / durationUs;
        double mbPerSec = (hugeFile.size() / (1024.0 * 1024.0)) / (durationUs / 1000000.0);
        cout << "Скорость потока: ~" << static_cast<long long>(tokensPerSec) << " токенов/сек ("
             << mbPerSec << " МБ/сек)" << endl;
    }

    return 0;
}