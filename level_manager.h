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

struct ActiveShapeProperties {
	GLuint posIndex;
	int counter;
	Clock globalTimer;
	Clock animTimer;
	glm::mat4 shapeModelMatrix;
};

enum class StateMachine { START, PLAY, GAME_OVER };

class LevelManager {
public:
	std::map<GLuint, PositionInfo> spawnPositions;
	std::list<ActiveShapeProperties>activeShapes;
	int selectedShape;
	Model* modelShapes;
	Animation animController;
	StateMachine currentState;
	Font font;
	LevelManager(const char* fontName);
	void renderShadows(GLuint programID, float deltaTime);
	void drawShapes(std::shared_ptr<Shader> shaderObj, glm::vec3 & cameraPos);
	void displayShapeText();
	void gameLoop();
private:
	const GLuint NUM_SHAPES = 30;
	int numCurrentShape;
	int totalShapes;
	const int STOP_COUNT = 6;
	float health = 100.0f;
	void createShapes();
	void initSpawnPositions();
	void firstSpawnShapes();
	void randomSpawnShapes();
	void setModelMatrix(GLuint position, std::list<ActiveShapeProperties>::reverse_iterator iter);
	int randomInt(int min, int max);
	float randomReal(float min, float max);
	GLuint findSpawnPositions() noexcept;
	void initPositionInfo(GLuint posIndex);
};

#endif

