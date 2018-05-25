#ifndef SHADOW_H
#define SHADOW_H

#include <GL/glew.h> 
#include <glm/glm.hpp>
#include "shader.h"

class Shadow
{
public:
	GLuint depthMap;
	Shadow();
	void createDepthMapFrameBuffer();
	void renderToFrameBuffer(glm::mat4& lightSpaceMatrix, std::shared_ptr<Shader> depthMapShader);
private:
	GLuint shadowVBO, shadowVAO, depthMapFBO;
};
#endif
