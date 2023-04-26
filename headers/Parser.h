#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <cmath>
#include <glad.h>

struct Token {
    /*
    0: no oper
    */
    unsigned char oper;
    float val;

    Token(unsigned char _oper, float _val) {
        oper = _oper;
        val = _val;
    }
};

class Graph {
public:
    Graph(const std::string &_text);
    Graph() = default;

    void SetUniform(unsigned int Shader_ID);
    int Tokenize();

    void SetText(const std::string &_text);

    float Eval(float x, float y);

private:
    std::queue<Token> _postfix;
    std::string text;
    std::map<unsigned char, int> precedence = { {'+', 0}, {'-', 0}, {'*', 1}, {'/', 1}, {'^', 2}, {')', 3}, {'(', 3} };
};


