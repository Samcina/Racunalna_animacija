#pragma once
#include "glm/glm.hpp"

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(unsigned int size);
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;
	void BufferSubData(const void* data, unsigned int size);
private:
	unsigned int id;
};