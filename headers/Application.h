#pragma once
#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <vector>
#include "vertexbuffer.h"
#include "IndexBuffer.h"
#include "shader.h"
#include "Parser.h"
#include "FrameBuffer.h"

void MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam );


class Application
{
public:
    Application(int screen_X, int scren_Y);
    ~Application() {
    };
    void GameLoop();
    vec2 res;
    vec2 zoom;
    vec2 offset;

    Shader *FirstPass;
    Shader *SecondPass;

private:
    GLFWwindow* _window;

    FrameBuffer *Mask;
    Graph parser;
    std::vector <VertexBuffer> _VertexBuffers;
    std::vector <IndexBuffer> _IndexBuffers;
    unsigned int vao;

    
    void Draw();
};

