#pragma once
#include "glad.h"
#include "shader.h"

class FrameBuffer {
public:
    FrameBuffer(vec2 res);

    void Bind();

    void UnBInd();

    void BindTexture();
private:
    unsigned int FBO;
    unsigned int TextureID;

};

