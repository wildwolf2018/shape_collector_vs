#define GLEW_STATIC
#include <GL\glew.h>
#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <list>
#include <tuple>
#include <irrKlang.h>
#include "model.h"
#include "..\\Header Files\\animation.h"
#include "..\\Header Files\\time_clock.h"
#include "..\\Header Files\\font.h"
#include "..\\Header Files\\particle.h"
#include "..\\Header Files\\canvas.h"

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

using  namespace irrklang;

enum class StateMachine { START, PLAY, GAME_OVER, LEVEL_ENDING};
using Collision = std::tuple<GLboolean, glm::vec3, Shapes3D>;

class LevelManager {
public:
	std::map<GLuint, PositionInfo> spawnPositions;//Positions in the level where shapes are going to be spawned
	std::list<ActiveShapeProperties>activeShapes;
	int selectedShape;
	GLboolean screenFlash = false,displayMissionText = false, gameOver = false,roundEnding = false, showIntroText = true;;
	int totalShapes;
	float currentHealth;
	Clock globalLevelTimer,gameClock;
	Model* modelShapes;
	Animation animController;
	StateMachine currentState;
	std::vector<int>completedLevels;
	Font *font[2];
	Canvas *backfround;
	const char *sounds[10];
	ISoundEngine *SoundEngine = createIrrKlangDevice();
	GLboolean playSound = true;
	int soundIndex = 0;
	LevelManager();
	void renderShadows(GLuint programID, float deltaTime);
	void reset();
	void chooseShape();
	void drawShapes(std::shared_ptr<Shader> shaderObj, glm::vec3 & cameraPos);
	GLchar *getMissionTarget();
	void displayShapeText();
	void resultMessage(GLfloat deltaTime);
	void setHealthSlider(GLboolean start);
	void start();
	void showMissionText();
	void displayGoalText(glm::vec3 & position, GLchar * shape, std::string & missionText, std::string &wrappedText);
	Collision testCollision(glm::vec3 cameraPosition);
	void gameLoop();
	void applyPhysics(glm::mat4& projection, glm::mat4& view, glm::vec3& cameraPos, GLfloat deltaTime);
	GLboolean roundOver();
	void gameOverReset();
	void displayRoundEndText();
private:
	Particle* explosion;
	const GLuint NUM_SHAPES = 30;
	int numCurrentShape;
	const int STOP_COUNT = 6;
	const int SHAPE_TYPE_COUNT = 10;
	float maximumHealth = 150.0f;
	int blinkingEndCount = 0;
	GLboolean shouldTest = true;
	Clock particleTimer, barTimer;
	std::string messageRsults[5];
	int messageIndex;
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

