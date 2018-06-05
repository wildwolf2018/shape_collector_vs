#include "level_manager.h"
#include "utilities.h"
#include "resource_manager.h"
#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <sstream>

LevelManager::LevelManager(const char* fontName):font{ fontName }, animController{9}, numCurrentShape{0}, totalShapes{0}
{
	currentState = StateMachine::START;
	activeShapes.reserve(5);
	createShapes();
	initSpawnPositions();
	animTimers.reserve(5);
	counters.reserve(5);
}

void LevelManager::gameLoop()
{
	switch (currentState)
	{
	case StateMachine::START:
		firstSpawnShapes();
		secondSpawnShapes();
		setModelMatrices();
		currentState = StateMachine::PLAY;
		break;
	case StateMachine::PLAY:
	//	displayShapeText(selectedShape);
		break;
	default:
		break;
	}
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
	animTimers.emplace_back(Clock());
	animTimers[0].startClock();
	globalTimer[0].startClock();
	animTimers[0].addTime();
	animController.setEndTime(positionIndex, animTimers[0].getElapsedTime());
	counters.push_back(0);
	////Shape 2
	//positionIndex = findSpawnPositions();
	//index = selectedShape * STRIDE + 1;
	//spawnPostions[positionIndex].activeObject = (modelShapes + index);
	//activeShapes.push_back(positionIndex);
	//printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
	////Shape 3
	//positionIndex = findSpawnPositions();
	//index = selectedShape * STRIDE + 2;
	//spawnPostions[positionIndex].activeObject = (modelShapes + index);
	//activeShapes.push_back(positionIndex);
	//printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
}

void LevelManager::secondSpawnShapes()
{
	//Place 3 other shapes on the grid
	//Shape 4
	//int randomSelectedShape = randomSelection(0, 29);
	//GLuint positionIndex = findSpawnPositions();
	//spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	//activeShapes.push_back(positionIndex);
	//printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
	////Shape 5
	//randomSelectedShape = randomSelection(0, 29);
	//positionIndex = findSpawnPositions();
	//spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	//activeShapes.push_back(positionIndex);
	//printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
	//Shape 6
	//randomSelectedShape = randomSelection(0, 29);
	//positionIndex = findSpawnPositions();
	//spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	//activeShapes.push_back(positionIndex);
	//printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
	////Shape 7
	//randomSelectedShape = randomSelection(0, 29);
	//positionIndex = findSpawnPositions();
	//spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	//activeShapes.push_back(positionIndex);
	//printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
	////Shape 8
	//randomSelectedShape = randomSelection(0, 29);
	//positionIndex = findSpawnPositions();
	//spawnPostions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
	//activeShapes.push_back(positionIndex);
	//printf("shape = %d ,  pos = %d\n", spawnPostions[positionIndex].activeObject->shapeType, positionIndex);
}

void LevelManager::setModelMatrices()
{
	PositionInfo temp;
	for (unsigned int i = 0; i < activeShapes.size(); ++i) {
		temp = spawnPostions[activeShapes[i]];
		if (temp.activeObject != nullptr) {
			setModelMatrix(temp);
		}
	}
}

void LevelManager::setModelMatrix(PositionInfo& spawnPosition)
{
	Model* shape = spawnPosition.activeObject;
	Shapes3D type = shape->shapeType;
	glm::mat4 modelMatrix;

	//Place shape in correct world position
	switch (type)
	{
	case Shapes3D::R_PRISM:
		spawnPosition.worldPosition.y = 1.8f;
		modelMatrix = glm::translate(modelMatrix, spawnPosition.worldPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
		shapeModelMatrices.push_back(modelMatrix);
		break;
	case Shapes3D::T_PRISM:
		spawnPosition.worldPosition.y = 1.0f;
		modelMatrix = glm::translate(modelMatrix, spawnPosition.worldPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
		shapeModelMatrices.push_back(modelMatrix);
		break;
	case Shapes3D::R_PYRAMID:
		spawnPosition.worldPosition.y = 2.5f;
		modelMatrix = glm::translate(modelMatrix, spawnPosition.worldPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
		shapeModelMatrices.push_back(modelMatrix);
		break;
	case Shapes3D::ELLIPSOID:
	case Shapes3D::T_PYRAMID:
		spawnPosition.worldPosition.y = 1.0f;
		modelMatrix = glm::translate(modelMatrix, spawnPosition.worldPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
		shapeModelMatrices.push_back(modelMatrix);
		break;
	default:
		spawnPosition.worldPosition.y = 0.0f;
		modelMatrix = glm::translate(modelMatrix, spawnPosition.worldPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
		shapeModelMatrices.push_back(modelMatrix);
	}
}

void LevelManager::renderShadows(GLuint programID, float deltaTime)
{
	for (unsigned int i = 0; i < activeShapes.size(); ++i) {
		int index = activeShapes[i];
		PositionInfo temp = spawnPostions[index];
		if (counters[i] <= STOP_COUNT) {
			globalTimer[i].addTime();
			animTimers[i].addTime();
			if (animTimers[i].getElapsedTime() < animController.endTimes[index]) {
				shapeModelMatrices[i] = glm::rotate(shapeModelMatrices[i], 1.0f * deltaTime, glm::vec3(1.0f, 1.0f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, glm::value_ptr(shapeModelMatrices[i]));
				temp.activeObject->Draw();
			}
			else {
				if (globalTimer[i].getElapsedTime() > 1.5f) {
					animController.endTimes[index] = 1.0f;
					animTimers[i].stopClock();
					animTimers[i].startClock();
					globalTimer[i].stopClock();
					globalTimer[i].startClock();
					++counters[i];
				}//if
			}//if-else
		}//if
		else {
			temp.activeObject = nullptr;
		}
	}//for loop
}

void LevelManager::drawShapes(std::shared_ptr<Shader>shaderObject, glm::vec3& cameraPos)
{
	for (unsigned int i = 0; i < activeShapes.size(); ++i) {
		if (counters[i] <= STOP_COUNT) {
			globalTimer[i].addTime();
			animTimers[i].addTime();
			shaderObject->setMatrix(shapeModelMatrices[i], "model");
			int index = activeShapes[i];
			float passedTime = animTimers[i].getElapsedTime();
			if (passedTime < animController.endTimes[index]) {
				PositionInfo p = spawnPostions[index];
				p.activeObject->setLightUniforms(shaderObject, cameraPos);
				p.activeObject->Draw();
			}//if
		}//if
	}//for loop
}

void LevelManager::displayShapeText()
{
	std::string shape{""};
	switch (selectedShape)
	{
	case 0:shape = "CONES";break;
	case 1:shape = "CUBES";break;
	case 2:shape = "CYLINDERS";break;
	case 3:shape = "ELLIPSOIDS";break;
	case 4:shape = "HEMISPHERE";break;
	case 5:shape = "R_PRISM"; break;
	case 6:shape = "R_PYRAMID"; break;
	case 7:shape = "SPHERE"; break;
	case 8:shape = "T_PRISM"; break;
	case 9:shape = "T_PYRAMID"; break;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::ostringstream resultsDisplayText;
	resultsDisplayText << shape << " X " << numCurrentShape;
	std::string temp{ resultsDisplayText.str() };
	font.renderCharacters(ResourceManager::GetShader("fontShader"), temp, 250.0f, 300.0f, 1.0f, glm::vec3(1.0f, 0.84f, 0.0f));
	glDisable(GL_BLEND);
}


