#pragma once

#include <cstdint>
#include <string>
#include <string_view>

class Node {
public:
  virtual ~Node() = default;
};

class IntNode : public Node {
public:
  uint64_t number;
  uint8_t bits;
  IntNode(int value, uint8_t bits = 32) : number(value), bits(bits) {}
};

class FloatNode : public Node {
public:
  double number;
  uint8_t bits;
  FloatNode(double value, uint8_t bits = 32) : number(value), bits(bits) {}
};

class CharNode : public Node {
public:
  char number;
  CharNode(char value) : number(value) {}
};

class BoolNode : public Node {
public:
  bool value;
  BoolNode(bool value) : value(value) {}
};

class StringLiteralNode : public Node {
public:
char* string;
StringLiteralNode(char* string) : string(string) {}
};


class BinaryNode : public Node {
public:
  char op;
  Node *leftOperand;
  Node *rightOperand;

  BinaryNode(Node *left, char op_char, Node *right)
      : leftOperand(left), op(op_char), rightOperand(right) {}

  ~BinaryNode() override {
    delete leftOperand;
    delete rightOperand;
  }
};

class LetNode : public Node {
public:
  std::string_view name;
  Node *value;

  LetNode(std::string_view name, Node *value)
      : name(name), value(value) {}

  ~LetNode() override { delete value; }
};

class ConstNode : public Node {
public:
  std::string_view name;
  Node *value;

  ConstNode(std::string_view name, Node *value)
      : name(name), value(value) {}

  ~ConstNode() override { delete value; }
};