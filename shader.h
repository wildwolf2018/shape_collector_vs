#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <memory>
#include <string>

class Shader
{
public:
	Shader(const GLchar *vShaderCode, const GLchar *fShaderCode);
	//Returns program ID 
	void Use();
	void setMatrix(glm::mat4 &matrix, std::string type);
	void setSampler2DValue(std::string samplerName, GLint samplerValue);
	//Program ID
	GLuint ProgramID;
private:
	//Compiles shader object
	GLuint CompileShader(const GLchar* shaderSource, GLenum shaderType);
	//Links program object
	void LinkProgram(GLuint hVertexShader, GLuint hFragmentShader);
};
#endif
