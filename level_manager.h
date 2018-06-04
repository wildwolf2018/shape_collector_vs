#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <list>
#include "model.h"
#include "animation.h"
#include "time_clock.h"
#include "font.h"

struct PositionInfo {
	glm::vec3 worldPosition;
	Model* activeObject;
};

enum class StateMachine { START, PLAY, GAME_OVER };

class LevelManager {
public:
	LevelManager(const char* fontName);
	std::map<GLuint, PositionInfo> spawnPostions;
	std::vector<GLuint>activeShapes;
	Model* modelShapes;
	Animation animController;
	StateMachine currentState;
	Font font;
	std::vector<glm::mat4>  shapeModelMatrices;
	void createShapes(GLuint programID, float deltaTime);
	void drawShapes(std::shared_ptr<Shader> shaderObj, glm::vec3 & cameraPos);
	void displayShapeText();
	void gameLoop();
private:
	const GLuint NUM_SHAPES = 30;
	int selectedShape;
	int numCurrentShape;
	int totalShapes;
	Clock globalTimer;
	void createShapes();
	void initSpawnPositions();
	void firstSpawnShapes();
	void secondSpawnShapes();
	void setModelMatrices();
	void setModelMatrix(PositionInfo & spawnPosition);
	int randomSelection(int min, int max);
	GLuint findSpawnPositions();
};

#endif

