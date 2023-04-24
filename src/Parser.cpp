#include "Parser.h"

Parser::Parser(string *_text) {
    text = *_text;
    if (tokenize() == -1) {
        cout << "Error: invalid input" << endl;
    }
}

Parser::Parser(string _text) {
    text = _text;
    if (tokenize() == -1) {
        cout << "Error: invalid input" << endl;
    }
}

void Parser::setuniforms(unsigned int Shader_ID) {
    queue<Token> postfix = _postfix;
    float values[256];
    int opers[256];
    int location;

    int size = postfix.size();
    for(int cur = 0; cur < 256 && !postfix.empty(); cur++){

        values[cur] = postfix.front().val;
        opers[cur] = postfix.front().oper;
        std::cout << opers[cur]<< ": " << postfix.front().val <<std::endl;
        postfix.pop();
    }

    location = glGetUniformLocation(Shader_ID, "size");
    glUniform1i(location, size);

    location = glGetUniformLocation(Shader_ID, "values");
    glUniform1fv(location, 256,values);

    location = glGetUniformLocation(Shader_ID, "opers");

    glUniform1iv(location, 256,opers);
}
int Parser::tokenize() {
    stack<Token> stack;
    bool last_token_was_op = true;
    for (int cur = 0; cur < text.size(); ++cur) {
        unsigned char currentchar = text[cur];

        if (isdigit(currentchar)) {
            int startcur = cur;
            bool isdot = false;
            while ((text[cur] >= '0' && text[cur] <= '9') || (!isdot && text[cur] == '.')) {
                cur++;
            }
            if (cur - startcur > 0) {
                _postfix.emplace(0, atof(text.substr(startcur, cur - startcur).c_str()));
                cur--; // decrement cur by 1 to re-check the current character
            }
            else {
                return -1;
            }
            last_token_was_op = false;
        }
        else if(currentchar== '='){


            while (!stack.empty() && stack.top().oper != '(') {
                _postfix.emplace(stack.top().oper, 0);
                stack.pop();
            }
            stack.emplace('-',0);

        }
        else if (currentchar == 'x' || currentchar == 'y') {
            _postfix.emplace(currentchar, 0);
            last_token_was_op = false;
        }
        else {
               if (last_token_was_op && currentchar != '(' && currentchar != ')')
                return -1;
            if (currentchar == ')') {
                while (!stack.empty() && stack.top().oper != '(') {
                    _postfix.emplace(stack.top().oper, 0);
                    stack.pop();
                }
                stack.pop();
            }
            else {
                while (!stack.empty() && stack.top().oper != '(' && precedence[stack.top().oper] >= precedence[currentchar]) {
                    _postfix.emplace(stack.top().oper, 0);
                    stack.pop();
                }
                stack.emplace(currentchar, 0);
                if (currentchar != '(')
                    last_token_was_op = true;
            }
        }
    }
    while (!stack.empty()) {
        _postfix.emplace(stack.top().oper, 0);
        stack.pop();
    }
    return 0;
}

float Parser::eval(float x, float y) {
    queue<Token> postfix = _postfix;
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