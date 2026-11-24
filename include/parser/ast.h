#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

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
  std::string string;
  StringLiteralNode(std::string string) : string(string) {}
};

class IdentifierNode : public Node {
public:
  std::string_view name;

  IdentifierNode(std::string_view name) : name(name) {}

  ~IdentifierNode() override {}
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

  LetNode(std::string_view name, Node *value) : name(name), value(value) {}

  ~LetNode() override { delete value; }
};

class ConstNode : public Node {
public:
  std::string_view name;
  Node *value;

  ConstNode(std::string_view name, Node *value) : name(name), value(value) {}

  ~ConstNode() override { delete value; }
};

class FnNode : public Node {
public:
  std::string_view nameFn;
  std::vector<std::string_view> param;
  std::vector<Node *> body;

  FnNode(std::string_view nameFn, std::vector<std::string_view> param,
         std::vector<Node *> body)
      : nameFn(nameFn), param(param), body(body) {};

  ~FnNode() override {
    for (size_t i = 0; i < body.size(); i++)
      delete body[i];
  }
};
