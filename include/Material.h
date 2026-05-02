//
// Created by awokenowen on 4/24/26.
//

#pragma once
#include "glad/glad.h"
#include <string>
#include "Math.h"

class Material {
public:
    Material();

    template<class T>
    void setUniform(const std::string &name, T value);

private:
    unsigned int m_shaderProgram;
};

template<typename T>
inline void Material::setUniform(const std::string& name, T value)
{
    glUseProgram(m_shaderProgram);
    int uniform = glGetUniformLocation(m_shaderProgram, name.c_str());

    if (std::is_same_v<T, int> || std::is_same_v<T, bool>)
    {
        glUniform1i(uniform, *reinterpret_cast<int*>(&value));
        return;
    }
    if (std::is_same_v<T, float>)
    {
        glUniform1f(uniform, *reinterpret_cast<float*>(&value));
        return;
    }
    if (std::is_same_v<T, Vector2>)
    {
        Vector2 _value = *reinterpret_cast<Vector2*>(&value);
        glUniform2f(uniform, _value.x, _value.y);
        return;
    }
    if (std::is_same_v<T, Vector3>)
    {
        Vector3 _value = *reinterpret_cast<Vector3*>(&value);
        glUniform3f(uniform, _value.x, _value.y, _value.z);
        return;
    }
    if (std::is_same_v<T, Vector4>)
    {
        Vector4 _value = *reinterpret_cast<Vector4*>(&value);
        glUniform4f(uniform, _value.x, _value.y, _value.z, _value.w);
        return;
    }
    if (std::is_same_v<T, Matrix2>)
    {
        Matrix2 _value = *reinterpret_cast<Matrix2*>(&value);
        glUniformMatrix2fv(uniform, 1, GL_FALSE, _value.toFloatArray());
        return;
    }
    if (std::is_same_v<T, Matrix3>)
    {
        Matrix3 _value = *reinterpret_cast<Matrix3*>(&value);
        glUniformMatrix3fv(uniform, 1, GL_FALSE, _value.toFloatArray());
        return;
    }
    if (std::is_same_v<T, Matrix4>)
    {
        Matrix4 _value = *reinterpret_cast<Matrix4*>(&value);
        glUniformMatrix4fv(uniform, 1, GL_FALSE, _value.toFloatArray());
        return;
    }
}
