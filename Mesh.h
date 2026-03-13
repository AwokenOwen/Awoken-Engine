#pragma once
#include <vector>
#include <string>
#include "glm.hpp"

using namespace glm;
using namespace std;

class Material;
class Object;
class Mesh
{
public:
	struct Vertex
	{
		vec3 position;
		vec3 normal;
		vec2 texCoords;

		Vertex() 
		{
			position = vec3(0.0f);
			normal = vec3(0.0f);
			texCoords = vec2(0.0f);
		}

		Vertex(vec3 position) 
		{
			this->position = position;
			normal = vec3(0.0f);
			texCoords = vec2(0.0f);
		}

		Vertex(vec3 position, vec3 normal, vec2 texCoords) {
			this->position = position;
			this->normal = normal;
			this->texCoords = texCoords;
		}
	};

	// mesh data
	vector<Vertex>       vertices;
	vector<unsigned int> indices;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices);

	Object* getParent();
	void setParent(Object* parent);

	Material* material;

	virtual void Draw();

protected:
	//  render data
	unsigned int VAO, VBO, EBO;

private:

	bool materialOwner;

	void setupMesh();
	void setUpShaderMatrices(unsigned int shaderProgram);
	void setUpShaderVariables(unsigned int shaderProgram);
	void setUpDirectionalLight(unsigned int shaderProgram);
	void setUpPointLights(unsigned int shaderProgram);

	Object* parent;

	bool once;
};