#ifndef FONT_H
#define FONT_H

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <map>
#include <vector>
#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "shader.h"

//Oustanding -> 220.0f, 300.0f, 1.0f
//Excellent -> 250.0f, 300.0f, 1.0f
//Superb ->  280.0f, 300.0f, 1.0f
//Good -> 310.0f, 300.0f, 1.0f
struct Character {
	glm::vec4 size; // xpos,ypos,width and height of glyph
	glm::vec4 cornerPoints; // coordinates of bottom left and top right corner of glyph
	float yOffset;
};

struct FreeTypeCharacter
{
	GLuint TextureID; // ID handle of the glyph texture 
	glm::ivec2 Size; // Size of glyph 
	glm::ivec2 Bearing; // Offset from baseline to left/top of glyph 
	GLuint Advance; // Offset to advance to next glyph 
};

class Font
{
	public:
		Font(const char* name);
		void loadFont(const char* fontName);
		void intializeFont();
		void renderCharacters(std::shared_ptr<Shader> &s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	private:
		GLuint vao;
		GLuint vbo;
		GLuint fontVAO;
		GLuint fontVBO;
		std::map<GLchar, FreeTypeCharacter> Characters;
};
#endif
