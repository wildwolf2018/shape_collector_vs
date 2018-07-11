#ifndef MODEL_H
#define MODEL_H
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp> 
#include <assimp/scene.h> 
#include <assimp/postprocess.h>
#include <vector>
#include <string>
#include "..\\Header Files\\mesh.h"
#include "..\\Header Files\\shader.h"

using std::vector;
using std::string;

enum class Shapes3D { CONE, CUBE, CYLINDER, ELLIPSOID, HEMISPHERE, R_PRISM, R_PYRAMID, SPHERE, T_PRISM, T_PYRAMID };

class Model { 
public: 
	Model() = default;
	Model(GLchar* path, Shapes3D shapeType);
	Shapes3D shapeType;
	void loadModel(string path);
	void Draw();
	void setLightUniforms(std::shared_ptr<Shader>& shaderObj, glm::vec3& cameraPos);
private:
	vector<Mesh> meshes; 
	string directory;  
	void processNode(aiNode* node, const aiScene* scene); 
	Mesh processMesh(aiMesh* mesh, const aiScene* scene); 
};
#endif
