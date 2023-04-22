#include "Parser.h"

Parser::Parser(string _text) {
    text = _text;
    if (tokenize() == -1) {
        cout << "Error: invalid input" << endl;
    }
}

int Parser::tokenize() {
    stack<Token> stack;
    char last_token_was_op = true;
    for (int cur = 0; cur < text.size(); ++cur) {
        char currentchar = text[cur];

        if (isdigit(currentchar)) {
            int startcur = cur;
            bool isdot = false;
            while ((text[cur] >= '0' && text[cur] <= '9') || (!isdot && text[cur] == '.')) {
                cur++;
            }
            if (cur - startcur > 0) {
                postfix.emplace(0, atof(text.substr(startcur, cur - startcur).c_str()));
                cur--; // decrement cur by 1 to re-check the current character
            }
            else {
                return -1;
            }
            last_token_was_op = false;
        }
        else if (currentchar == 'x' || currentchar == 'y') {
            postfix.emplace(currentchar, 0);
            last_token_was_op = false;
        }
        else {
            if (last_token_was_op && currentchar != '(' && currentchar != ')')
                return -1;
            if (currentchar == ')') {
                while (!stack.empty() && stack.top().oper != '(') {
                    postfix.emplace(stack.top().oper, 0);
                    stack.pop();
                }
                stack.pop();
            }
            else {
                while (!stack.empty() && stack.top().oper != '(' && precedence[stack.top().oper] >= precedence[currentchar]) {
                    postfix.emplace(stack.top().oper, 0);
                    stack.pop();
                }
                stack.emplace(currentchar, 0);
                if (currentchar != '(')
                    last_token_was_op = true;
            }
        }
    }
    while (!stack.empty()) {
        postfix.emplace(stack.top().oper, stack.top().val);
        stack.pop();
    }
    return 0;
}

float Parser::eval(float x, float y) {
    stack<float> stack;
    while (postfix.size() > 0) {
        Token* front = &postfix.front();
        switch (front->oper) {
        case '+': {
            float temp = stack.top();
            stack.pop();
            temp += stack.top();
            stack.pop();
            stack.push(temp);
            break;
        }
        case '-': {
            float temp = stack.top();
            stack.pop();
            temp = stack.top() - temp;
            stack.pop();
            stack.push(temp);
            break;
        }
        case '*': {
            float temp = stack.top();
            stack.pop();
            temp *= stack.top();
            stack.pop();
            stack.push(temp);
            break;
        }
        case '/': {
            float temp = stack.top();
            stack.pop();
            temp = stack.top() / temp;
            stack.pop();
            stack.push(temp);
            break;
        }
        case '^': {
            float temp = stack.top();
            stack.pop();
            temp = pow(stack.top(), temp);
            stack.pop();
            stack.push(temp);
            break;
        }
        case 'x': {
            stack.push(x);
            break;
        }
        case 'y': {
            stack.push(y);
            break;
        }
        default: {
            stack.push(front->val);
            break;
        }
        }
        postfix.pop();
    }

    return stack.top();
}