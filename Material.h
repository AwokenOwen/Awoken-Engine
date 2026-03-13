#pragma once
#include "glm.hpp"
#include "glad/glad.h"
#include <gtc/type_ptr.hpp>
#include <vector>
#include <string>

using namespace glm;
using namespace std;

enum MaterialType
{
	DEFAULT_LIT,
	CUBEMAP,
	UI
};

enum UIAnchorPoints {
	CENTER,
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
};

class Material
{
public:
	Material();

	Material(const char* vertexShaderPath, const char* fragmentShaderPath);

	void setShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
	unsigned int getShaderProgram();

	void loadTextures();

	// Load Texture from path and place in Vector
	void setTexture(const char* path);
	// Load Texture from path and replace Texture in Vector 
	void setTexture(const char* path, int index);

	void setCubeMapTexture(vector<const char*> paths);

	vector<unsigned int> textures;

	MaterialType type;

	UIAnchorPoints anchorPoint;

	bool twoSided;

	template <typename T> void setUniform(std::string name, T value);

private:

	unsigned int skyboxTexture;

	// Shader Program
	unsigned int shaderProgram;
};

template<typename T>
inline void Material::setUniform(std::string name, T value)
{
	if (std::is_same_v<T, int> || std::is_same_v<T, bool>)
	{
		int uniform = glGetUniformLocation(shaderProgram, name.c_str());
		glUniform1i(uniform, *reinterpret_cast<int*>(&value));
		return;
	}
	if (std::is_same_v<T, float>)
	{
		int uniform = glGetUniformLocation(shaderProgram, name.c_str());
		glUniform1f(uniform, *reinterpret_cast<float*>(&value));
		return;
	}
	if (std::is_same_v<T, vec2>)
	{
		int uniform = glGetUniformLocation(shaderProgram, name.c_str());
		vec2 _value = *reinterpret_cast<vec2*>(&value);
		glUniform2f(uniform, _value.x, _value.y);
		return;
	}
	if (std::is_same_v<T, vec3>)
	{
		int uniform = glGetUniformLocation(shaderProgram, name.c_str());
		vec3 _value = *reinterpret_cast<vec3*>(&value);
		glUniform3f(uniform, _value.x, _value.y, _value.z);
		return;
	}
	if (std::is_same_v<T, vec4>)
	{
		int uniform = glGetUniformLocation(shaderProgram, name.c_str());
		vec4 _value = *reinterpret_cast<vec4*>(&value);
		glUniform4f(uniform, _value.x, _value.y, _value.z, _value.w);
		return;
	}
	if (std::is_same_v<T, mat4>)
	{
		int uniform = glGetUniformLocation(shaderProgram, name.c_str());
		mat4 _value = *reinterpret_cast<mat4*>(&value);
		glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(_value));
		return;
	}
}
