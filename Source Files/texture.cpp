#include <iostream>
#include "..\\Header Files\\texture.h"

Texture2D::Texture2D()
	: width(0), height(0), internalFormat(GL_RGBA), imageFormat(GL_RGBA), wrap_S(GL_REPEAT), wrap_T(GL_REPEAT), filterMin(GL_LINEAR), filterMax(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data)
{
	this-> width = width;
	this-> height = height;
	// Create Texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this-> internalFormat, width, height, 0, this-> imageFormat, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this-> wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this-> wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this-> filterMax);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}