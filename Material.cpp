#include "Material.h"
#include "ResourceManager.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


Material::Material()
{
    setShaderProgram("assets/defaultAssets/Shaders/default.vert", "assets/defaultAssets/Shaders/default.frag");
    type = MaterialType::DEFAULT_LIT;
    anchorPoint = UIAnchorPoints::CENTER;
    twoSided = false;
}

Material::Material(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    setShaderProgram(vertexShaderPath, fragmentShaderPath);
    type = MaterialType::DEFAULT_LIT;
}

unsigned int Material::getShaderProgram()
{
    return shaderProgram;
}

void Material::setTexture(const char* path)
{
    textures.push_back(Resource.loadImage(path));
}

void Material::setTexture(const char* path, int index)
{
    textures[index] = Resource.loadImage(path);
}

void Material::setCubeMapTexture(vector<const char*> paths)
{
    skyboxTexture = Resource.loadCubeMap(paths);
}

void Material::loadTextures()
{
    for (int i = 0; i < textures.size(); i++)
    {
        setUniform<int>(string("texture[" + to_string(i) + "]"), i);

        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
}

void Material::setShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    std::string path = std::string(vertexShaderPath);
    path += fragmentShaderPath;
    unsigned int result = Resource.getShaderProgramFromMap(path);
    if (result != -1)
    {
        shaderProgram = result;
        return;
    }

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexShaderPath);
        fShaderFile.open(fragmentShaderPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file�s buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMILATION_FAILED\n" << infoLog << std::endl;
    }

    Resource.addShaderProgramToMap(path, shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
