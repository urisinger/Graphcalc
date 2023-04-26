#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
    glCreateBuffers(1, &BufferID);
}


IndexBuffer :: ~IndexBuffer()
{
    glDeleteBuffers(1, &BufferID);
}

void IndexBuffer :: AddData(const unsigned int* data, unsigned int count) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}


void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferID);
}

void IndexBuffer::UnBind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}