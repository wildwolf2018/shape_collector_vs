#ifndef MESH_H
#define MESH_H
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "..\\Header Files\\shader.h"

using std::vector;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
};

class Mesh { 
public:
	vector<Vertex> vertices; 
	vector<GLuint> indices; 
	Mesh(vector<Vertex> vertices, vector<GLuint> indices); 
	void Draw();
private:
	GLuint VAO, VBO, EBO;
	void setupMesh(); 
};

#endif