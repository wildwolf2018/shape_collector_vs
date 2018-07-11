#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <GL/glew.h>
#include "..\\Header Files\\shader.h"


Shader::Shader(const GLchar *vShaderCode, const GLchar *fShaderCode)
{
	GLuint vertex = CompileShader(vShaderCode, GL_VERTEX_SHADER);
	GLuint fragment = CompileShader(fShaderCode, GL_FRAGMENT_SHADER);
	LinkProgram(vertex, fragment);
}

GLuint Shader::CompileShader(const GLchar *shaderSource, GLenum shaderType)
{
	GLuint handle;
	GLint success;
	GLchar infoLog[512];

	if (shaderType == GL_VERTEX_SHADER)
	{
		handle = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (shaderType == GL_FRAGMENT_SHADER)
	{
		handle = glCreateShader(GL_FRAGMENT_SHADER);
	}
	glShaderSource(handle, 1, &shaderSource, NULL);
	glCompileShader(handle);
	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(handle, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	free((void*)shaderSource);
	return handle;
}

void Shader::LinkProgram(GLuint hVertexShader, GLuint hFragmentShader)
{
	GLuint shaderProgram;
	GLint success;
	GLchar infoLog[512];

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, hVertexShader);
	glAttachShader(shaderProgram, hFragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	ProgramID = shaderProgram;
}
void Shader::Use()
{
	glUseProgram(ProgramID);
}

void Shader::setMatrix(glm::mat4 &matrix, std::string type)
{
	GLint location = glGetUniformLocation(ProgramID, type.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setSampler2DValue(std::string samplerName, GLint samplerValue)
{
	GLint location = glGetUniformLocation(ProgramID, samplerName.c_str());
	glUniform1i(location, samplerValue);
}
