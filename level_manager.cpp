#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <sstream>
#include "level_manager.h"
#include "utilities.h"
#include "resource_manager.h"

LevelManager::LevelManager(const char* fontName):font{ fontName }, animController{9}, numCurrentShape{0}, totalShapes{0}
{
	currentState = StateMachine::START;
	createShapes();
	initSpawnPositions();
}

void LevelManager::gameLoop()
{
	switch (currentState)
	{
	case StateMachine::START:
		currentState = StateMachine::PLAY;
		break;
	case StateMachine::PLAY:
		randomSpawnShapes();
		//displayShapeText();
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
		}
	}
}

void LevelManager::initSpawnPositions()
{
	//Front Row
	PositionInfo p0 = { glm::vec3(-100.0f, 0.0f, -100.0f), nullptr };
	spawnPositions.insert(std::make_pair(0, p0));
	PositionInfo p1 = { glm::vec3(0.0f, 0.0f, -100.0f), nullptr };
	spawnPositions.insert(std::make_pair(1, p1));
	PositionInfo p2 = { glm::vec3(100.0f, 0.0f, -100.0f), nullptr };
	spawnPositions.insert(std::make_pair(2, p2));
	//Middel Row
	PositionInfo p3 = { glm::vec3(-100.0f, 0.0f, 0.0f), nullptr };
	spawnPositions.insert(std::make_pair(3, p3));
	PositionInfo p4 = { glm::vec3(0.0f, 0.0f, 0.0f), nullptr };
	spawnPositions.insert(std::make_pair(4, p4));
	PositionInfo p5 = { glm::vec3(100.0f, 0.0f, 0.0f), nullptr };
	spawnPositions.insert(std::make_pair(5, p5));
	//Back Row
	PositionInfo p6 = { glm::vec3(-100.0f, 0.0f, 100.0f), nullptr };
	spawnPositions.insert(std::make_pair(6, p6));
	PositionInfo p7 = { glm::vec3(0.0f, 0.0f, 100.0f), nullptr };
	spawnPositions.insert(std::make_pair(7, p7));
	PositionInfo p8 = { glm::vec3(100.0f, 0.0f, 100.0f), nullptr };
	spawnPositions.insert(std::make_pair(8, p8));
}

int LevelManager::randomInt(int min, int max)
{
	// random number engine
	static std::default_random_engine e;
	//random number engine seed
	unsigned int seedValue = std::chrono::steady_clock::now().time_since_epoch().count();
	e.seed(seedValue);
	std::uniform_int_distribution<int> distr(min, max);
	return distr(e);
}

float LevelManager::randomReal(float min, float max)
{
	// random number engine
	static std::default_random_engine e;
	//random number engine seed
	unsigned int seedValue = std::chrono::steady_clock::now().time_since_epoch().count();
	e.seed(seedValue);
	std::uniform_real_distribution<float> distr(min, max);
	return distr(e);
}

GLuint LevelManager::findSpawnPositions() noexcept
{
	GLuint positionIndex;
	while (true) {
		positionIndex = randomInt(0, 8);
		if (spawnPositions[positionIndex].activeObject == nullptr) {
			break;
		}
	}
	return positionIndex;
}

void LevelManager::initPositionInfo(GLuint posIndex)
{
	activeShapes.emplace_back(ActiveShapeProperties{ 0, 0, Clock(), Clock(), glm::mat4() });
	auto p = activeShapes.rbegin();
	setModelMatrix(posIndex, p);
	p->posIndex = posIndex;
	p->globalTimer.startClock();
	p->animTimer.startClock();
	p->animTimer.addTime();
	float offset = randomReal(2.0, 5.0f);
	float lifeSpan = p->animTimer.getElapsedTime() + offset;
	animController.setEndTime(posIndex, lifeSpan);
}

void LevelManager::firstSpawnShapes()
{
	selectedShape = randomInt(0, 9);
	GLuint positionIndex;
	constexpr unsigned int STRIDE = 3;

	//First place 3 shapes of the selected type on the level
	for (int i = 0; i < 3; ++i) {
		positionIndex = findSpawnPositions();
		int index = selectedShape * STRIDE + i;
		spawnPositions[positionIndex].activeObject = (modelShapes + index);
		initPositionInfo(positionIndex);
		
	}
	
	//Place 3 other shapes on the level
	for (int i = 1; i <= 3; ++i) {
		int randomSelectedShape = randomInt(0, 29);
		positionIndex = findSpawnPositions();
		spawnPositions[positionIndex].activeObject = &modelShapes[randomSelectedShape];
		initPositionInfo(positionIndex);
	}
}

void LevelManager::randomSpawnShapes()
{
	std::size_t listSize = activeShapes.size();
	if (listSize > 0 && listSize < 6){
		GLuint pos;
		constexpr int STRIDE = 3;
		int randomSelectedShape;
		auto num = std::count_if(std::begin(activeShapes), std::end(activeShapes), 
			[&position = spawnPositions, &shapeType = selectedShape](ActiveShapeProperties& shape) 
			{ return position[shape.posIndex].activeObject->shapeType == static_cast<Shapes3D>(shapeType); });
	    if (num == 0) {
			for (int i = 0; i < 2; ++i) {
				pos = findSpawnPositions();
				int index = selectedShape * STRIDE + i;
				spawnPositions[pos].activeObject = (modelShapes + index);
				initPositionInfo(pos);
			}
		}
		else if (num == 3){
			do {
				randomSelectedShape = randomInt(0, 29);
			} while (randomSelectedShape == selectedShape);
			pos = findSpawnPositions();
			spawnPositions[pos].activeObject = &modelShapes[randomSelectedShape];
			initPositionInfo(pos);
		}
		else if ((num >= 0 && num < 3) || num > 3) {
			randomSelectedShape = randomInt(0, 29);
			pos = findSpawnPositions();
			spawnPositions[pos].activeObject = &modelShapes[randomSelectedShape];
			initPositionInfo(pos);
		}//if-else
	}
	else if(listSize == 0) {
		firstSpawnShapes();
	}//if-else
}//randomSpawnShapes

