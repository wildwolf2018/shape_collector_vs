#include <chrono>
#include <random>
#include <iostream>
#include "particle.h"
#include "resource_manager.h"

Particle::Particle(const GLchar* vShader, const  GLchar* fShader, const  GLchar* textureFile, GLint numParticles) :
	time{0}, rate{ 0.00075f}, isAnimPlaying{false}, particleSpeed{0.0f}
{
	ResourceManager::LoadShader(vShader, fShader, "particleShader");
	ResourceManager::LoadTexture(textureFile, false, "particle");
	this -> numParticles = numParticles;
	const  GLint NUM_OF_COMPONENTS = 8;
	const  GLint PARTICLE_SIZE = numParticles * NUM_OF_COMPONENTS;
	data = new GLfloat[PARTICLE_SIZE];// array to store particles' data 

	glGenVertexArrays(1, &particleVAO);
	glGenBuffers(1, &particleVBO);
	glBindVertexArray(particleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_SIZE * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)(numParticles * 3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)(numParticles * 6 * sizeof(GLfloat)));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (GLvoid*)(numParticles * 7 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	initParticles(PARTICLE_SIZE);
}

GLvoid Particle::initParticles(const  GLint particleDataSize)
{
	std::default_random_engine e; // random number engine
	GLuint seedValue = 0; // random number engine seed
	GLuint timeOffset = 0, vertexOffset = 0, velocityOffset = numParticles * 3;
	GLuint startTimeOffset = numParticles * 6, lifeTimeOffset = numParticles * 7;
	for (GLuint i = 0; i < numParticles; i++) {
		//Store position coordinates of particle
		data[vertexOffset++] = 0.0f;
		data[vertexOffset++] = 0.0f;
		data[vertexOffset++] = 0.0f;

		//Calculate random velocity of particle
		seedValue = std::chrono::steady_clock::now().time_since_epoch().count() + timeOffset;
		e.seed(seedValue);
		timeOffset += 10;
		std::uniform_real_distribution<double> distR1(0, 2 * PI);
		theta = static_cast<float> (distR1(e));
		seedValue = std::chrono::steady_clock::now().time_since_epoch().count() + timeOffset;
		e.seed(seedValue);
		std::uniform_real_distribution<double> distR2(0, PI);
		phi = static_cast<float> (distR2(e));
		data[velocityOffset++] = std::sin(phi) * std::cos(theta);// velocity x
		data[velocityOffset++] = std::cos(phi);// velocity y
		data[velocityOffset++] = std::sin(phi) * std::sin(theta);// velocity z

		//Calculate startTime of particle
		data[startTimeOffset++] = time;
		time += rate;
		//Store lifeTime of particle
		data[lifeTimeOffset++] = 2.0f;
		timeOffset += 10;
	}
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleDataSize * sizeof(GLfloat), data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLvoid Particle::setUniforms(glm::mat4& projection, glm::mat4& view, std::shared_ptr<Shader> particleShaderObj, GLfloat timer, GLfloat deltaTime)
{
	glUniformMatrix4fv(glGetUniformLocation(particleShaderObj->ProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1f(glGetUniformLocation(particleShaderObj->ProgramID, "time"), timer);
	particleSpeed += 5.0f * deltaTime;
	glUniform1f(glGetUniformLocation(particleShaderObj->ProgramID, "speed"), particleSpeed);
	Texture2D tex = ResourceManager::GetTexture("particle");
	tex.Bind();
	glUniform1i(glGetUniformLocation(particleShaderObj->ProgramID, "sprite"), 0);
	if (animStart) {
		glUniform1f(glGetUniformLocation(particleShaderObj->ProgramID, "animationStartTime"), timer);
		animStart = false;
	}
	glUniformMatrix4fv(glGetUniformLocation(particleShaderObj->ProgramID, "viewMatrix"), 1, GL_FALSE, glm::value_ptr(view));
}

GLvoid Particle::drawParticle()
{
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Draw particle explosion
	glBindVertexArray(particleVAO);
	glDrawArrays(GL_POINTS, 0, numParticles);
	glBindVertexArray(0);
	glDisable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}