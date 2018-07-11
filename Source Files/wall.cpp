#include <iostream>
#include <vector>
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "..\\Header Files\\resource_manager.h"
#include "..\\Header Files\\wall.h"

Wall::Wall(glm::vec3 _normal, int type)
	:normal{ _normal }
{
	//initilize the vertices
	vertices.push_back(glm::vec3(-10.0f, 2.5f, 0.0f));
	vertices.push_back(glm::vec3(-10.0f, -2.5f, 0.0f));
	vertices.push_back(glm::vec3(10.0f, 2.5f, 0.0f));
	vertices.push_back(glm::vec3(10.0f, -2.5f, 0.0f));
	if (type == 1 || type == 2) {
		textureCoords.push_back(glm::vec2(0.0f, 0.5f));
		textureCoords.push_back(glm::vec2(0.0f, 0.0f));
		textureCoords.push_back(glm::vec2(5.0f, 0.5f));
		textureCoords.push_back(glm::vec2(5.0f, 0.0f));
	}
	else if (type == 3)
	{
		textureCoords.push_back(glm::vec2(0.0f, 1.0f));
		textureCoords.push_back(glm::vec2(0.0f, 0.0f));
		textureCoords.push_back(glm::vec2(1.0f, 1.0f));
		textureCoords.push_back(glm::vec2(1.0f, 0.0f));
	}
	norms.push_back(normal);
	norms.push_back(normal);
	norms.push_back(normal);
	norms.push_back(normal);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) + textureCoords.size() * sizeof(glm::vec2)
		+ norms.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), &vertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), norms.size() * sizeof(glm::vec3), &norms[0]);
	glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3) + norms.size() * sizeof(glm::vec3),
		textureCoords.size() * sizeof(glm::vec2), &textureCoords[0]);
	//create vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//map index 0 to vertex postion in buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)(vertices.size() * sizeof(glm::vec3)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (GLvoid*)(vertices.size() * sizeof(glm::vec3) + norms.size()
		* sizeof(glm::vec3)));
	//enable vertex attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}

void Wall::setWorldPosition(glm::mat4 &model)
{
	for (int index = 0; index < vertices.size(); index++)
		vertices[index] = glm::vec3(model * glm::vec4(vertices[index], 1.0f));
}

void Wall::findWallExtents()
{
	float minX = vertices[0].x, maxX = minX;
	float minY = vertices[0].y, maxY = minY;
	float minZ = vertices[0].z, maxZ = minZ;

	for (int i = 1; i < vertices.size(); i++)
	{
		if (vertices[i].x < minX)
			minX = vertices[i].x;
		if (vertices[i].x > maxX)
			maxX = vertices[i].x;
		if (vertices[i].y < minY)
			minY = vertices[i].y;
		if (vertices[i].y > maxY)
			maxY = vertices[i].y;
		if (vertices[i].z < minZ)
			minZ = vertices[i].z;
		if (vertices[i].z > maxZ)
			maxZ = vertices[i].z;
	}
	extents.insert({ "min_x", minX });
	extents.insert({ "max_x", maxX });
	extents.insert({ "min_y", minY });
	extents.insert({ "max_y", maxY });
	extents.insert({ "min_z", minZ });
	extents.insert({ "max_z", maxZ });
	extents.insert({ "centreX", (extents["max_x"] - extents["min_x"]) / 2 });
	extents.insert({ "centreY", (extents["max_y"] - extents["min_y"]) / 2 });
	extents.insert({ "centreZ", (extents["max_z"] - extents["min_z"]) / 2 });
}

void Wall::draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}