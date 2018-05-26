#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <random>
#include <string>
#include <map>
#include "resource_manager.h"
#include "model.h"
#include "wall.h"
#include "shadow.h"
#include "font.h"
#include "particle.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include <irrKlang.h>

using namespace irrklang;

//#include <assimp/Importer.hpp> 
//#include <assimp/scene.h> 
//#include <assimp/postprocess.h>

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_cursor_reset(GLFWwindow* window, int entered);
void do_movement();
GLboolean testCollision(glm::vec3 cameraPosition, glm::vec3 shapeCenterPos);

//Viewport size
const int WIDTH = 1200;
const int HEIGHT = 700;

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 30.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
/* Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right
(due to how Eular angles work) so we initially rotate a bit to the left.**/
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame
std::chrono::time_point<std::chrono::steady_clock> start_ticks, end_ticks;
GLfloat timer = 1000.0f;
GLboolean showObj = true;

int main()
{
	glfwInit();
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Model Loading", NULL, NULL);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetCursorEnterCallback(window, mouse_cursor_reset);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);
	//Set Viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	Wall floor{ glm::vec3(0.0f, 0.0f, 1.0f) };
	Shadow shadowObj{};
	shadowObj.createDepthMapFrameBuffer();

	//Create projection matrix and pass it to the shader
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 500.0f);
	glm::mat4 model, view;
	glm::vec3 shapeCenterPos = glm::vec3(0.0f, 0.0f, 0.0f);
	model = glm::translate(model, shapeCenterPos);
	shapeCenterPos = glm::vec3(model * glm::vec4(shapeCenterPos, 1.0f));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	//shapeCenterPos = glm::vec3(model * glm::vec4(shapeCenterPos, 1.0f));
	//model = glm::rotate(model, 100.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

	//Place floor in the world
	glm::mat4 modelFloorMatrix;
	modelFloorMatrix = glm::translate(modelFloorMatrix, glm::vec3(0.0, -15.0f, 0.0f));
	modelFloorMatrix = glm::scale(modelFloorMatrix, glm::vec3(15.0, 3.0, 60.0));
	modelFloorMatrix = glm::rotate(modelFloorMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	GLfloat near_plane = 1.0f, far_plane = 500.0f;
	glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(-10.0f, 10.0f, 10.0f), glm::vec3(0.0f), glm::vec3(1.0));//vec3(14.64f, 20.0f, 10.0f),
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	Model _model("C:\\Users\\Teboho\\Documents\\Visual Studio 2015\\Projects\\SOILTest\\OpenGlLTest\\cube2.obj");
	std::shared_ptr<Shader> shaderObject = ResourceManager::GetShader("model");
	std::shared_ptr<Shader> depthMapShader = ResourceManager::GetShader("shadow_map");
	
	//Particles
	int numberOfParticles = 100;
	Particle explosion{ "vshader.txt", "fshader.txt", "red_particle.png", numberOfParticles};

	Texture2D particleTexture = ResourceManager::GetTexture("particle");
	std::shared_ptr<Shader> particleShaderObj = ResourceManager::GetShader("particleShader");

	//Font
	Font messageText("STENCIL.ttf");
	ISoundEngine *SoundEngine = createIrrKlangDevice();
	//SoundEngine -> play2D("intro.mp3", GL_TRUE);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		do_movement();
		if (explosion.isAnimPlaying == false && testCollision(cameraPos, shapeCenterPos)) {
			timer = 0.0f;
			explosion.particleSpeed = 0.0f;
			explosion.animStart = true;
			//Start the game clcok
			start_ticks = std::chrono::steady_clock::now();
			showObj = false;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Render to the shadow map framebuffer
		shadowObj.renderToFrameBuffer(lightSpaceMatrix, depthMapShader);
		glUniformMatrix4fv(glGetUniformLocation(depthMapShader->ProgramID, "model"), 1, GL_FALSE, glm::value_ptr(modelFloorMatrix));
		floor.draw();
		model = glm::rotate(model, 1.0f * deltaTime, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(depthMapShader->ProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		if(showObj)
		_model.Draw();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Calculate deltaTime
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	
		//Clear color and depth buffers
		glViewport(0, 0, WIDTH, HEIGHT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		////Draw the objects in the scene as normal
		glDisable(GL_CULL_FACE);
		shaderObject->Use();
		shaderObject->setMatrix(projection, "projection");
		shaderObject->setMatrix(model, "model");
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		shaderObject->setMatrix(view, "view");
		shaderObject->setMatrix(lightSpaceMatrix, "lightSpaceMatrix");
		glBindTexture(GL_TEXTURE_2D, shadowObj.depthMap);
		glUniform1i(glGetUniformLocation(shaderObject->ProgramID, "shadowMap"), 0);
		if (showObj) {
			_model.setLightUniforms(shaderObject, cameraPos);
			_model.Draw();
		}
		shaderObject->setMatrix(modelFloorMatrix, "model");
		floor.setUniforms(shaderObject, cameraPos);
		floor.draw();

		// Send uniforms data to shader
		if (timer <= 2.0f) {
			particleShaderObj->Use();
			explosion.setUniforms(projection, view, particleShaderObj, timer, deltaTime);
			explosion.drawParticle();
			std::cout << "total time elapsed = " << timer << std::endl;
			explosion.isAnimPlaying = true;
		}

		// Calculate deltaTime for next frame
		end_ticks = std::chrono::steady_clock::now();
		std::chrono::duration<double> delta = end_ticks - start_ticks;
		timer += delta.count();

		//Display collision message
		if (timer >= 1.5f && timer <= 2.0f) {
			glEnable(GL_BLEND); 
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			messageText.renderCharacters(ResourceManager::GetShader("fontShader"), "Excellent!!!", 250.0f, 300.0f, 1.0f, glm::vec3(1.0f, 0.84f, 0.0f));
			glDisable(GL_BLEND);
			//message.displayOnScreen(correctResultMessages[3], glm::vec3(500.0f, 350.0f, 0.5f), glm::vec3(1.0f, 0.84f, 0.0f), view);
		}

		// Use end_ticks as the new begin_ticks for next frame
		start_ticks = end_ticks;
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

void do_movement()
{
	// Camera controls
	GLfloat cameraSpeed = 10.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keys[GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	cameraFront.y = 0.0f;
	//std::cout << "x = " << cameraPos.x << "  y = " << cameraPos.y << "  z = " << cameraPos.z << std::endl;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}


GLboolean firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = static_cast<GLfloat>(xpos);
		lastY = static_cast<GLfloat>(ypos);
		firstMouse = false;
	}

	GLfloat xoffset = static_cast<GLfloat>(xpos - lastX);
	GLfloat yoffset = static_cast<GLfloat>(lastY - ypos); // Reversed since y-coordinates go from bottom to left
	lastX = static_cast<GLfloat>(xpos);
	lastY = static_cast<GLfloat>(ypos);

	GLfloat sensitivity = 0.1f;	// Change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void mouse_cursor_reset(GLFWwindow* window, int entered)
{
	if (entered)
	{
		// The cursor entered the client area of the window
		firstMouse = true;
	}
}

GLboolean testCollision(glm::vec3 cameraPosition, glm::vec3 shapeCenterPos)
{
	constexpr float PLAYER_RADIUS = 0.1f;
	constexpr float OBJ_RADIUS = 3.5f;

	glm::vec3 s = shapeCenterPos - cameraPos;
	float distanceSquared = glm::dot(s, s);
	float sumOfSquares = (PLAYER_RADIUS* PLAYER_RADIUS) + (OBJ_RADIUS * OBJ_RADIUS);
	float d = sumOfSquares * sumOfSquares;
	if (distanceSquared <= d)
	{
		std::cout << "Collision Detected\n";
		return true;
	}
	return false;
	
}


