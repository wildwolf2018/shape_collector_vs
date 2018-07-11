#include "..\\Header Files\\mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices) 
{
	this->vertices = vertices; 
	this->indices = indices; 
	this->setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &this->VAO); 
	glGenBuffers(1, &this->VBO); 
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0); 
	glEnableVertexAttribArray(1); 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal)); 
	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glActiveTexture(GL_TEXTURE0);
	// Draw mesh 
	glBindVertexArray(this->VAO); 
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0); 
	glBindVertexArray(0);
}
