#ifndef H_WALL
#define H_WALL

#include <GL/glew.h> 
#include <vector>
#include <map>
#include <glm/glm.hpp>

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
	std::map<std::string, float> extents;//wall boundaries and centre
	glm::vec3 normal;//wall plane normal
	Wall(glm::vec3 normal, int type);
	//set postion of wall in the scene
	void setWorldPosition(glm::mat4 &model);
	void findWallExtents();
	void draw();

private:
	std::vector<glm::vec3> vertices;//vertices positions
	std::vector<glm::vec3> norms;//vertices normals
	std::vector<glm::vec2> textureCoords;//vertices texture coordinates
	GLuint VBO;
	GLuint VAO;
};
#endif
