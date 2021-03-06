#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <sstream>
#include <string.h>
#include <irrKlang.h>
#include "..\\Header Files\\level_manager.h"
#include "..\\Header Files\\utilities.h"
#include "..\\Header Files\\resource_manager.h"

using namespace irrklang;

LevelManager::LevelManager() :animController{ 9 }, particleTimer{}, globalLevelTimer{}, backfround(new Canvas)
{
	//font = new Font[2]
	font[0] = new Font("Fonts\\ARCADE.ttf");
	font[1] = new Font("Fonts\\zorque.ttf");
	
	sounds[0] = "Sounds\\Bouncing-Around-Town.mp3";
	sounds[1] = "Sounds\\haunted-forest.mp3";
	sounds[2] = "Sounds\\Frantic-Gameplay.mp3";
	sounds[3] = "Sounds\\Grunge-Street-Game.mp3";
	sounds[4] = "Sounds\\moment-of-strange.mp3";
	sounds[5] = "Sounds\\Bells-of-Weirdness.mp3";
	sounds[6] = "Sounds\\Magical-Day.mp3";
	sounds[7] = "Sounds\\Techno-Gameplay_Looping.mp3";
	sounds[8] = "Sounds\\Theyre-Closing-In_Looping.mp3";
	sounds[9] = "Sounds\\Torment_Looping.mp3";
	int numberOfParticles = 100;
	totalShapes = 0;
	currentHealth = maximumHealth;
	messageRsults[0] = "    EXCELLENT!";
	messageRsults[1] = "OUTSTANDING!";
	messageRsults[2] = "       SUPERB!";
	messageRsults[3] = "       GOOD!";
	messageRsults[4] = "          OOPS!";
	explosion = new Particle{ "Shaders\\vshader.txt", "Shaders\\fshader.txt", "red_particle.png", numberOfParticles };
	createShapes();
	initSpawnPositions();
	currentState = StateMachine::START;
	completedLevels.reserve(10);
}

void LevelManager::gameLoop()
{
	int completedRounds = 0;
	switch (currentState)
	{
	case StateMachine::START:
		if (playSound) {
			if (soundIndex > 9)
				soundIndex = 0;
			SoundEngine->play2D(sounds[soundIndex], GL_TRUE);
			playSound = false;
		}
		start();
		break;
	case StateMachine::PLAY:
		showIntroText = false;
		randomSpawnShapes();
		break;
	case StateMachine::LEVEL_ENDING:
		++soundIndex;
		globalLevelTimer.stopClock();
		completedRounds = completedLevels.size();
		std::cout << "rounds = " << completedRounds << std::endl;
		if (completedRounds < SHAPE_TYPE_COUNT) {
			gameClock.startClock();
			gameClock.timer = 5.01f;
			displayMissionText = false;
			currentState = StateMachine::START;
			SoundEngine->stopAllSounds();
			playSound = true;
		}
		else {
			currentState = StateMachine::GAME_OVER;
		}	
		break;
	case StateMachine::GAME_OVER:
		SoundEngine->stopAllSounds();
		globalLevelTimer.stopClock();
		if (completedLevels.size() != 0) {
			completedLevels.clear();
			displayMissionText = false;
		}
		playSound = true;
	}
}

void LevelManager::applyPhysics(glm::mat4& projection, glm::mat4& view, glm::vec3& cameraPos, GLfloat deltaTime)
{
	Collision result;
	if ( !Particle::isAnimPlaying && !roundEnding) {
		result = testCollision(cameraPos);
		if (std::get<0>(result)) {
			SoundEngine->play2D("Sounds\\Bells2.mp3", GL_FALSE);
			glm::vec3 spawnPoint = std::get<1>(result);
			explosion->initParticles(spawnPoint);
			if (std::get<2>(result) == static_cast<Shapes3D>(selectedShape)) {
				numCurrentShape += 1;
				totalShapes += 1;
				if(currentHealth > 0)
					currentHealth += 20.0f;
				if (currentHealth > maximumHealth)
					currentHealth = maximumHealth;
				messageIndex = randomInt(0, 3);
			}
			else {
				currentHealth -= 10.0f;
				messageIndex = 4;
			}
			particleTimer.startClock();
		}
	}
	particleTimer.addTime();
	GLfloat timer = particleTimer.getElapsedTime();
	if (!particleTimer.stopTime && timer <= 1.0f) {
		Texture2D particleTexture = ResourceManager::GetTexture("particle");
		std::shared_ptr<Shader> particleShaderObj = ResourceManager::GetShader("particleShader");
		particleShaderObj->Use();
		explosion->setUniforms(projection, view, particleShaderObj, timer, deltaTime);
		explosion->drawParticle();
		screenFlash = true;
	}
}//applyPhysics

