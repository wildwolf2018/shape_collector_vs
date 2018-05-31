#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <list>
#include "model.h"

struct PostionInfo {
	glm::vec3 worldPosition;
	Model* activeObject;
};

class LevelManager {
public:
	LevelManager();
	std::map<GLuint, PostionInfo> spawnPostions;
	Model* modelShapes;
private:
	const GLuint NUM_SHAPES = 30;
	int selectedShape;
	unsigned int objectAnimStates = 0x00000000;
	unsigned int availablePositions;
	std::list<GLuint>activeShapes;
	void createShapes();
	void initSpawnPositions();
	void firstSpawnShapes();
	void secondSpawnShapes();
	int randomSelection(int min, int max);
	GLuint findSpawnPositions();
};

#endif

