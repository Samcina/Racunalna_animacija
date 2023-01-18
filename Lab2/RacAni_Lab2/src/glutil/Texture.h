#pragma once

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture();

	void Bind(unsigned int slot = 0);
	void Unbind();

	int GetWidth() const;
	int GetHeight() const;
	int GetBoundSlot() const;
private:
	unsigned int id;
	std::string filePath;
	unsigned char* localBuffer;
	int width;
	int height;
	int BPP;
	int boundSlot;
};