void LevelManager::setModelMatrix(GLuint position, std::list<ActiveShapeProperties>:: reverse_iterator iter)
{
	Shapes3D type = spawnPositions[position].activeObject->shapeType;
	glm::mat4 modelMatrix;

	//Place shape in correct world position
	switch (type)
	{
	case Shapes3D::R_PRISM:	spawnPositions[position].worldPosition.y = 1.8f;
			modelMatrix = glm::translate(modelMatrix, spawnPositions[position].worldPosition);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
			break;
	case Shapes3D::T_PRISM: spawnPositions[position].worldPosition.y = 1.0f;
			modelMatrix = glm::translate(modelMatrix, spawnPositions[position].worldPosition);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(3.0f, 3.0f, 3.0f));
			break;
	case Shapes3D::R_PYRAMID:spawnPositions[position].worldPosition.y = 2.5f;
			modelMatrix = glm::translate(modelMatrix, spawnPositions[position].worldPosition);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			break;
	case Shapes3D::ELLIPSOID:
	case Shapes3D::T_PYRAMID: spawnPositions[position].worldPosition.y = 1.0f;
			modelMatrix = glm::translate(modelMatrix, spawnPositions[position].worldPosition);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
			break;
	default:spawnPositions[position].worldPosition.y = 0.0f;
			modelMatrix = glm::translate(modelMatrix, spawnPositions[position].worldPosition);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(5.0f, 5.0f, 5.0f));
	}
	iter->shapeModelMatrix = modelMatrix;
}//setModelMatrix

void LevelManager::renderShadows(GLuint programID, float deltaTime)
{
	auto iter = activeShapes.begin();
	while (iter != activeShapes.end()) {
		int index = iter->posIndex;
		PositionInfo temp = spawnPositions[index];
		int blinkCounter = iter->counter;
		if (blinkCounter >= 0 && blinkCounter <= STOP_COUNT) {
			iter->globalTimer.addTime();
			iter->animTimer.addTime();
			if (iter->animTimer.getElapsedTime() < animController.endTimes[index]) {
				iter->shapeModelMatrix = glm::rotate(iter->shapeModelMatrix, 1.0f * deltaTime, glm::vec3(1.0f, 1.0f, 1.0f));
				glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, glm::value_ptr(iter->shapeModelMatrix));
				temp.activeObject->Draw();
			}
			else {
				if (iter->globalTimer.getElapsedTime() > 1.4f) {
					animController.endTimes[index] = 1.0f;
					iter->animTimer.stopClock();
					iter->animTimer.startClock();
					iter->globalTimer.stopClock();
					iter->globalTimer.startClock();
					iter->counter += 1;
				}//if
			}//if-else
		}//if
		if (blinkCounter < 0 || blinkCounter > STOP_COUNT) {
			spawnPositions[iter->posIndex].activeObject = nullptr;
			iter = activeShapes.erase(iter);
		}
		else {
			++iter;
		}
	}//for loop
}//:renderShadows

void LevelManager::drawShapes(std::shared_ptr<Shader>shaderObject, glm::vec3& cameraPos)
{
	for (auto iter = activeShapes.begin(); iter != activeShapes.end(); ++iter) {
		int counter = iter->counter;
		if (counter >= 0 && counter <= STOP_COUNT)
		{
			iter->globalTimer.addTime();
			iter->animTimer.addTime();
			shaderObject->setMatrix(iter->shapeModelMatrix, "model");
			int index = iter->posIndex;
			float passedTime = iter->animTimer.getElapsedTime();
			if (passedTime < animController.endTimes[index]) {
				PositionInfo p = spawnPositions[index];
				p.activeObject->setLightUniforms(shaderObject, cameraPos);
				p.activeObject->Draw();
			}//if
		}//if
	}//for loop
}//drawShapes

void LevelManager::displayShapeText()
{
	std::string shape{""};
	switch (selectedShape)
	{
	case 0:shape = "CONE";break;
	case 1:shape = "CUBE";break;
	case 2:shape = "CYLINDER";break;
	case 3:shape = "ELLIPSOID";break;
	case 4:shape = "HEMISPHERE";break;
	case 5:shape = "RECTANGULAR PRISM"; break;
	case 6:shape = "RECTANGULAR PYRAMID"; break;
	case 7:shape = "SPHERE"; break;
	case 8:shape = "TRIANGULAR PRISM"; break;
	case 9:shape = "TRIANGULAR PYRAMID"; break;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	std::ostringstream resultsDisplayText, healthDisplayText;
	resultsDisplayText << shape << " X " << numCurrentShape;
	//health -= 1.0f;
	healthDisplayText << "HEALTH " << health;
	std::string s_health{ healthDisplayText.str() };
	std::string temp{ resultsDisplayText.str() };
	font.renderCharacters(ResourceManager::GetShader("fontShader"), s_health, 5.0f, 540.0f, 0.3f, glm::vec3(1.0f, 0.84f, 0.0f));
	font.renderCharacters(ResourceManager::GetShader("fontShader"), temp, 5.0f, 520.0f, 0.3f, glm::vec3(1.0f, 0.84f, 0.0f));
	glDisable(GL_BLEND);
}//displayShapeText