GLboolean LevelManager::roundOver()
{
	if (globalLevelTimer.getElapsedTime() > 60.0f) {
		roundEnding = true;
		return true;
	}
	roundEnding = false;
	return false;
}

void LevelManager::gameOverReset()
{
	gameOver = true;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::vec3 startTextColor(1.0f, 0.0f, 0.0f), posAndScale(220.0f, 300.0f, 1.5f);
	font[0]->renderCharacters(ResourceManager::GetShader("fontShader"), "GAME OVER",
		posAndScale.x, posAndScale.y, posAndScale.z, startTextColor);
	glm::vec3 resultTextColor(0.0f, 1.0f, 0.0f), resultsTextPos(170.0f, 200.0f, 1.0f);
	std::ostringstream resultsDisplayText;
	resultsDisplayText << "TOTAL COLLECTION " << totalShapes;
	std::string totalShapesText{ resultsDisplayText.str() };
	font[0]->renderCharacters(ResourceManager::GetShader("fontShader"), totalShapesText,
		resultsTextPos.x, resultsTextPos.y, resultsTextPos.z, resultTextColor);
	
	glm::vec3 restartTextColor(1.0f, 1.0f, 1.0f), restartTextPos(10.0f, 20.0f, 0.5f);
	static Clock startTextTimer{};
	if (startTextTimer.timer == 0)
		startTextTimer.startClock();
	startTextTimer.addTime();
	float timer = startTextTimer.getElapsedTime();
	if (timer > 1.0f && timer <= 2.0f) {
		font[0]->renderCharacters(ResourceManager::GetShader("fontShader"), "Press X to start",
			restartTextPos.x, restartTextPos.y, restartTextPos.z, restartTextColor);
	}
	else if (timer > 2.0f) {
		startTextTimer.stopClock();
	}
	glDisable(GL_BLEND);
}

void LevelManager::displayRoundEndText()
{
	if (Particle::isAnimPlaying)return;
	static Clock textDisplayTimer{};
	if (textDisplayTimer.timer == 0 && blinkingEndCount < 5)
		textDisplayTimer.startClock();
	textDisplayTimer.addTime();
	float timer = textDisplayTimer.getElapsedTime();
	if (timer > 1.0f && timer <= 2.0f) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::vec3 startTextColor(0.94f, 1.0f, 0.29f), posAndScale(90.0f, 300.0f, 1.5f);
		font[0]->renderCharacters(ResourceManager::GetShader("fontShader"), "MISSION COMPLETED",
			posAndScale.x, posAndScale.y, posAndScale.z, startTextColor);
		glDisable(GL_BLEND);
	}
	else if (timer > 2.0f) {
		textDisplayTimer.stopClock();
		++blinkingEndCount;
	}
}

void LevelManager::createShapes()
{
	constexpr GLuint TOTAL_TYPES = EnumToInt(Shapes3D::T_PYRAMID) + 1;
	const GLchar* shapes[TOTAL_TYPES] = { ".//Models//cone2.obj", ".//Models//cube2.obj",".//Models//cylinder2.obj", ".//Models//ellipsoid2.obj", 
		".//Models//hemisphere2.obj",".//Models//rectangular_prism2.obj",".//Models//rectangular_pyramid2.obj", ".//Models//sphere2.obj",
		".//Models//triangular_prism2.obj", ".//Models//triangular_pyramid2.obj"};
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
		}else if (num == 3){
			do {
				randomSelectedShape = randomInt(0, 29);
			} while (randomSelectedShape == selectedShape);
			pos = findSpawnPositions();
			spawnPositions[pos].activeObject = &modelShapes[randomSelectedShape];
			initPositionInfo(pos);
		}else if ((num >= 0 && num < 3) || num > 3) {
			randomSelectedShape = randomInt(0, 29);
			pos = findSpawnPositions();
			spawnPositions[pos].activeObject = &modelShapes[randomSelectedShape];
			initPositionInfo(pos);
		}//if-else
	}else if(listSize == 0) {
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
			}else {
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
		}else {
			++iter;
		}
	}//for loop
}//:renderShadows

