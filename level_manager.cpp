#include "level_manager.h"
#include "utilities.h"
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

LevelManager::LevelManager()
{
	activeShapes.reserve(6);
	createShapes();
	initSpawnPositions();
	firstSpawnShapes();
	secondSpawnShapes();
}

void LevelManager::createShapes()
{
	constexpr GLuint TOTAL_TYPES = EnumToInt(Shapes3D::T_PYRAMID) + 1;
	const GLchar* shapes[TOTAL_TYPES] = { "C:\\Users\\Teboho\\Documents\\Projects\\ShapeCollecor_VS\\SOILTest\\OpenGlLTest\\cone2.obj", 
		"C:\\Users\\Teboho\\Documents\\Projects\\ShapeCollecor_VS\\SOILTest\\OpenGlLTest\\cube2.obj",
		"C:\\Users\\Teboho\\Documents\\Projects\\ShapeCollecor_VS\\SOILTest\\OpenGlLTest\\cylinder2.obj", 
		"C:\\Users\\Teboho\\Documents\\Projects\\ShapeCollecor_VS\\SOILTest\\OpenGlLTest\\ellipsoid2.obj", 
		"C:\\Users\\Teboho\\Documents\\Projects\\ShapeCollecor_VS\\SOILTest\\OpenGlLTest\\hemisphere2.obj",
		"C:\\Users\\Teboho\\Documents\\Projects\\ShapeCollecor_VS\\SOILTest\\OpenGlLTest\\rectangular_prism2.obj", 
		"C:\\Users\\Teboho\\Documents\\Projects\\ShapeCollecor_VS\\SOILTest\\OpenGlLTest\\rectangular_pyramid2.obj", 
		"C:\\Users\\Teboho\\Documents\\Projects\\ShapeCollecor_VS\\SOILTest\\OpenGlLTest\\sphere2.obj",
		"C:\\Users\\Teboho\\Documents\\Projects\\ShapeCollecor_VS\\SOILTest\\OpenGlLTest\\triangular_prism2.obj",  
		"C:\\Users\\Teboho\\Documents\\Projects\\ShapeCollecor_VS\\SOILTest\\OpenGlLTest\\triangular_pyramid2.obj"};
	modelShapes = new Model[NUM_SHAPES];
	GLuint index = 0;
	for (GLuint i = EnumToInt(Shapes3D::CONE); i < TOTAL_TYPES; ++i) {
		for (GLuint j = 0; j < 3; ++j) {
			modelShapes[index].shapeType = static_cast<Shapes3D>(i);
			modelShapes[index].loadModel(shapes[i]);
			++index;
		//	std::cout << "index = " << index << std::endl;
		}
	}
}

void LevelManager::initSpawnPositions()
{
	//Front Row
	PositionInfo p0 = { glm::vec3(-100.0f, 0.0f, -100.0f), nullptr };
	spawnPostions.insert(std::make_pair(0, p0));
	PositionInfo p1 = { glm::vec3(0.0f, 0.0f, -100.0f), nullptr };
	spawnPostions.insert(std::make_pair(1, p1));
	PositionInfo p2 = { glm::vec3(100.0f, 0.0f, -100.0f), nullptr };
	spawnPostions.insert(std::make_pair(2, p2));
	//Middel Row
	PositionInfo p3 = { glm::vec3(-100.0f, 0.0f, 0.0f), nullptr };
	spawnPostions.insert(std::make_pair(3, p3));
	PositionInfo p4 = { glm::vec3(0.0f, 0.0f, 0.0f), nullptr };
	spawnPostions.insert(std::make_pair(4, p4));
	PositionInfo p5 = { glm::vec3(100.0f, 0.0f, 0.0f), nullptr };
	spawnPostions.insert(std::make_pair(5, p5));
	//Back Row
	PositionInfo p6 = { glm::vec3(-100.0f, 0.0f, 100.0f), nullptr };
	spawnPostions.insert(std::make_pair(6, p6));
	PositionInfo p7 = { glm::vec3(0.0f, 0.0f, 100.0f), nullptr };
	spawnPostions.insert(std::make_pair(7, p7));
	PositionInfo p8 = { glm::vec3(100.0f, 0.0f, 100.0f), nullptr };
	spawnPostions.insert(std::make_pair(8, p8));
}

int LevelManager::randomSelection(int min, int max)
{
	// random number engine
	static std::default_random_engine e;
	//random number engine seed
	unsigned int seedValue = std::chrono::steady_clock::now().time_since_epoch().count();
	e.seed(seedValue);
	std::uniform_int_distribution<int> distr(min, max);
	return distr(e);
}

GLuint LevelManager::findSpawnPositions()
{
	GLuint positionIndex;
	while (true) {
		positionIndex = randomSelection(0, 8);
		if (spawnPostions[positionIndex].activeObject == nullptr) {
			printf("POS = %d ", positionIndex);
			break;
		}
	}
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
	activeShapes.push_back(positionIndex);
	printf("shape = %d , pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
	//Shape 2
	positionIndex = findSpawnPositions();
	index = selectedShape * STRIDE + 1;
	spawnPostions[positionIndex].activeObject = (modelShapes + index);
	activeShapes.push_back(positionIndex);
	printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
	//Shape 3
	positionIndex = findSpawnPositions();
	index = selectedShape * STRIDE + 2;
	spawnPostions[positionIndex].activeObject = (modelShapes + index);
	activeShapes.push_back(positionIndex);
	printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
}

void LevelManager::secondSpawnShapes()
{
	//Place 3 other shapes on the grid
	//Shape 4
	int randomSelectedShape = randomSelection(0, 29);
	GLuint positionIndex = findSpawnPositions();
	spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	activeShapes.push_back(positionIndex);
	printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
	//Shape 5
	randomSelectedShape = randomSelection(0, 29);
	positionIndex = findSpawnPositions();
	spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	activeShapes.push_back(positionIndex);
	printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
	//Shape 6
	randomSelectedShape = randomSelection(0, 29);
	positionIndex = findSpawnPositions();
	spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	activeShapes.push_back(positionIndex);
	printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
	//Shape 7
	randomSelectedShape = randomSelection(0, 29);
	positionIndex = findSpawnPositions();
	spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	activeShapes.push_back(positionIndex);
	printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
	//Shape 8
	randomSelectedShape = randomSelection(0, 29);
	positionIndex = findSpawnPositions();
	spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	activeShapes.push_back(positionIndex);
	printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
}



