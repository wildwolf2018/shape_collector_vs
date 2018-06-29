#ifndef SKYBOX_H
#define SKYBOX_H
#include <vector>
#include <GL/glew.h> 

class Skybox
{
public:
		Skybox();
		//Loads the cube faces images
		GLuint loadCubeMap(std::vector<const GLchar*> faces);
		GLuint skyboxVBO;
		GLuint skyboxVAO;
};
#endif