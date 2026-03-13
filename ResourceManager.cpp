#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"

#include "ResourceManager.h"
#include "Mesh.h"
#include <iostream>
#include "Scene.h"
#include "glad/glad.h"

int ResourceManager::Initialize()
{
	return 0;
}

void ResourceManager::Terminate()
{
	return;
}

ResourceManager& ResourceManager::getInstance()
{
	static ResourceManager single;
	return single;
}

int ResourceManager::loadScene(string name)
{
	return 0;
}

unsigned int ResourceManager::loadImage(const char* path)
{
	// If Texture already loaded grab loaded texture
	auto mapTexture = textureMap.find(string(path));
	if (mapTexture != textureMap.end())
	{
		return mapTexture->second;
	}

	//Loading Texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on currently bound texture)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height,
		&nrChannels, 0);
	if (data)
	{
		switch (nrChannels) {
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R, width, height, 0, GL_R,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		case 2:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		default:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			break;
		}
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	textureMap.insert({string(path), texture});

	return texture;
}

unsigned int ResourceManager::loadCubeMap(vector<const char*> paths)
{
	if (paths.size() != 6)
	{
		cout << "Number of cube map images is incorrect imput 6" << endl;
		return -1;
	}

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	unsigned char* data;
	for (unsigned int i = 0; i < paths.size(); i++)
	{
		data = stbi_load(paths[i], &width, &height, &nrChannels, 0);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
		);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	return textureID;
}

vector<Mesh*> ResourceManager::getMeshFromMap(string path)
{
	// If Texture already loaded grab loaded texture
	auto mapMesh = meshMap.find(string(path));
	if (mapMesh != meshMap.end())
	{
		return mapMesh->second;
	}
	return {};
}

void ResourceManager::addMeshToMap(string path, vector<Mesh*> mesh)
{
	meshMap.insert({ path, mesh });
}

unsigned int ResourceManager::getShaderProgramFromMap(string path)
{
	// If Texture already loaded grab loaded texture
	auto mapShader = shaderMap.find(string(path));
	if (mapShader != shaderMap.end())
	{
		return mapShader->second;
	}

	return -1;
}

void ResourceManager::addShaderProgramToMap(string path, unsigned int shaderProgram)
{
	shaderMap.insert({ path, shaderProgram });
}

ResourceManager::ResourceManager()
{
}
