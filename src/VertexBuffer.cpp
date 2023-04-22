#include "vertexbuffer.h"

#include <iostream>

VertexBuffer::VertexBuffer() 
{
    glCreateBuffers(1, &BufferID);
    glBindBuffer(GL_ARRAY_BUFFER, BufferID);
    glEnableVertexArrayAttrib(BufferID, 0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    glEnableVertexAttribArray(0);
}


VertexBuffer :: ~VertexBuffer()
{
    glDeleteBuffers(1, &BufferID);
}

void VertexBuffer::AddData(const void* data, unsigned int size)
{
    glBindBuffer(GL_ARRAY_BUFFER, BufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*size, data, GL_DYNAMIC_DRAW);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, BufferID);
}

void VertexBuffer::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

