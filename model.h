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
#include "mesh.h"
#include "shader.h"

using std::vector;
using std::string;

class Model { 
public: 
	Model(GLchar* path) { this->loadModel(path); } 
	void Draw();
	void setLightUniforms(std::shared_ptr<Shader>& shaderObj, glm::vec3& cameraPos);
private:
	vector<Mesh> meshes; 
	string directory; 
	void loadModel(string path); 
	void processNode(aiNode* node, const aiScene* scene); 
	Mesh processMesh(aiMesh* mesh, const aiScene* scene); 
};
#endif
