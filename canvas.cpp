#include "canvas.h"

Canvas::Canvas() :cubeMap{}
{
	ResourceManager::LoadShader("vertex_shader.txt", "frag_shader.txt", "wallShader");
	ResourceManager::LoadShader("vert_skybox.txt", "frag_skybox.txt", "skyboxShader");
	ResourceManager::LoadShader("vertexFloorShader.txt", "fragFloorShader.txt", "floorShader");
	ResourceManager::LoadTexture("bricks_diff.jpg", false, "frontWall");
	ResourceManager::LoadTexture("marble_ceiling.jpg", false, "floor");
	ResourceManager::LoadTexture("brick.jpg", false, "sideWalls");
	initialise();
	vector<const GLchar*> skyboxFaceImages{".//Textures//bluecloud_rt.jpg", ".//Textures//bluecloud_lf.jpg", ".//Textures//graycloud_up.jpg",
		".//Textures//bluecloud_dn.jpg",".//Textures//bluecloud_bk.jpg",".//Textures//bluecloud_ft.jpg"};
	skyboxTextureID = cubeMap.loadCubeMap(skyboxFaceImages);
}

void Canvas::initialise()
{
	createGameWalls();
	setWallModelMatices();
	setFloorCeilingModelMatrices();
	setWallLocations();
	findWallExtents();
}

void Canvas::createGameWalls()
{
	//Create the walls,floor and celing
	walls.push_back(std::shared_ptr<Wall> {new Wall(glm::vec3(0.0f, 0.0f, 1.0f), 1)}); //front wall
	walls.push_back(std::shared_ptr<Wall> {new Wall(glm::vec3(1.0f, 0.0f, 0.0f), 1)}); //left wall
	walls.push_back(std::shared_ptr<Wall> {new Wall(glm::vec3(-1.0f, 0.0f, 0.0f), 1)}); //right wall
	walls.push_back(std::shared_ptr<Wall> {new Wall(glm::vec3(0.0f, 0.0f, -1.0f), 1)}); //back wall
	walls.push_back(std::shared_ptr<Wall> {new Wall(glm::vec3(0.0f, 1.0f, 0.0f), 2)}); //floor
	//walls.push_back(std::shared_ptr<Wall> {new Wall(glm::vec3(0.0f, -1.0f, 0.0f), 3)}); //ceiling
}

void Canvas::setFloorCeilingModelMatrices()
{
	// Place floor in the world
	glm::mat4 model5;
	auto translateFloor = std::make_tuple(Transforms::translate, glm::vec3(0.0, -15.0, 0.0));
	auto rotateFloor = std::make_tuple(Transforms::rotate, glm::vec3(1.0, 0.0, 0.0));
	auto scaleFloor = std::make_tuple(Transforms::scale, glm::vec3(15.0, 3.0, 60.0));
	setModelMatrix(model5, { translateFloor, scaleFloor, rotateFloor });
	matrices.push_back(model5);

	//Place ceiling in the world
	// glm::mat4 model6;
	// auto translateCeiling = std::make_tuple(Transforms::translate, glm::vec3(0.0, 15.0, 0.0));
	// auto rotateCeiling =  std::make_tuple(Transforms::rotate, glm::vec3(1.0, 0.0, 0.0));
	// auto scaleCeiling = std::make_tuple(Transforms::scale, glm::vec3(600.0, 1200.0, 2400.0));
	// setModelMatrix(model6, {translateCeiling, scaleCeiling, rotateCeiling});
	// matrices.push_back(model6);
}

void Canvas::setWallModelMatices()
{
	//Place front wall
	glm::mat4 model1;
	auto translateFrontWall = std::make_tuple(Transforms::translate, glm::vec3(0.0, 0.0, -150.0));
	auto scaleFrontWall = std::make_tuple(Transforms::scale, glm::vec3(15.0, 6.0, 3.0));
	setModelMatrix(model1, { translateFrontWall, scaleFrontWall });
	matrices.push_back(model1);
	//Place left wall
	glm::mat4 model2;
	auto translateLeftWall = std::make_tuple(Transforms::translate, glm::vec3(-150.0, 0.0, 0.0));
	auto rotateLeftWall = std::make_tuple(Transforms::rotate, glm::vec3(0.0, 1.0, 0.0));
	auto scaleLeftWall = std::make_tuple(Transforms::scale, glm::vec3(15.0, 6.0, 3.0));
	setModelMatrix(model2, { translateLeftWall, rotateLeftWall, scaleLeftWall });
	matrices.push_back(model2);
	//Place right wall
	glm::mat4 model3;
	auto translateRightWall = std::make_tuple(Transforms::translate, glm::vec3(150.0, 0.0, 0.0));
	auto rotateRightWall = std::make_tuple(Transforms::rotate, glm::vec3(0.0, 1.0, 0.0));
	auto scaleRightWall = std::make_tuple(Transforms::scale, glm::vec3(15.0, 6.0, 3.0));
	setModelMatrix(model3, { translateRightWall, rotateRightWall, scaleRightWall });
	matrices.push_back(model3);
	//Place back wall
	glm::mat4 model4;
	auto translateBackWall = std::make_tuple(Transforms::translate, glm::vec3(0.0, 0.0, 150.0));
	auto scaleBackWall = std::make_tuple(Transforms::scale, glm::vec3(15.0, 6.0, 3.0));
	setModelMatrix(model4, { translateBackWall, scaleBackWall });
	matrices.push_back(model4);
}

