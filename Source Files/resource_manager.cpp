#include <iostream>
#include <fstream>
#include <cstdlib>
#include <SOIL.h>
#include <cstdio>
#include "..\\Header Files\\resource_manager.h"
#include "..\\Header Files\\shader.h"


// Instantiate static variables
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, std::shared_ptr<Shader>> ResourceManager::Shaders;


std::shared_ptr<Shader>  ResourceManager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, std::string name)
{
	GLchar* vertexShaderCode = loadShaderFromFile(vShaderFile);
    GLchar* fragmentShaderCode = loadShaderFromFile(fShaderFile);
	Shaders[name] = std::shared_ptr<Shader>(new Shader{ vertexShaderCode, fragmentShaderCode });
	if (Shaders[name] == nullptr)printf("shader is null\n");
	return Shaders[name];
}
std::shared_ptr<Shader> ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	////(Properly) delete all shaders	
	//for (auto iter : Shaders)
	//	glDeleteProgram(iter.second->ProgramID);
	//// (Properly) delete all textures
	//for (auto iter : Textures)
	//	glDeleteTextures(1, &iter.second.ID);
}

GLchar* ResourceManager::loadShaderFromFile(const GLchar* shaderFile)
{
	char tempBuffer[60];
	FILE* inFile; 
	fopen_s(&inFile, shaderFile, "r");
	if (inFile == nullptr) {
		printf("Could not open file");
		return NULL;
	}
	fseek(inFile, 0, SEEK_END);
	size_t fileSize = ftell(inFile);
	rewind(inFile);
	char* data = (char*)malloc(fileSize + 1);
	memset(data, '\0', fileSize + 1);
	while (fgets(tempBuffer, 60, inFile) != nullptr) {
		strcat_s(data, fileSize+1, tempBuffer);
	}
	fclose(inFile);
	//printf("%s", data);
	return data;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
	// Create Texture object
	Texture2D texture;
	if (alpha)
	{
		texture.internalFormat = GL_RGBA;
		texture.imageFormat = GL_RGBA;
	}
	// Load image
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.imageFormat == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Now generate texture
	texture.Generate(width, height, image);
	// And finally free image data
	SOIL_free_image_data(image);
	return texture;
}