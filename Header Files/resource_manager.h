#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <map>
#include <string>

#include <GL/glew.h>
#include "..\\Header Files\\shader.h"
#include "..\\Header Files\\texture.h"

class ResourceManager
{
public:
	// Resource storage
	static std::map<std::string, std::shared_ptr<Shader>> Shaders;
	static std::map<std::string, Texture2D> Textures;
	static std::map<std::string, glm::mat4> matrices;
	// Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	static std::shared_ptr<Shader>   LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name);
	// Retrieves a stored sader
	static std::shared_ptr<Shader>   GetShader(std::string name);
	// Loads (and generates) a texture from file
	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
	// Retrieves a stored texture
	static Texture2D GetTexture(std::string name);
	// Properly de-allocates all loaded resources
	static void      Clear();
private:
	// Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	ResourceManager() { }
	// Loads and generates a shader from file
	static  GLchar* loadShaderFromFile(const GLchar *shaderFile);
	// Loads a single texture from file
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);
};

#endif
