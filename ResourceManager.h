#pragma once
#include <fstream>
#include <string>
#include <map>
#include <assimp/mesh.h>
#include <vector>

using namespace std;

#define Resource ResourceManager::getInstance()

class Mesh;
class ResourceManager
{
public:
	//Start the Resource Manager make folders and files nessesary for running the game if not made already
	int Initialize();

	//close files and stop the Resource Manager
	void Terminate();

	//Singleton get function
	static ResourceManager& getInstance();

	//grab a scene from the assets folder and load it into the World Manager
	int loadScene(string name);

	unsigned int loadImage(const char* path);

	unsigned int loadCubeMap(vector<const char*> paths);

	vector<Mesh*> getMeshFromMap(string path);
	void addMeshToMap(string path, vector<Mesh*> mesh);

	unsigned int getShaderProgramFromMap(string path);
	void addShaderProgramToMap(string path, unsigned int shaderProgram);

private:
	ResourceManager();

	map<string, vector<Mesh*>> meshMap;
	map<string, unsigned int> textureMap;
	map<string, unsigned int> shaderMap;
};