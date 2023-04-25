#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <math.h>
#include <glad.h>

using namespace std;

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
    string text;
    Graph(string _text);
    Graph(string *_text);

    void setuniforms(unsigned int Shader_ID);
    int tokenize();

    float eval(float x, float y);

private:
    queue<Token> _postfix;
    std::map<char, int> precedence = { {'+', 0}, {'-', 0}, {'*', 1}, {'/', 1}, {'^', 2}, {')', 3}, {'(', 3} };
};