void Canvas::setWallLocations()
{
	walls[0]->setWorldPosition(matrices[0]);
	walls[1]->setWorldPosition(matrices[1]);
	walls[2]->setWorldPosition(matrices[2]);
	walls[3]->setWorldPosition(matrices[3]);
	walls[4]->setWorldPosition(matrices[4]);
	//walls[5] -> setWorldPosition(matrices[5]);
}

void Canvas::findWallExtents()
{
	walls[0]->findWallExtents();
	walls[1]->findWallExtents();
	walls[2]->findWallExtents();
	walls[3]->findWallExtents();
}

void Canvas::render(glm::vec3 & cameraPos, glm::mat4 &projection, glm::mat4 &viewMatrix)
{
	// Front wall
	ResourceManager::GetShader("wallShader")->Use();
	GLuint programID = ResourceManager::GetShader("wallShader")->ProgramID;
	ResourceManager::GetShader("wallShader")->setMatrix(matrices[0], "model");
	ResourceManager::GetShader("wallShader")->setMatrix(viewMatrix, "view");
	ResourceManager::GetShader("wallShader")->setMatrix(projection, "projection");
	glActiveTexture(GL_TEXTURE0);
	ResourceManager::GetTexture("frontWall").Bind();
	glm::vec3 tempVec = glm::vec3(0.1f, 0.1f, 0.1f);
	setMaterialUniforms(programID, 0, tempVec, 32.0f);
	setLightProperties(programID, glm::vec3(80.0f, 500.0f, 20.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.9f, 0.9f, 0.9f),
		glm::vec3(0.2f, 0.2f, 0.2f));
	glUniform3f(glGetUniformLocation(programID, "viewPos"), cameraPos.x, cameraPos.y, cameraPos.z);
	walls[0]->draw();
	//Back wall
	ResourceManager::GetShader("wallShader")->setMatrix(matrices[3], "model");
	ResourceManager::GetTexture("frontWall").Bind();
	setMaterialUniforms(programID, 0, tempVec, 32.0f);
	walls[3]->draw();
	//Left wall
	ResourceManager::GetShader("wallShader")->setMatrix(matrices[1], "model");
	glActiveTexture(GL_TEXTURE2);
	ResourceManager::GetTexture("sideWalls").Bind();
	setMaterialUniforms(programID, 2, tempVec, 32.0f);
	walls[1]->draw();
	//Right wall
	ResourceManager::GetShader("wallShader")->setMatrix(matrices[2], "model");
	setMaterialUniforms(programID, 2, tempVec, 32.0f);
	walls[2]->draw();
	//Floor
	/*ResourceManager::GetShader("wallShader")->setMatrix(matrices[4], "model");
	glActiveTexture(GL_TEXTURE1);
	ResourceManager::GetTexture("floor").Bind();
	setMaterialUniforms(programID, 1, tempVec, 32.0f);
	walls[4]->draw();*/
}

void Canvas::renderSkybox(glm::mat4 & projectionMatrix, glm::mat4 & viewMatrix)
{
	GLint oldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
	glDepthFunc(GL_LEQUAL);
	ResourceManager::GetShader("skyboxShader")->Use();
	GLuint skyboxProgramID = ResourceManager::GetShader("skyboxShader")->ProgramID;
	ResourceManager::GetShader("skyboxShader")->setMatrix(projectionMatrix, "projection");
	glm::mat4 view = glm::mat4(glm::mat3(viewMatrix));
	ResourceManager::GetShader("skyboxShader")->setMatrix(view, "view");
	glBindVertexArray(cubeMap.skyboxVAO);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glUniform1i(glGetUniformLocation(skyboxProgramID, "skybox"), 3);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(oldDepthFuncMode);
}

void Canvas::setModelMatrix(glm::mat4 &model, std::initializer_list<std::tuple<Transforms, glm::vec3>>list, float angleInRadians)
{
	Transforms type;
	for (auto i = list.begin(); i != list.end(); i++)
	{
		auto _tuple = *i;
		type = std::get<0>(_tuple);// 0 represents Transforms enumeration
		switch (type)
		{
		case Transforms::translate:
			model = glm::translate(model, std::get<1>(_tuple));//1 represents translate vector element in tuple
			break;
		case Transforms::rotate:
			model = glm::rotate(model, glm::radians(angleInRadians), std::get<1>(_tuple));//1 represents rotation vector element in tuple
			break;
		case Transforms::scale:
			model = glm::scale(model, std::get<1>(_tuple));//1 represents scale vector element in tuple
			break;
		}
	}
}

void Canvas::setLightProperties(GLuint programID, glm::vec3 position, glm::vec3 ambientColor, glm::vec3 diffuseColor, glm::vec3 specularColor)
{
	glUniform3f(glGetUniformLocation(programID, "light.position"), position.x, position.y, position.z);//position	
	glUniform3f(glGetUniformLocation(programID, "light.ambient"), ambientColor.x, ambientColor.y, ambientColor.z);//ambinet color
	glUniform3f(glGetUniformLocation(programID, "light.diffuse"), diffuseColor.x, diffuseColor.y, diffuseColor.z);//diffuse color
	glUniform3f(glGetUniformLocation(programID, "light.specular"), specularColor.x, specularColor.y, specularColor.z);//specular colo
}

void Canvas::setMaterialUniforms(GLuint programID, GLuint textureUnit, glm::vec3 & specularColor, float specularPower)
{
	glUniform1i(glGetUniformLocation(programID, "material.diffuseMap"), textureUnit);
	glUniform3f(glGetUniformLocation(programID, "material.specular"), specularColor.x, specularColor.y, specularColor.z);
	glUniform1f(glGetUniformLocation(programID, "material.shininess"), specularPower);
}

