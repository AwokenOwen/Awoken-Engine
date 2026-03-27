/*
 * Written by: AwokenOwen
 * Last Updated: March 27th 2026
 */

#pragma once
#include "glm.hpp"
#include "glad/glad.h"
#include <gtc/type_ptr.hpp>
#include <vector>
#include <string>

using namespace glm;
using namespace std;

/**
 * @brief Enum used to determine how to render the mesh
 */
enum MaterialType
{
	/**
	 * @brief load the uniforms in the PBR base shader and get 3D transform values
	 */
	DEFAULT_LIT,
	/**
	 * @brief load the skybox texture from the Active Scene and put it into the cube map base shader
	 */
	CUBE_MAP,
	/**
	 * @brief load the mesh as 2D and uniforms for the base UI shader
	 * position and scale are based on pixels
	 */
	UI,
	/**
	 * @brief load the mesh as 2D and uniforms for the base GUI shader
	 * position is based on [-1, 1] of screen
	 * scale is based on [0, 1] to fit the screen
	 */
	GUI,
};

/**
 * @brief the anchor/origin of the GUI/UI object with default being CENTER
 */
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

class Object;
/**
 * @brief Class that holds the shaders and handles the uniform loading of the default shaders
 */
class Material
{
public:
	/**
	 * @brief Default constructor creates the material as DEFAULT_LIT and loads the default shaders and sets the anchor point to the center of the screen
	 */
	Material();

	/**
	 * @brief Contractor sets the material as DEFAULT_LIT and sets the anchor point to the center of the screen but loads the given shader
	 *
	 * @param vertexShaderPath Path to the vertex shader
	 * @param fragmentShaderPath Path to the fragment shader
	 */
	Material(const char* vertexShaderPath, const char* fragmentShaderPath);

	/**
	 * @brief Setter for the Parent Object Variable
	 *
	 * @param parent The new Parent object
	 */
	void setParent(Object *parent);

	/**
	 * @brief Sets the shader of the material, loads if not loaded before or grabs from the resource manager if loaded before
	 *
	 * @param vertexShaderPath Path to the vertex shader
	 * @param fragmentShaderPath Path to the fragment shader
	 */
	void setShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
	/**
	 * @brief Getter for the unsigned int representing the shader program by the shader code
	 *
	 * @return int
	 */
	unsigned int getShaderProgram() const;

	/**
	 * Load all the textures stored in the material into the shader
	 */
	void loadTextures();

	/**
	 * @brief Adds texture from path into the vector of textures to get added to the shader
	 *
	 * @param path Path to image file
	 */
	void addTexture(const char* path);
	// Load Texture from path and replace Texture in Vector 
	/**
	 * @brief set existing texture in texture vector to a new texture
	 *
	 * @param path Path to image file
	 * @param index Index of the texture to replace in vector
	 */
	void setTexture(const char* path, int index);

	/**
	 * @brief sets the skybox texture used in the shader to a new cube map
	 *
	 * @param paths Path to the folder of cube map textures
	 */
	void setSkyboxTexture(const vector<const char*> &paths);

	/**
	 * @brief Used to set uniforms in the shader
	 *
	 * @tparam T The type of uniform to be set
	 * @param name Name of the uniform in the shader
	 * @param value Value to set the uniform to
	 */
	template <typename T> void setUniform(std::string name, T value);

	/**
	 * @brief Setter for the type of Material
	 *
	 * @param materialType the type the material is getting set to
	 */
	void setMaterialType(MaterialType materialType);
	/**
	 * @brief Getter for the material type
	 *
	 * @return MaterialType
	 */
	MaterialType getMaterialType() const;

	/**
	 * @brief Setter for the anchor point of a UI object. Will do nothing if not a GUI/UI object
	 *
	 * @param anchorPoint the new anchor point
	 */
	void setUIAnchorPoint(UIAnchorPoints anchorPoint);
	/**
	 * @brief Getter for the anchor point
	 *
	 * @return UIAnchorPoint
	 */
	UIAnchorPoints getUIAnchorPoint() const;

	/**
	 * @brief Setter for the two-sided variable. Will turn off face culling if true
	 *
	 * @param twoSided new two-sided value
	 */
	void setTwoSided(bool twoSided);
	/**
	 * @brief Getter for the two-sided variable
	 *
	 * @return bool
	 */
	bool getTwoSided() const;
	/**
	 * @brief Setter for the transparent variable, will put the object into the transparent list if turned on and put it in the opaque list if not
	 *
	 * @param transparent The new transparent variable
	 */
	void setTransparent(bool transparent);
	/**
	 * @brief Getter for the transparent variable
	 *
	 * @return Bool
	 */
	bool getTransparent() const;
private:

	/**
	 * @brief Parent Object Variable
	 */
	Object* m_parent;
	/**
	 * @brief Vector of textures to be added to the shader
	 */
	vector<unsigned int> m_textures;
	/**
	 * @brief Current skybox texture to be added to the shader
	 */
	unsigned int m_skyboxTexture;
	/**
	 * @brief Program created by openGL holding the compiled shaders
	 */
	unsigned int m_shaderProgram;
	/**
	 * @brief Material Type determining uniforms to be added to the base versions of the shaders
	 */
	MaterialType m_type;
	/**
	 * @brief Anchor point determining where the origin is for GUI/UI objects
	 */
	UIAnchorPoints m_anchorPoint;
	/**
	 * @brief Bool determining where to cull a face or not
	 */
	bool m_twoSided;
	/**
	 * @brief Bool variable for whether to turn on transparency
	 */
	bool m_transparent = false;
};

template<typename T>
inline void Material::setUniform(std::string name, T value)
{
	if (std::is_same_v<T, int> || std::is_same_v<T, bool>)
	{
		int uniform = glGetUniformLocation(m_shaderProgram, name.c_str());
		glUniform1i(uniform, *reinterpret_cast<int*>(&value));
		return;
	}
	if (std::is_same_v<T, float>)
	{
		int uniform = glGetUniformLocation(m_shaderProgram, name.c_str());
		glUniform1f(uniform, *reinterpret_cast<float*>(&value));
		return;
	}
	if (std::is_same_v<T, vec2>)
	{
		int uniform = glGetUniformLocation(m_shaderProgram, name.c_str());
		vec2 _value = *reinterpret_cast<vec2*>(&value);
		glUniform2f(uniform, _value.x, _value.y);
		return;
	}
	if (std::is_same_v<T, vec3>)
	{
		int uniform = glGetUniformLocation(m_shaderProgram, name.c_str());
		vec3 _value = *reinterpret_cast<vec3*>(&value);
		glUniform3f(uniform, _value.x, _value.y, _value.z);
		return;
	}
	if (std::is_same_v<T, vec4>)
	{
		int uniform = glGetUniformLocation(m_shaderProgram, name.c_str());
		vec4 _value = *reinterpret_cast<vec4*>(&value);
		glUniform4f(uniform, _value.x, _value.y, _value.z, _value.w);
		return;
	}
	if (std::is_same_v<T, mat4>)
	{
		int uniform = glGetUniformLocation(m_shaderProgram, name.c_str());
		mat4 _value = *reinterpret_cast<mat4*>(&value);
		glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(_value));
		return;
	}
}