void LevelManager::reset()
{
	auto iter = activeShapes.begin();
	while (iter != activeShapes.end()) {
		int index = iter->posIndex;
		spawnPositions[index].activeObject = nullptr;

		++iter;
	}
	if(activeShapes.size() != 0)
		activeShapes.clear();
}

void LevelManager::chooseShape()
{
	int num = 0;
	do{
		selectedShape = randomInt(0, 9);
		num = std::count(completedLevels.begin(), completedLevels.end(), selectedShape);
	}while (num != 0);
	completedLevels.push_back(selectedShape);
	std::cout << "seleceted = " << selectedShape << std::endl;
}

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

GLchar* LevelManager::getMissionTarget()
{
	GLchar* name = nullptr;

	switch (selectedShape)
	{
	case 0: name = _strdup("CONES"); break;
	case 1: name = _strdup("CUBES"); break;
	case 2: name = _strdup("CYLINDERS"); break;
	case 3: name = _strdup("ELLIPSOIDS"); break;
	case 4: name = _strdup("HEMISPHERES"); break;
	case 5: name = _strdup("RECTANGULAR PRISMS"); break;
	case 6: name = _strdup("RECTANGULAR PYRAMIDS"); break;
	case 7: name = _strdup("SPHERES"); break;
	case 8: name = _strdup("TRIANGULAR PRISMS"); break;
	case 9: name = _strdup("TRIANGULAR PYRAMIDS"); break;
	}
	return name;
}

void LevelManager::displayShapeText()
{
	if (roundEnding || gameOver)return;
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
	healthDisplayText << "HEALTH ";
	std::string s_health{ healthDisplayText.str() };
	std::string temp{ resultsDisplayText.str() };
	font[0]->renderCharacters(ResourceManager::GetShader("fontShader"), s_health, 5.0f, 530.0f, 0.5f, glm::vec3(1.0f, 0.84f, 0.0f));
	font[0]->renderCharacters(ResourceManager::GetShader("fontShader"), temp, 5.0f, 510.0f, 0.5f, glm::vec3(1.0f, 0.84f, 0.0f));
	glDisable(GL_BLEND);
}//displayShapeText

void LevelManager::resultMessage(GLfloat deltaTime)
{
	float timer = particleTimer.getElapsedTime();
	if (timer >= 1.0f && timer <= 1.5f) {
		screenFlash = false;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		font[1]->renderCharacters(ResourceManager::GetShader("fontShader"), messageRsults[messageIndex], 
			220.0f, 300.0f, 1.0f, glm::vec3(1.0f, 0.84f, 0.0f));
		glDisable(GL_BLEND);
	}
	else if (timer > 1.5f) {
		particleTimer.stopClock();
		Particle::isAnimPlaying = false;
	}
	if(!roundEnding && !gameOver)
	setHealthSlider(true);
}

