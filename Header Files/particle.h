#ifndef PARTICLE_H
#define PARTICLE_H
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "..\\Header Files\\shader.h"


class Particle {
public:
	Particle(const  GLchar* vShader, const GLchar* fShader, const GLchar* textureFile, GLint numParticles);
	GLvoid setUniforms(glm::mat4& projection, glm::mat4& view, std::shared_ptr<Shader> particleShaderObj, GLfloat timer, GLfloat deltaTime);
	GLvoid drawParticle();
	GLvoid initParticles(glm::vec3& spwanPoint);
	GLboolean animStart = false;
	static GLboolean isAnimPlaying;
	GLfloat particleSpeed;
	GLint particleDataSize;
private:
	GLint numParticles;
	const  GLfloat PI = 3.141592654f; // constant value pie
	GLuint particleVAO, particleVBO;
	GLfloat *data;
	GLfloat time; // animation startTime
	GLfloat rate ; // rate at which particles are spawned
	GLdouble theta, phi; //spherical coordinates of particle
};
#endif
