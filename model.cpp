#include "model.h"
#include <iostream>

Model::Model(GLchar* path, Shapes3D shapeType)
{
	this->loadModel(path);
	this->shapeType = shapeType;
}

void Model::Draw()
{ 
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw();
}

void Model::loadModel(string path) 
{
	Assimp::Importer import; const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
	{ 
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl; 
		return; 
	} 
	this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) 
{ 
	// Process all the node’s meshes (if any) 
	for(GLuint i = 0; i < node->mNumMeshes; i++) 
	{ 
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
		this->meshes.push_back(this->processMesh(mesh, scene)); } 
	// Then do the same for each of its children 
	for(GLuint i = 0; i < node->mNumChildren; i++) {
		this->processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) 
{
	vector<Vertex> vertices; 
	vector<GLuint> indices;
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector; 
		vector.x = mesh->mVertices[i].x; 
		vector.y = mesh->mVertices[i].y; 
		vector.z = mesh->mVertices[i].z; 
		vertex.position = vector;

		vector.x = mesh->mNormals[i].x; 
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z; 
		vertex.normal = vector;

		vertices.push_back(vertex); 
	}
	for (GLuint i = 0; i < mesh->mNumFaces; i++) 
	{ 
		aiFace face = mesh->mFaces[i]; 
		for (GLuint j = 0; j < face.mNumIndices; j++) 
			indices.push_back(face.mIndices[j]); 
	}
	return Mesh(vertices, indices);
}

void Model::setLightUniforms(std::shared_ptr<Shader>& shaderObj, glm::vec3& cameraPos)
{
	GLuint programID = shaderObj->ProgramID;
	//Light propeties' uniform locations
	GLuint lighPositionLoc = glGetUniformLocation(programID, "light.position");
	GLuint lighAmbLoc = glGetUniformLocation(programID, "light.ambient");
	GLuint lighDiffuseLoc = glGetUniformLocation(programID, "light.diffuse");
	GLuint lighSpecLoc = glGetUniformLocation(programID, "light.specular");
	GLuint viewPositionLoc = glGetUniformLocation(programID, "viewPos");
	glUniform3f(viewPositionLoc, cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(lighPositionLoc, 10.0f, -10.0f, -10.0f);
	glUniform3f(lighAmbLoc, 0.8f, 0.8f, 0.8f);
	glUniform3f(lighDiffuseLoc, 0.8f, 0.8f, 0.8f);
	glUniform3f(lighSpecLoc, 0.5f, 0.5, 0.5f);
	//Set Material properties uniforms
	GLuint materialAmbLoc = glGetUniformLocation(programID, "material.ambient");
	GLuint materialDiffuseLoc = glGetUniformLocation(programID, "material.diffuse");
	GLuint materialSpecLoc = glGetUniformLocation(programID, "material.specular");
	GLuint specExpoLoc = glGetUniformLocation(programID, "material.shininess");
	glUniform3f(materialAmbLoc, 0.4f, 0.0f, 0.0f);
	glUniform3f(materialDiffuseLoc, 0.4f, 0.0f, 0.0f);
	glUniform3f(materialSpecLoc, 0.4f, 0.0f, 0.0f);
	glUniform1f(specExpoLoc, 32.0f);// 0.4f, 0.0f, 0.0f);
}



