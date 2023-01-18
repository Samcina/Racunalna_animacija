#include "Texture.h"
#include "stb_image/stb_image.h"
#include <iostream>

Texture::Texture(const std::string& filePath)
	:
	id(0),
	filePath(filePath),
	localBuffer(nullptr),
	width(0),
	height(0),
	BPP(0),
	boundSlot(-1)
{
	std::cout << filePath << std::endl;
	stbi_set_flip_vertically_on_load(1);
	localBuffer = stbi_load(filePath.c_str(), &width, &height, &BPP, 4);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (localBuffer)
		stbi_image_free(localBuffer);
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

void Texture::Bind(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, id);
	boundSlot = 0 + slot;
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	//boundSlot = -1;
}

int Texture::GetWidth() const
{
	return width;
}

int Texture::GetHeight() const
{
	return height;
}

int Texture::GetBoundSlot() const
{
	return boundSlot;
}
