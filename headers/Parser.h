#pragma once

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <math.h>

using namespace std;

struct Token {
    /*
    0: no oper
    */
    char oper;
    float val;

    Token(char _oper, float _val) {
        oper = _oper;
        val = _val;
    }
};

class Parser {
public:
    string text;
    queue<Token> postfix;

    Parser(string _text);

    int tokenize();

    float eval(float x, float y);

private:
    std::map<char, int> precedence = { {'+', 0}, {'-', 0}, {'*', 1}, {'/', 1}, {'^', 2}, {')', 3}, {'(', 3} };
};


