#include <chrono>
#include <iostream>
#include <lexer/lexer.h>
#include <parser/parser.h>
#include <parser/ast.h>
#include <string>
#include <vector>

using namespace std;

int main() {
  string sampleCode = R"(
let x = 100
const pi = 3.14
let flag_1 = true

fn calculate_tax a, b
  let tax = a * b
  let total = a + tax
  total

let message = "привет мир это длинная строка с пробелами"
let empty_str = ""
let glue_test = a + b - c * d / e
let weird_num = 12.34
let broken_ident = 999
const final_result = "конец блока"
)";

  const int REPEAT_COUNT = 1000;
  string hugeFile;
  hugeFile.reserve(sampleCode.size() * REPEAT_COUNT);
  for (int i = 0; i < REPEAT_COUNT; i++) hugeFile += sampleCode;

  Lexer benchLexer(hugeFile);
  size_t benchTokenCount = 0;
  auto startTime = chrono::high_resolution_clock::now();
  while (true) {
    Token t = benchLexer.nextToken();
    if (t.type == END_OF_FILE) break;
    benchTokenCount++;
  }
  auto endTime = chrono::high_resolution_clock::now();
  auto durationUs = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
  
  cout << "Лексер: " << benchTokenCount << " токенов за " << durationUs / 1000.0 << " мс" << endl;

  const int PARSE_REPEAT = 100;
  size_t totalStatements = 0;
  
  auto parseStartTime = chrono::high_resolution_clock::now();
  for (int i = 0; i < PARSE_REPEAT; i++) {
      Lexer parseLexer(sampleCode);
      Parser parser(std::move(parseLexer));
      std::vector<Node*> ast = parser.parse();
      totalStatements += ast.size();
      for (Node* node : ast) delete node;
  }
  auto parseEndTime = chrono::high_resolution_clock::now();
  auto parseDurationUs = chrono::duration_cast<chrono::microseconds>(parseEndTime - parseStartTime).count();
  
  cout << "Парсер: " << totalStatements << " стейтментов за " << parseDurationUs / 1000.0 << " мс" << endl;

  return 0;
}