#include "level_manager.h"
#include "utilities.h"
#include <random>
#include <chrono>
#include <algorithm>

LevelManager::LevelManager()
{
	createShapes();
	initSpawnPositions();
	firstSpawnShapes();
	secondSpawnShapes();
}

void LevelManager::createShapes()
{
	constexpr GLuint TOTAL_TYPES = EnumToInt(Shapes3D::T_PYRAMID) + 1;
	const GLchar* shapes[TOTAL_TYPES] = { ".\\OpenGlLTest\\cone2.obj", ".\\OpenGlLTest\\cube2.obj",
		".\\OpenGlLTest\\cylinder2.obj", ".\\OpenGlLTest\\ellipsoid2.obj", ".\\OpenGlLTest\\hemisphere2.obj",
		".\\OpenGlLTest\\rectangular_prism2.obj", ".\\OpenGlLTest\\rectangular_pyramid2.obj", ".\\OpenGlLTest\\sphere2.obj",
		".\\OpenGlLTest\\triangular_prism2.obj",  ".\\OpenGlLTest\\triangluar_pyramid2.obj" };
	modelShapes = new Model[NUM_SHAPES];
	GLuint index = 0;
	for (GLuint i = EnumToInt(Shapes3D::CONE); i < TOTAL_TYPES; ++i) {
		for (GLuint j = 0; j < 3; ++j) {
			modelShapes[index].shapeType = static_cast<Shapes3D>(i);
			modelShapes[index].loadModel(shapes[i]);
			++index;
		}
	}
}

void LevelManager::initSpawnPositions()
{
	//Front Row
	PostionInfo p0 = { glm::vec3(-100.0f, 0.0f, -100.0f), nullptr };
	spawnPostions.insert(std::make_pair(0, p0));
	PostionInfo p1 = { glm::vec3(0.0f, 0.0f, -100.0f), nullptr };
	spawnPostions.insert(std::make_pair(1, p1));
	PostionInfo p2 = { glm::vec3(100.0f, 0.0f, -100.0f), nullptr };
	spawnPostions.insert(std::make_pair(2, p2));
	//Middel Row
	PostionInfo p3 = { glm::vec3(100.0f, 0.0f, 0.0f), nullptr };
	spawnPostions.insert(std::make_pair(3, p3));
	PostionInfo p4 = { glm::vec3(0.0f, 0.0f, 0.0f), nullptr };
	spawnPostions.insert(std::make_pair(4, p4));
	PostionInfo p5 = { glm::vec3(100.0f, 0.0f, 0.0f), nullptr };
	spawnPostions.insert(std::make_pair(5, p5));
	//Back Row
	PostionInfo p6 = { glm::vec3(-100.0f, 0.0f, 100.0f), nullptr };
	spawnPostions.insert(std::make_pair(6, p6));
	PostionInfo p7 = { glm::vec3(0.0f, 0.0f, 100.0f), nullptr };
	spawnPostions.insert(std::make_pair(7, p7));
	PostionInfo p8 = { glm::vec3(100.0f, 0.0f, 100.0f), nullptr };
	spawnPostions.insert(std::make_pair(8, p8));
}

int LevelManager::randomSelection(int min, int max)
{
	// random number engine
	static std::default_random_engine e;
	//random number engine seed
	unsigned int seedValue = std::chrono::steady_clock::now().time_since_epoch().count();
	e.seed(seedValue);
	std::uniform_int_distribution<int> distr(0, 9);
	return distr(e);
}

GLuint LevelManager::findSpawnPositions()
{
	GLuint positionIndex;
	do {
		positionIndex = randomSelection(0, 8);
	} while (spawnPostions[positionIndex].activeObject != nullptr);
	return positionIndex;
}

void LevelManager::firstSpawnShapes()
{
	selectedShape = randomSelection(0, 9);
	GLuint positionIndex;
	constexpr unsigned int STRIDE = 3;
	//First place 3 shapes of the selected type on the level
	//Shape 1
	positionIndex = findSpawnPositions();
	int index = selectedShape * STRIDE;
	spawnPostions[positionIndex].activeObject = (modelShapes + index);
	activeShapes.push_back(index);
	//Shape 2
	positionIndex = findSpawnPositions();
	index = selectedShape * STRIDE + 1;
	spawnPostions[positionIndex].activeObject = (modelShapes + index);
	activeShapes.push_back(index);
	//Shape 3
	positionIndex = findSpawnPositions();
	index = selectedShape * STRIDE + 2;
	spawnPostions[positionIndex].activeObject = (modelShapes + index);
	activeShapes.push_back(index);
}

void LevelManager::secondSpawnShapes()
{
	//Place 3 other shapes on the grid
	//Shape 4
	int randomSelectedShape = randomSelection(0, 29);
	GLuint positionIndex = findSpawnPositions();
	spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	activeShapes.push_back(randomSelectedShape);
	//Shape 5
	randomSelectedShape = randomSelection(0, 29);
	positionIndex = findSpawnPositions();
	spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	activeShapes.push_back(randomSelectedShape);
	//Shape 6
	randomSelectedShape = randomSelection(0, 29);
	positionIndex = findSpawnPositions();
	spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	activeShapes.push_back(randomSelectedShape);
}



