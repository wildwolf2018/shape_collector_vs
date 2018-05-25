#include "shadow.h"
#include "resource_manager.h"

Shadow::Shadow()
{
	ResourceManager::LoadShader("model_vshader.txt", "model_fshader.txt", "model");
	ResourceManager::LoadShader("shadow_vertex_shader.txt", "shadow_fragment_shader.txt", "shadow_map");

	float quadVerticexData[] = {
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f
	};

	glGenBuffers(1, &shadowVBO);
	glBindBuffer(GL_ARRAY_BUFFER, shadowVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerticexData), quadVerticexData, GL_STATIC_DRAW);
	//create vertex array object
	glGenVertexArrays(1, &shadowVAO);
	glBindVertexArray(shadowVAO);
	//map index 0 to vertex postion in buffer
	glBindBuffer(GL_ARRAY_BUFFER, shadowVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//enable vertex attribute arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void Shadow::createDepthMapFrameBuffer()
{
	//Create framebuffer
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	//Creates 2D texture to be used as framebuffer's dpeth buffer
	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void Shadow::renderToFrameBuffer(glm::mat4& lightSpaceMatrix, std::shared_ptr<Shader> depthMapShader)
{
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	depthMapShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader->ProgramID, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
	const GLuint SHADOW_WIDTH = 1024;
	const GLuint SHADOW_HEIGHT = 1024;
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//glUniformMatrix4fv(glGetUniformLocation(depthMapShader->ProgramID, "model"), 1, GL_FALSE, glm::value_ptr(modelFloorMatrix));
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glClear(GL_DEPTH_BUFFER_BIT);
}