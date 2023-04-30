#pragma once
#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

    vec2 lastpress;

    Shader *maskshader;
    Shader *lineshader;
    char input[256];

private:
    GLFWwindow* _window;

    FrameBuffer *Mask;
    Graph parser;
    std::vector <VertexBuffer> _VertexBuffers;
    std::vector <IndexBuffer> _IndexBuffers;
    unsigned int vao;

    bool test=true;

    void Draw();
};

