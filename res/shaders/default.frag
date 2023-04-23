#version 460 core

layout(location = 0) out vec4 color;

in vec4 gl_FragCoord;


uniform float values[256];
uniform int opers[256];
uniform int size;
uniform vec2 Screen_Size;
uniform vec2 zoom;
uniform vec2 offset;
#define GRAPHCOLOR vec3(0.176, 0.439, 0.701)

float eval(vec2 pos) {
    float stack[256];
    int top = -1;
    int size_ = 0;
    while (size_ < size) {
        int front_oper = opers[size_];
        switch (front_oper) {
            case 43: { // '+'
                float temp = stack[top--];
                temp += stack[top--];
                stack[++top] = temp;
                break;
            }
            case 45: { // '-'
                float temp = stack[top--];
                temp = stack[top--] - temp;
                stack[++top] = temp;
                break;
            }
            case 42: { // '*'
                float temp = stack[top--];
                temp *= stack[top--];
                stack[++top] = temp;
                break;
            }
            case 47: { // '/'
                float temp = stack[top--];
                temp = stack[top--] / temp;
                stack[++top] = temp;
                break;
            }
            case 94: { // '^'
                float temp = stack[top--];
                temp = pow(stack[top--], temp);
                stack[++top] = temp;
                break;
            }
            case 120: { // 'x'
                stack[++top] = pos.x;
                break;
            }
            case 121: { // 'y'
                stack[++top] = pos.y;
                break;
            }
            default: {
                stack[++top] = values[size_];
                break;
            }
        }
        size_++;
    }

    return stack[top];
}



vec2 cameramat(vec2 pos) {
    return((pos - offset) * zoom);
}

void main() {
    float eps = (5.0*length(zoom)) / length(Screen_Size);
    vec2 pos = cameramat(2.0 * (gl_FragCoord.xy / Screen_Size) - vec2(1, 1));
    vec3 _color = vec3(0.9, 0.9, 0.9);
    _color = mix(_color, vec3(0.70), step(abs(pos.x - round(pos.x)), 0.001 * zoom.x));
    _color = mix(_color, vec3(0.70), step(abs(pos.y - round(pos.y)), 0.001 * zoom.y));
    _color = mix(_color, vec3(0.4), step(abs(pos.x), 0.002 * zoom.x));
    _color = mix(_color, vec3(0.4), step(abs(pos.y), 0.0025 * zoom.y));
    vec2 grad = vec2(
    (eval(pos + vec2(eps, 0)) - eval(pos - vec2(eps, 0))),
    (eval(pos + vec2(0, eps)) - eval(pos - vec2(0, eps)))
    ) / (2.0 * eps);
    eps *= length(grad);
    _color = mix(_color, GRAPHCOLOR, step(abs(eval(pos)), eps));
    color = vec4(_color, 0.9);
}
