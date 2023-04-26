#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(vec2 res) {
    glGenFramebuffers(1,&FBO);
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);

    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D,TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,res.x,res.y,0,GL_RGB,GL_UNSIGNED_BYTE,0);


    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,TextureID,0);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void FrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
}

void FrameBuffer::UnBInd() {
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}
void FrameBuffer::BindTexture() {
    glBindTexture(GL_TEXTURE_2D,TextureID);
}