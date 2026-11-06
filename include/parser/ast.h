#pragma once

#include <cstdint>

class Node {
public:
    virtual ~Node() = default;
};

class IntNode : public Node {
public:
    uint64_t number;
    uint8_t bits;
    IntNode(int value, uint8_t bits=32) : number(value), bits(bits){}
};

class FloatNode : public Node {
public:
    double number; 
    uint8_t bits;
    FloatNode(double value, uint8_t bits=32) : number(value), bits(bits) {}
};

class BinaryNode : public Node {
public:
    Node* leftOperand;  
    char op;            
    Node* rightOperand;

    BinaryNode(Node* left, char op_char, Node* right)
        : leftOperand(left), op(op_char), rightOperand(right) {}

    // Деструктор: когда удаляется коробка BinaryNode, 
    // она сама очищает память от левой и правой части
    ~BinaryNode() override {
        delete leftOperand;
        delete rightOperand;
    }
};