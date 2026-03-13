#pragma once
#include "Component.h"
#include "Mesh.h"
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

class Material;
class MeshRenderer : public Component
{
public:
	MeshRenderer(Object* _parent);

	// called every frame
	void update() override;

	void Draw();

	void loadCubeMap(vector<const char*> paths);

	void loadModel(string path);

	Material* material;

private:
	vector<Mesh*> meshes;
	string directory;

	void processNode(aiNode* node, const aiScene* scene, string path);
	Mesh* processMesh(aiMesh* mesh, const aiScene* scene);
};

