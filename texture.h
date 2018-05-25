#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

class Texture2D
{
public:
	GLuint ID;
	// Texture image dimensions
	GLuint width, height;
	// Texture Format
	GLuint internalFormat;
	GLuint imageFormat;
	// Texture configuration
	GLuint wrap_S;
	GLuint wrap_T;
	GLuint filterMin;
	GLuint filterMax;
	// Constructor (sets default texture modes)
	Texture2D();
	// Generates texture from image data
	void Generate(GLuint width, GLuint height, unsigned char* data);
	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
};


#endif
