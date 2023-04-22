#pragma once

#include <GL/glew.h>

class IndexBuffer
{
private:
	unsigned int BufferID;
public:
	IndexBuffer();
	~IndexBuffer();
	
	void adddata(const unsigned int* data, unsigned int count);
	void Bind() const;
	void UnBind() const;

};