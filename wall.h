#ifndef H_WALL
#define H_WALL

#include <GL/glew.h> 
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "mesh.h"

using std::vector;
using std::map;
using std::string;

struct Vec3
{
	float x;
	float y;
	float z;
};


class Wall
{
public:
	//std::map<std::string, float> extents;//wall boundaries and centre
	glm::vec3 normal;//wall plane normal
	Wall(glm::vec3 normal);
	void setUniforms(std::shared_ptr<Shader>& shaderObj, glm::vec3& cameraPos);
	void draw();

private:
	std::vector<Vec3> vertices;//vertices positions
	std::vector<Vec3> norms;//vertices normals
							//std::vector<glm::vec2> textureCoords;//vertices texture coordinates
	GLuint VBO;
	GLuint VAO;
};
#endif