void LevelManager::setHealthSlider(GLboolean start)
{
	constexpr float HEALTH_BAR_X = 140.0f;
	constexpr float HEALTH_BAR_Y = 714.0f;
	constexpr float HEALTH_BAR_HEIGHT = 20.0f;
	constexpr float HEALTH_BAR_WIDTH = 150.0f;
	float drainRate = 400.0f / HEALTH_BAR_WIDTH;
	float healthTime = barTimer.getElapsedTime();

	if (start) {
		barTimer.startClock();
		start = false;
	}
	barTimer.addTime();
	glEnable(GL_SCISSOR_TEST);
	glScissor(HEALTH_BAR_X, HEALTH_BAR_Y, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	currentHealth -= (drainRate * healthTime);
	if (currentHealth >= 0.0f)
	{
		glScissor(HEALTH_BAR_X, HEALTH_BAR_Y, currentHealth, HEALTH_BAR_HEIGHT);
		glClearColor(0.2f, 0.78f, 0.78f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		barTimer.stopClock();
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}

void LevelManager::start()
{
	if (globalLevelTimer.stopTime)
		globalLevelTimer.startClock();
	float timer = gameClock.getElapsedTime();
	showIntroText = true;
	if(timer == 0)
		gameClock.startClock();
	if (timer <= 5.0f)
	{
		totalShapes = 0;
		glm::vec3 posAndScale(80.0f, 280.0f, 2.0f);
		glm::vec3 titleColor(102.0f, 0.0f, 0.0f);
		std::string title{ "THE COLLECTOR" };
		glClearColor(0.25f, 0.29f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		font[0]->renderCharacters(ResourceManager::GetShader("fontShader"), title,
			posAndScale.x, posAndScale.y, posAndScale.z, titleColor);
		glDisable(GL_BLEND);
	}
	else if (timer >=  6.5f && timer < 8.5f)
	{
		if (!displayMissionText) {
			chooseShape();
			reset();
			displayMissionText = true;
			currentHealth = maximumHealth;
			numCurrentShape = 0;
			blinkingEndCount = 0;
			roundEnding = false;
			gameOver = false;
		}
		showMissionText();
	}
	else if (timer > 10.0f) {
		gameClock.stopClock();
		numCurrentShape = 0;
		currentState = StateMachine::PLAY;
	}
	gameClock.addTime();
}

void LevelManager::showMissionText()
{
	glm::vec3 posAndScale(250.0f, 300.0f, 1.5f), startTextColor(0.94f, 1.0f, 0.29f);
	std::string text = "MISSION";
	glClearColor(0.25f, 0.29f, 0.298f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	font[0]->renderCharacters(ResourceManager::GetShader("fontShader"), text,
		posAndScale.x, posAndScale.y, posAndScale.z, startTextColor);

	GLchar* shape = getMissionTarget();
	glm::vec3 textPosition;
	std::string missionText, wrappedText{};
	displayGoalText(textPosition, shape, missionText, wrappedText);
	missionText = "COLLECT ALL THE " + missionText;
	glm::vec3 missionTextColor(0.0f, 1.0f, 0.0f);
	font[0]->renderCharacters(ResourceManager::GetShader("fontShader"), missionText,
		textPosition.x, textPosition.y, textPosition.z, missionTextColor);
	font[0]->renderCharacters(ResourceManager::GetShader("fontShader"), wrappedText,
		300.0f, 200.0f, textPosition.z, missionTextColor);
	glDisable(GL_BLEND);
}

void LevelManager::displayGoalText(glm::vec3 &position, GLchar *shape, std::string &missionText, std::string &wrappedText)
{
	position.x = 70.0f;
	position.y = 250.0f;
	position.z = 1.0f;
	
	char *scratch = NULL;
	char temp[25];
	strcpy_s(temp, sizeof(temp), shape);
	GLchar *tokenPtr = strtok_s(temp, " ", &scratch);

	if (tokenPtr != NULL) {
		missionText = tokenPtr;
		if (strcmp("CONES", tokenPtr) == 0) {
			missionText = "CONES";
			position.x = 130.0f;
			return;
		}
		else if (strcmp("CUBES", tokenPtr) == 0) {
			missionText = "CUBES";
			position.x = 130.0f;
			return;
		}
		else if (strcmp("SPHERES", tokenPtr) == 0) {
			missionText = "SPHERES";
			position.x = 100.0f;
			return;
		}
	}
	tokenPtr = strtok_s(NULL, " ", &scratch);
	if (tokenPtr != NULL) {
		if (strstr(tokenPtr, "PRISMS")) wrappedText = "PRISMS";
		else if (strstr(tokenPtr, "PYRAMIDS")) wrappedText = "PYRAMIDS";
	}
}

Collision LevelManager::testCollision(glm::vec3 cameraPosition)
{
	constexpr float PLAYER_RADIUS = 1.0f;
	constexpr float OBJ_RADIUS = 5.0f;
	
	for (auto iter = activeShapes.begin(); iter != activeShapes.end(); ++iter) {
		PositionInfo point = spawnPositions[iter->posIndex];
		glm::vec3 shapeCenterPos = point.worldPosition;
		glm::vec3 s = shapeCenterPos - cameraPosition;
		float distanceSquared = glm::dot(s, s);
		float sumOfSquares = (PLAYER_RADIUS* PLAYER_RADIUS) + (OBJ_RADIUS * OBJ_RADIUS);
		float d = sumOfSquares * sumOfSquares;
		if (distanceSquared <= d)
		{
			iter->counter = -1;
			Particle::isAnimPlaying = true;
			return std::make_tuple(true, shapeCenterPos, point.activeObject->shapeType);
		}
	}
	return std::make_tuple(false, glm::vec3(0.0f), Shapes3D::CONE);
}



