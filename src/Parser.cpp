#include "Parser.h"


Graph::Graph(const std::string& _text) {
    SetText(_text);
}

void Graph::SetText(const std::string& _text) {
    text = _text;

    if(Tokenize() == -1){
        std::cout << "Error: invalid input" << std::endl;
        std::queue<Token> empty;
        empty.emplace(0,-1);
        _postfix = empty;
    }
}

void Graph::SetUniform(unsigned int Shader_ID) {
    glUseProgram(Shader_ID);
    std::queue<Token> postfix = _postfix;
    float values[256];
    int opers[256];
    int location;

    int size = postfix.size();
    for(int cur = 0; cur < 256 && !postfix.empty(); cur++){

        values[cur] = postfix.front().val;
        opers[cur] = postfix.front().oper;
        std::cout << postfix.front().oper << ": " << postfix.front().val <<std::endl;
        postfix.pop();
    }

    location = glGetUniformLocation(Shader_ID, "size");
    glUniform1i(location, size);

    location = glGetUniformLocation(Shader_ID, "values");
    glUniform1fv(location, 256,values);

    location = glGetUniformLocation(Shader_ID, "opers");

    glUniform1iv(location, 256,opers);
    glUseProgram(0);
}

int Graph::Tokenize() {
    std::stack<Token> stack;
    std::queue<Token> empty;
    _postfix = empty;
    bool last_token_was_op = true;
    for (int cur = 0; cur < text.size(); ++cur) {
        unsigned char currentchar = text[cur];

        if (isdigit(currentchar)) {
            int startcur = cur;
            bool isdot = false;
            while ((text[cur] >= '0' && text[cur] <= '9') || (!isdot && text[cur] == '.')) {
                if (text[cur] == '.') {
                    isdot = true;
                }
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
        else if (currentchar == '=') {
            while (!stack.empty()) {
                _postfix.emplace(stack.top().oper, 0);
                stack.pop();
            }
            stack.emplace('-', 0);
            stack.emplace('(',0);
        }
        else if (currentchar == 'x' || currentchar == 'y') {
            _postfix.emplace(currentchar, 0);
            last_token_was_op = false;
        }
        else if (currentchar == 's' && cur + 2 < text.size() && text.substr(cur, 3) == "sin") {
            stack.emplace('s', 0);
            cur += 2; // skip "in" characters
            last_token_was_op = true;
        }
        else if (currentchar != ' ') {
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

    while (!stack.empty() && stack.top().oper != '(') {
        _postfix.emplace(stack.top().oper, 0);
        stack.pop();
    }

    if(!stack.empty()){
    stack.pop();
    }

    while (!stack.empty()) {
        _postfix.emplace(stack.top().oper, 0);
        stack.pop();
    }

    return 0;
}

float Graph::Eval(float x, float y) {
    std::queue<Token> postfix = _postfix;
    std::stack<float> stack;
    while (!postfix.empty()) {
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