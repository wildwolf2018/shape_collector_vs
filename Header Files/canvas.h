#ifndef CANVAS_H
#define CANVAS_H

#include <GL\glew.h>
#include <memory>
#include <tuple>
#include <initializer_list>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <vector>
#include "..\\Header Files\\wall.h"
#include "..\\Header Files\\font.h"
#include "..\\Header Files\\skybox.h"
#include "..\\Header Files\\resource_manager.h"

class Canvas
{
public:
	Canvas();
	void initialise();
	void createGameWalls();
	void setWallModelMatices();
	void setFloorCeilingModelMatrices();
	void setWallLocations();
	void findWallExtents();
	void render(glm::vec3 &cameraPos, glm::mat4 &projection, glm::mat4& viewMatrix);
	void renderSkybox(glm::mat4& projectionMatrix, glm::mat4& viewMatrix);
	std::vector<glm::mat4> matrices;
	std::vector<std::shared_ptr<Wall>>walls;
	void setLightProperties(GLuint programID, glm::vec3 position, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor);
	void setMaterialUniforms(GLuint programID, GLuint textureUnit, glm::vec3& specularColor, float specularPower);
private:
	Skybox cubeMap;
	GLuint skyboxTextureID;
	enum class Transforms { translate, rotate, scale };
	void setModelMatrix(glm::mat4&, std::initializer_list<std::tuple<Transforms, glm::vec3>>, float = 90.0f);
	/*void setLightProperties(GLuint programID, glm::vec3 position, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor);
	void setMaterialUniforms(GLuint programID, GLuint textureUnit, glm::vec3& specularColor, float specularPower);*/
};
#endif

