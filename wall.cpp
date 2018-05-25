#include <iostream>
#include <vector>
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "resource_manager.h"
#include "wall.h"

Wall::Wall(glm::vec3 _normal)
	:normal{ _normal }
{
	//initilize the vertices
	Vec3 v1 = { -10.0f, 2.5f, 0.0f };
	Vec3 v2 = { -10.0f, -2.5f, 0.0f };
	Vec3 v3 = { 10.0f, 2.5f, 0.0f };
	Vec3 v4 = { 10.0f, -2.5f, 0.0f };
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);
	vertices.push_back(v4);
	//normals
	Vec3 norm = { normal.x, normal.y, normal.z };
	norms.push_back(norm);
	norms.push_back(norm);
	norms.push_back(norm);
	norms.push_back(norm);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3) + norms.size() * sizeof(Vec3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vec3), &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3), norms.size() * sizeof(Vec3), &norms[0]);
	//create vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//map index 0 to vertex postion in buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3), (GLvoid*)(vertices.size() * sizeof(Vec3)));
	//enable vertex attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Wall::setUniforms(std::shared_ptr<Shader>& shaderObj, glm::vec3& cameraPos)
{
	GLuint programID = shaderObj->ProgramID;
	//Light propeties' uniform locations
	GLuint lighPositionLoc = glGetUniformLocation(programID, "light.position");
	GLuint lighAmbLoc = glGetUniformLocation(programID, "light.ambient");
	GLuint lighDiffuseLoc = glGetUniformLocation(programID, "light.diffuse");
	GLuint lighSpecLoc = glGetUniformLocation(programID, "light.specular");
	GLuint viewPositionLoc = glGetUniformLocation(programID, "viewPos");
	glUniform3f(viewPositionLoc, cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(lighAmbLoc, 0.2f, 0.2f, 0.2f);
	glUniform3f(lighDiffuseLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lighSpecLoc, 1.0f, 1.0, 1.0f);
	//Set Material properties uniforms
	GLuint materialAmbLoc = glGetUniformLocation(programID, "material.ambient");
	GLuint materialDiffuseLoc = glGetUniformLocation(programID, "material.diffuse");
	GLuint materialSpecLoc = glGetUniformLocation(programID, "material.specular");
	GLuint specExpoLoc = glGetUniformLocation(programID, "material.shininess");
	glUniform3f(materialAmbLoc, 0.5f, 0.5f, 0.5f);
	glUniform3f(materialDiffuseLoc, 0.5f, 0.5f, 0.5f);
	glUniform3f(materialSpecLoc, 1.0f, 1.0f, 1.0f);
	glUniform1f(specExpoLoc, 32.0f);
}
void Wall::draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}