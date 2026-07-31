//
// Created by AwokenOwen on 4/14/26.
//
#pragma once
#include <cmath>
#include <numbers>

#include "nlohmann/json.hpp"

#define PI static_cast<float>(std::numbers::pi)

inline float toRadians(const float degrees) {
    return degrees * PI / 180.f;
};

inline float cot(const float a) {
    return 1.0f / std::tan(a);
};

class Matrix4;
class Matrix3;
class Matrix2;
class Quaternion;
class Vector4;
class Vector3;
class Vector2 {
public:
    Vector2() = default;
    Vector2(const float x, const float y){data[0] = x; data[1] = y;}
    explicit Vector2(const float data[2]) {this->data[0] = data[0];this->data[1] = data[1];}
    explicit Vector2(const Vector3 &v);
    explicit Vector2(const Vector4 &v);

    float data[2]{};

    [[nodiscard]] float x() const {return data[0];}
    [[nodiscard]] float y() const {return data[1];}

    [[nodiscard]] float magnitude() const;
    void Normalize();
    [[nodiscard]] Vector2 normalize() const;

    static float dot(const Vector2 &a, const Vector2 &b);
    static Vector2 lerp(Vector2& a, Vector2& b, float t);

#pragma region operators
    float& operator[](const int index) {
        return data[index];
    }
    const float& operator[](const int index) const {
        return data[index];
    }
    bool operator==(const Vector2& other) const
    {
        bool result = true;
        for (int i = 0; i < 2; i++)
        {
            if (data[i] != other.data[i])
                result = false;
        }
        return result;
    }
    Vector2& operator=(const Vector2& other)
    {
        if (this == &other)
            return *this;
        for (int i = 0; i < 2; i++)
        {
            data[i] = other.data[i];
        }
        return *this;
    }
    Vector2& operator+=(const Vector2& other)
    {
        for (int i = 0; i < 2; i++)
        {
            data[i] += other.data[i];
        }
        return *this;
    }
    Vector2& operator-=(const Vector2& other)
    {
        for (int i = 0; i < 2; i++)
        {
            data[i] -= other.data[i];
        }
        return *this;
    }
    Vector2& operator*=(const Vector2& other)
    {
        for (int i = 0; i < 2; i++)
        {
            data[i] *= other.data[i];
        }
        return *this;
    }
    Vector2& operator/=(const Vector2& other)
    {
        for (int i = 0; i < 2; i++)
        {
            data[i] /= other.data[i];
        }
        return *this;
    }
    friend Vector2 operator+(Vector2 lhs, const Vector2& rhs) {
        lhs += rhs;
        return lhs;
    }
    friend Vector2 operator-(Vector2 lhs, const Vector2& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
    friend Vector2 operator*(Vector2 lhs, const Vector2& rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    friend Vector2 operator/(Vector2 lhs, const Vector2& rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    Vector2& operator*=(const float other)
    {
        for (float & i : data)
        {
            i *= other;
        }
        return *this;
    }
    Vector2& operator/=(const float other)
    {
        for (float & i : data)
        {
            i /= other;
        }
        return *this;
    }
    friend Vector2 operator*(const Vector2& lhs, const float rhs)
    {
        return {lhs.x() * rhs, lhs.y() * rhs};
    }
    friend Vector2 operator*(const float lhs, const Vector2& rhs)
    {
        return {rhs.x() * lhs, rhs.y() * lhs};
    }
    friend Vector2 operator/(Vector2& lhs, const float rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    friend Vector2 operator/(const float lhs, Vector2& rhs)
    {
        rhs /= lhs;
        return rhs;
    }
#pragma endregion operators

    Vector2& operator=(const Vector3& other);
    Vector2& operator=(const Vector4& other);

    static Vector2 fromJson(const nlohmann::json& json);
    nlohmann::json toJson();
};
class Vector3 {
public:
    Vector3() = default;
    Vector3(const float x, const float y, const float z){data[0] = x; data[1] = y; data[2] = z;}
    Vector3(const Vector2 &v, float z);
    Vector3(float x, const Vector2 &v);
    explicit Vector3(const float data[3]) {this->data[0] = data[0];this->data[1] = data[1];this->data[2] = data[2];}
    explicit Vector3(const Vector2 &v);
    explicit Vector3(const Vector4 &v);

    float data[3]{};

    [[nodiscard]] float x() const {return data[0];}
    [[nodiscard]] float y() const {return data[1];}
    [[nodiscard]] float z() const {return data[2];}

    [[nodiscard]] float magnitude() const;
    void Normalize();
    [[nodiscard]] Vector3 normalize() const;

    [[nodiscard]] static Vector3 cross(const Vector3 &a, const Vector3 &b);
    [[nodiscard]] static float dot(const Vector3 &a, const Vector3 &b);
    [[nodiscard]] static Vector3 lerp(Vector3 &a, Vector3 &b, float t);
    [[nodiscard]] static Vector3 forward();
    [[nodiscard]] static Vector3 right();
    [[nodiscard]] static Vector3 up();

#pragma region operators
    float& operator[](const int index) {
        return data[index];
    }
    const float& operator[](const int index) const {
        return data[index];
    }
    bool operator==(const Vector3& other) const
    {
        bool result = true;
        for (int i = 0; i < 3; i++)
        {
            if (data[i] != other.data[i])
                result = false;
        }
        return result;
    }
    Vector3& operator=(const Vector3& other)
    {
        if (this == &other)
            return *this;
        for (int i = 0; i < 3; i++)
        {
            data[i] = other.data[i];
        }
        return *this;
    }
    Vector3& operator+=(const Vector3& other)
    {
        for (int i = 0; i < 3; i++)
        {
            data[i] += other.data[i];
        }
        return *this;
    }
    Vector3& operator-=(const Vector3& other)
    {
        for (int i = 0; i < 3; i++)
        {
            data[i] -= other.data[i];
        }
        return *this;
    }
    Vector3& operator*=(const Vector3& other)
    {
        for (int i = 0; i < 3; i++)
        {
            data[i] *= other.data[i];
        }
        return *this;
    }
    Vector3& operator/=(const Vector3& other)
    {
        for (int i = 0; i < 3; i++)
        {
            data[i] /= other.data[i];
        }
        return *this;
    }
    friend Vector3 operator+(Vector3 lhs, const Vector3& rhs) {
        lhs += rhs;
        return lhs;
    }
    friend Vector3 operator-(Vector3 lhs, const Vector3& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
    friend Vector3 operator*(Vector3 lhs, const Vector3& rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    friend Vector3 operator/(Vector3 lhs, const Vector3& rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    friend Vector3 operator+(Vector3 lhs, Vector3& rhs) {
        lhs += rhs;
        return lhs;
    }
    friend Vector3 operator-(Vector3 lhs, Vector3& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
    friend Vector3 operator*(Vector3 lhs, Vector3& rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    friend Vector3 operator/(Vector3 lhs, Vector3& rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    Vector3& operator*=(const float other)
    {
        for (float & i : data)
        {
            i *= other;
        }
        return *this;
    }
    Vector3& operator/=(const float other)
    {
        for (float & i : data)
        {
            i /= other;
        }
        return *this;
    }

    friend Vector3 operator*(const Vector3& lhs, const float rhs)
    {
        Vector3 result{lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs};
        return result;
    }
    friend Vector3 operator*(const float lhs, const Vector3& rhs)
    {
        Vector3 result{rhs.x() * lhs, rhs.y() * lhs, rhs.z() * lhs};
        return result;
    }
    friend Vector3 operator/(Vector3& lhs, const float rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    friend Vector3 operator/(const float lhs, Vector3& rhs)
    {
        rhs /= lhs;
        return rhs;
    }
#pragma endregion operators

    Vector3& operator=(const Vector2& other);
    Vector3& operator=(const Vector4& other);

    static Vector3 fromJson(nlohmann::json json);
    nlohmann::json toJson();
};
class Vector4 {
public:
    Vector4(){data[0] = data[1] = data[2] = 0.f; data[3] = 1.f;}
    Vector4(const float x, const float y, const float z, const float w){data[0] = x; data[1] = y; data[2] = z; data[3] = w;}
    Vector4(const Vector3 &v, float w);
    Vector4(float x, const Vector3 &v);
    Vector4(const Vector2 &v, float z, float w);
    Vector4(float x, const Vector2 &v, float w);
    Vector4(float x, float y, const Vector2 &v);
    explicit Vector4(const float data[4]){this->data[0] = data[0];this->data[1] = data[1];this->data[2] = data[2];this->data[3] = data[3];}
    explicit Vector4(const Vector3 &v);
    explicit Vector4(const Vector2 &v);

    float data[4]{};

    [[nodiscard]] float x() const {return data[0];}
    [[nodiscard]] float y() const {return data[1];}
    [[nodiscard]] float z() const {return data[2];}
    [[nodiscard]] float w() const {return data[3];}

    [[nodiscard]] float magnitude() const;
    void Normalize();
    [[nodiscard]] Vector4 normalize() const;

    static float dot(const Vector4 &a, const Vector4 &b);
    static Vector4 lerp(Vector4 &a, Vector4 &b, float t);

#pragma region operators
    float& operator[](const int index) {
        return data[index];
    }
    const float& operator[](const int index) const {
        return data[index];
    }
    bool operator==(const Vector4& other) const
    {
        bool result = true;
        for (int i = 0; i < 4; i++)
        {
            if (data[i] != other.data[i])
                result = false;
        }
        return result;
    }
    Vector4& operator=(const Vector4& other)
    {
        if (this == &other)
            return *this;
        for (int i = 0; i < 4; i++)
        {
            data[i] = other.data[i];
        }
        return *this;
    }
    Vector4& operator+=(const Vector4& other)
    {
        for (int i = 0; i < 4; i++)
        {
            data[i] += other.data[i];
        }
        return *this;
    }
    Vector4& operator-=(const Vector4& other)
    {
        for (int i = 0; i < 4; i++)
        {
            data[i] -= other.data[i];
        }
        return *this;
    }
    Vector4& operator*=(const Vector4& other)
    {
        for (int i = 0; i < 4; i++)
        {
            data[i] *= other.data[i];
        }
        return *this;
    }
    Vector4& operator/=(const Vector4& other)
    {
        for (int i = 0; i < 4; i++)
        {
            data[i] /= other.data[i];
        }
        return *this;
    }
    friend Vector4 operator+(Vector4 lhs, const Vector4& rhs) {
        lhs += rhs;
        return lhs;
    }
    friend Vector4 operator-(Vector4 lhs, const Vector4& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
    friend Vector4 operator*(Vector4 lhs, const Vector4& rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    friend Vector4 operator/(Vector4 lhs, const Vector4& rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    Vector4& operator*=(const float other)
    {
        for (float & i : data)
        {
            i *= other;
        }
        return *this;
    }
    Vector4& operator/=(const float other)
    {
        for (float & i : data)
        {
            i /= other;
        }
        return *this;
    }
    friend Vector4 operator*(const Vector4& lhs, const float rhs)
    {
        return {lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs, lhs.w() * rhs};
    }
    friend Vector4 operator*(const float lhs, const Vector4& rhs)
    {
        return {lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z(), rhs.w() * lhs};
    }
    friend Vector4 operator/(Vector4& lhs, const float rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    friend Vector4 operator/(const float lhs, Vector4& rhs)
    {
        rhs /= lhs;
        return rhs;
    }
#pragma endregion operators

    Vector4& operator=(const Vector2& other);
    Vector4& operator=(const Vector3& other);

    static Vector4 fromJson(nlohmann::json json);
    [[nodiscard]] nlohmann::json toJson() const;
};
class Quaternion {
public:
    Quaternion(){data[0] = data[1] = data[2] = 0.f; data[3] = 1.f;}
    Quaternion(const float x, const float y, const float z, const float w){data[0] = x; data[1] = y; data[2] = z; data[3] = w;}
    Quaternion(Vector3 &axis, float angle);
    explicit Quaternion(const Vector3 &EulerAngles);
    explicit Quaternion(const Matrix4 &m);

    float data[4]{};

    [[nodiscard]] float x() const {return data[0];}
    [[nodiscard]] float y() const {return data[1];}
    [[nodiscard]] float z() const {return data[2];}
    [[nodiscard]] float w() const {return data[3];}

    [[nodiscard]] float magnitude() const;
    void Normalize();
    [[nodiscard]] Quaternion normalize() const;
    [[nodiscard]] Vector3 eulerAngles() const;
    [[nodiscard]] Matrix4 toMatrix() const;

    static float dot(const Quaternion &a, const Quaternion &b);
    static Quaternion slerp(Quaternion &a, Quaternion &b, float t);

    #pragma region operators
    float& operator[](const int index) {
        return data[index];
    }
    const float& operator[](const int index) const {
        return data[index];
    }
    bool operator==(const Quaternion& other) const
    {
        bool result = true;
        for (int i = 0; i < 4; i++)
        {
            if (data[i] != other.data[i])
                result = false;
        }
        return result;
    }
    Quaternion& operator=(const Quaternion& other)
    {
        if (this == &other)
            return *this;
        for (int i = 0; i < 4; i++)
        {
            data[i] = other.data[i];
        }
        return *this;
    }
    Quaternion& operator+=(const Quaternion& other)
    {
        for (int i = 0; i < 4; i++)
        {
            data[i] += other.data[i];
        }
        return *this;
    }
    Quaternion& operator-=(const Quaternion& other)
    {
        for (int i = 0; i < 4; i++)
        {
            data[i] -= other.data[i];
        }
        return *this;
    }
    Quaternion& operator*=(const Quaternion& other)
    {
        for (int i = 0; i < 4; i++)
        {
            data[i] *= other.data[i];
        }
        return *this;
    }
    Quaternion& operator/=(const Quaternion& other)
    {
        for (int i = 0; i < 4; i++)
        {
            data[i] /= other.data[i];
        }
        return *this;
    }
    friend Quaternion operator+(Quaternion lhs, const Quaternion& rhs) {
        lhs += rhs;
        return lhs;
    }
    friend Quaternion operator-(Quaternion lhs, const Quaternion& rhs)
    {
        lhs -= rhs;
        return lhs;
    }
    friend Quaternion operator*(Quaternion lhs, const Quaternion& rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    friend Quaternion operator/(Quaternion lhs, const Quaternion& rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    Quaternion& operator*=(const float other)
    {
        for (float & i : data)
        {
            i *= other;
        }
        return *this;
    }
    Quaternion& operator/=(const float other)
    {
        for (float & i : data)
        {
            i /= other;
        }
        return *this;
    }
    friend Quaternion operator*(Quaternion& lhs, const float rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    friend Quaternion operator*(const float lhs, Quaternion& rhs)
    {
        rhs *= lhs;
        return rhs;
    }
    friend Quaternion operator/(Quaternion& lhs, const float rhs)
    {
        lhs /= rhs;
        return lhs;
    }
    friend Quaternion operator/(const float lhs, Quaternion& rhs)
    {
        rhs /= lhs;
        return rhs;
    }
#pragma endregion operators

    static Quaternion fromJson(nlohmann::json json);
    [[nodiscard]] nlohmann::json toJson() const;
};
class Matrix2 {
public:
    Matrix2()
    {
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if (i == j)
                {
                    this->data[i][j] = 1.f;
                    continue;
                }
                this->data[i][j] = 0.f;
            }
        }
    }
    explicit Matrix2(Vector2 data[2])
    {
        this->data[0] = data[0];
        this->data[1] = data[1];
    }
    explicit Matrix2(const Matrix3 &a);
    explicit Matrix2(const Matrix4 &a);

    Vector2 data[2]{};

    void Transpose();
    [[nodiscard]] Matrix2 transpose() const;

    void Inverse();
    [[nodiscard]] Matrix2 inverse() const;

#pragma region operators

    Vector2& operator[](const int index) {
        return data[index];
    }

    const Vector2& operator[](const int index) const {
        return data[index];
    }

    bool operator==(const Matrix2& other) const
    {
        bool result = true;
        for (int i = 0; i < 2; i++)
        {
            if (data[i] != other.data[i])
                result = false;
        }
        return result;
    }

    Matrix2& operator=(const Matrix2& other)
    {
        if (this == &other)
        {
            return *this;
        }
        for (int i = 0; i < 2; i++)
        {
            data[i] = other.data[i];
        }
        return *this;
    }

    Matrix2& operator*=(const Matrix2& other)
    {
        Matrix2 result;

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    result[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }

        *this = result;
        return *this;
    }
    friend Matrix2 operator*(Matrix2 lhs, const Matrix2& rhs)
    {
        lhs *= rhs;
        return lhs;
    }

#pragma endregion operators

    Matrix2& operator=(const Matrix3& other);
    Matrix2& operator=(const Matrix4& other);

    static Matrix2 fromJson(const nlohmann::json& json);
    nlohmann::json toJson();
};
class Matrix3 {
public:
    Matrix3()
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if (i == j)
                {
                    this->data[i][j] = 1.f;
                    continue;
                }
                this->data[i][j] = 0.f;
            }
        }
    }
    explicit Matrix3(Vector3 data[3])
    {
        this->data[0] = data[0];
        this->data[1] = data[1];
        this->data[2] = data[2];
    }
    explicit Matrix3(const Matrix2 &a);
    explicit Matrix3(const Matrix4 &a);

    Vector3 data[3]{};

    void Transpose();
    [[nodiscard]] Matrix3 transpose() const;

    void Inverse();
    [[nodiscard]] Matrix3 inverse() const;

#pragma region operators

    Vector3& operator[](const int index) {
        return data[index];
    }

    const Vector3& operator[](const int index) const {
        return data[index];
    }

    bool operator==(const Matrix3& other) const
    {
        bool result = true;
        for (int i = 0; i < 3; i++)
        {
            if (data[i] != other.data[i])
                result = false;
        }
        return result;
    }

    Matrix3& operator=(const Matrix3& other)
    {
        if (this == &other)
        {
            return *this;
        }
        for (int i = 0; i < 3; i++)
        {
            data[i] = other.data[i];
        }
        return *this;
    }

    Matrix3& operator*=(const Matrix3& other)
    {
        Matrix3 result;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                for (int k = 0; k < 3; k++)
                {
                    result[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }

        *this = result;
        return *this;
    }
    friend Matrix3 operator*(Matrix3 lhs, const Matrix3& rhs)
    {
        lhs *= rhs;
        return lhs;
    }

#pragma endregion operators

    Matrix3& operator=(const Matrix2& other);
    Matrix3& operator=(const Matrix4& other);

    static Matrix3 fromJson(nlohmann::json json);
    nlohmann::json toJson();
};
class Matrix4 {
public:
    Matrix4()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                if (i == j)
                {
                    this->data[i][j] = 1.f;
                    continue;
                }
                this->data[i][j] = 0.f;
            }
        }
    }

    explicit Matrix4(Vector4 data[4])
    {
        this->data[0] = data[0];
        this->data[1] = data[1];
        this->data[2] = data[2];
        this->data[3] = data[3];
    }
    explicit Matrix4(const Matrix2 &a);
    explicit Matrix4(const Matrix3 &a);

    Vector4 data[4]{};

    void Transpose();
    [[nodiscard]] Matrix4 transpose() const;

    void Inverse();
    [[nodiscard]] Matrix4 inverse() const;

    [[nodiscard]] static Matrix4 Translate(const Matrix4& org, const Vector3& translation);
    [[nodiscard]] static Matrix4 Rotate(const Matrix4& org, Vector3 axis, float angle);
    [[nodiscard]] static Matrix4 Rotate(const Matrix4& org, const Quaternion& rotation);
    [[nodiscard]] static Matrix4 Scale(const Matrix4& org, const Vector3& scale);
    [[nodiscard]] static Matrix4 ModelMatrix(const Vector3 &position, const Quaternion &rotation, const Vector3 &scale);
    [[nodiscard]] static Matrix4 LookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up);
    [[nodiscard]] static Matrix4 PerspectiveMatrix(float fov, float aspect, float near, float far);
    [[nodiscard]] static Matrix4 OrthographicMatrix(float left, float right, float bottom, float top, float near, float far);

#pragma region operators

    Vector4& operator[](const int index) {
        return data[index];
    }

    const Vector4& operator[](const int index) const {
        return data[index];
    }

    bool operator==(const Matrix4& other) const
    {
        bool result = true;
        for (int i = 0; i < 4; i++)
        {
            if (data[i] != other.data[i])
                result = false;
        }
        return result;
    }

    Matrix4& operator=(const Matrix4& other)
    {
        if (this == &other)
        {
            return *this;
        }
        for (int i = 0; i < 4; i++)
        {
            data[i] = other.data[i];
        }
        return *this;
    }

    Matrix4& operator*=(const Matrix4& other)
    {
        Vector4 result[4];
        for (int col = 0; col < 4; col++)
            for (int row = 0; row < 4; row++)
            {
                float sum = 0.f;
                for (int k = 0; k < 4; k++)
                    sum += data[k][row] * other.data[col][k];
                result[col][row] = sum;
            }
        *this = Matrix4(result);
        return *this;
    }
    friend Matrix4 operator*(Matrix4 lhs, const Matrix4& rhs)
    {
        lhs *= rhs;
        return lhs;
    }
    Vector4 operator*(const Vector4& v) const
    {
        Vector4 result(0.f, 0.f, 0.f, 0.f);
        for (int col = 0; col < 4; col++)
            for (int row = 0; row < 4; row++)
                result[row] += data[col][row] * v[col];
        return result;
    }
#pragma endregion operators

    Matrix4& operator=(const Matrix2& other);
    Matrix4& operator=(const Matrix3& other);

    static Matrix4 fromJson(nlohmann::json json);
    [[nodiscard]] nlohmann::json toJson() const;
};

#pragma region det

inline float det2(Matrix2 a)
{
    return a[0][0] * a[1][1] - a[0][1] * a[1][0];
}

inline float det3(Matrix3 a)
{
    const auto d =
        a[0][0]*a[1][1]*a[2][2]
      + a[0][1]*a[1][2]*a[2][0]
      + a[0][2]*a[2][1]*a[1][0]
      - a[2][0]*a[1][1]*a[0][2]
      - a[1][0]*a[0][1]*a[2][2]
      - a[0][0]*a[1][2]*a[2][1];
    return d;
}

inline float det4(Matrix4 a)
{
    Vector3 v0[3] = {
        Vector3(a[1][1], a[1][2], a[1][3]),
        Vector3(a[2][1], a[2][2], a[2][3]),
        Vector3(a[3][1], a[3][2], a[3][3])
    };
    const Matrix3 m0(v0);
    Vector3 v1[3] = {
        Vector3(a[1][0], a[1][2], a[1][3]),
        Vector3(a[2][0], a[2][2], a[2][3]),
        Vector3(a[3][0], a[3][2], a[3][3])
    };
    const Matrix3 m1(v1);
    Vector3 v2[3] = {
        Vector3(a[1][0], a[1][1], a[1][3]),
        Vector3(a[2][0], a[2][1], a[2][3]),
        Vector3(a[3][0], a[3][1], a[3][3])
    };
    const Matrix3 m2(v2);
    Vector3 v3[3] = {
        Vector3(a[1][0], a[1][1], a[1][2]),
        Vector3(a[2][0], a[2][1], a[2][2]),
        Vector3(a[3][0], a[3][1], a[3][2])
    };
    const Matrix3 m3(v3);
    return a[0][0]*det3(m0) - a[0][1]*det3(m1)
        + a[0][2]*det3(m2) - a[0][3]*det3(m3);
}
#pragma endregion det

#pragma region Vector2

inline Vector2::Vector2(const Vector3& v)
{
    data[0] = v.x();
    data[1] = v.y();
}

inline Vector2::Vector2(const Vector4& v)
{
    data[0] = v.x();
    data[1] = v.y();
}

inline float Vector2::magnitude() const
{
    return std::sqrt(data[0] * data[0] + data[1] * data[1]);
}

inline void Vector2::Normalize()
{
    *this /= magnitude();
}

inline Vector2 Vector2::normalize() const
{
    auto result = *this;
    result.Normalize();

    return result;
}

inline float Vector2::dot(const Vector2& a, const Vector2& b)
{
    return a.x() * b.x() + a.y() * b.y();
}

inline Vector2 Vector2::lerp(Vector2& a, Vector2& b, const float t)
{
    return (1 - t) * a + t * b;
}

inline Vector2& Vector2::operator=(const Vector3& other)
{
    data[0] = other.x();
    data[1] = other.y();
    return *this;
}

inline Vector2& Vector2::operator=(const Vector4& other)
{
    data[0] = other.x();
    data[1] = other.y();
    return *this;
}

inline Vector2 Vector2::fromJson(const nlohmann::json& json)
{
    Vector2 result;
    result.data[0] = json["x"].get<float>();
    result.data[1] = json["y"].get<float>();

    return result;
}

inline nlohmann::json Vector2::toJson()
{
    nlohmann::json result;
    result["x"] = data[0];
    result["y"] = data[1];

    return result;
}

#pragma endregion Vector2

#pragma region Vector3

inline Vector3::Vector3(const Vector2& v)
{
    data[0] = v.x();
    data[1] = v.y();
    data[2] = 0.f;
}

inline Vector3::Vector3(const Vector2& v, const float z)
{
    data[0] = v.x();
    data[1] = v.y();
    data[2] = z;
}

inline Vector3::Vector3(const float x, const Vector2& v)
{
    data[0] = x;
    data[1] = v.x();
    data[2] = v.y();
}

inline Vector3::Vector3(const Vector4& v)
{
    data[0] = v.x();
    data[1] = v.y();
    data[2] = v.z();
}

inline float Vector3::magnitude() const
{
    return std::sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
}

inline void Vector3::Normalize()
{
    *this /= magnitude();
}

inline Vector3 Vector3::normalize() const
{
    Vector3 result = *this;
    result.Normalize();

    return result;
}

inline Vector3 Vector3::cross(const Vector3& a, const Vector3& b)
{
    Vector3 result;
    result.data[0] = a.y() * b.z() - a.z() * b.y();
    result.data[1] = a.z() * b.x() - a.x() * b.z();
    result.data[2] = a.x() * b.y() - a.y() * b.x();

    return result;
}

inline float Vector3::dot(const Vector3& a, const Vector3& b)
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

inline Vector3 Vector3::lerp(Vector3& a, Vector3& b, float t)
{
    return (1.0f - t) * a + t * b;
}

inline Vector3 Vector3::forward()
{
    return {0, 0, -1};
}

inline Vector3 Vector3::right()
{
    return {1, 0, 0};
}

inline Vector3 Vector3::up()
{
    return {0, 1, 0};
}

inline Vector3& Vector3::operator=(const Vector2& other)
{
    data[0] = other.x();
    data[1] = other.y();
    data[2] = 0.f;
    return *this;
}

inline Vector3& Vector3::operator=(const Vector4& other)
{
    data[0] = other.x();
    data[1] = other.y();
    data[2] = other.z();
    return *this;
}

inline Vector3 Vector3::fromJson(nlohmann::json json)
{
    Vector3 result;
    result.data[0] = json["x"].get<float>();
    result.data[1] = json["y"].get<float>();
    result.data[2] = json["z"].get<float>();
    return result;
}

inline nlohmann::json Vector3::toJson()
{
    nlohmann::json j;
    j["x"] = data[0];
    j["y"] = data[1];
    j["z"] = data[2];
    return j;
}

#pragma endregion Vector3

#pragma region Vector4

inline Vector4::Vector4(const Vector3& v)
{
    data[0] = v.x();
    data[1] = v.y();
    data[2] = v.z();
    data[3] = 1.f;
}

inline Vector4::Vector4(const Vector3& v, const float w)
{
    data[0] = v.x();
    data[1] = v.y();
    data[2] = v.z();
    data[3] = w;
}

inline Vector4::Vector4(const float x, const Vector3& v)
{
    data[0] = x;
    data[1] = v.x();
    data[2] = v.y();
    data[3] = v.z();
}

inline Vector4::Vector4(const Vector2& v)
{
    data[0] = v.x();
    data[1] = v.y();
    data[2] = 0.f;
    data[3] = 1.f;
}

inline Vector4::Vector4(const Vector2& v, const float z, const float w)
{
    data[0] = v.x();
    data[1] = v.y();
    data[2] = z;
    data[3] = w;
}

inline Vector4::Vector4(const float x, const Vector2& v, const float w)
{
    data[0] = x;
    data[1] = v.x();
    data[2] = v.y();
    data[3] = w;
}

inline Vector4::Vector4(const float x, const float y, const Vector2& v)
{
    data[0] = x;
    data[1] = y;
    data[2] = v.x();
    data[3] = v.y();
}

inline float Vector4::magnitude() const
{
    return std::sqrt(x() * x() + y() * y() + z() * z() + w() * w());
}

inline void Vector4::Normalize()
{
    *this /= magnitude();
}

inline Vector4 Vector4::normalize() const
{
    Vector4 result = *this;
    result.Normalize();
    return result;
}

inline float Vector4::dot(const Vector4& a, const Vector4& b)
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z() + a.w() * b.w();
}

inline Vector4 Vector4::lerp(Vector4& a, Vector4& b, const float t)
{
    return (1.0f - t) * a + t * b;
}

inline Vector4& Vector4::operator=(const Vector2& other)
{
    data[0] = other.x();
    data[1] = other.y();
    data[2] = 0.f;
    data[3] = 1.f;
    return *this;
}

inline Vector4& Vector4::operator=(const Vector3& other)
{
    data[0] = other.x();
    data[1] = other.y();
    data[2] = other.z();
    data[3] = 1.f;
    return *this;
}

inline Vector4 Vector4::fromJson(nlohmann::json json)
{
    Vector4 result;
    result.data[0] = json["x"].get<float>();
    result.data[1] = json["y"].get<float>();
    result.data[2] = json["z"].get<float>();
    result.data[3] = json["w"].get<float>();
    return result;
}

inline nlohmann::json Vector4::toJson() const
{
    nlohmann::json j;
    j["x"] = data[0];
    j["y"] = data[1];
    j["z"] = data[2];
    j["w"] = data[3];
    return j;
}

#pragma endregion Vector4

#pragma region Quaternion

inline Quaternion::Quaternion(Vector3& axis, const float angle)
{
    axis.Normalize();
    const auto s = std::sin(angle / 2);

    data[0] = axis.x() * s;
    data[1] = axis.y() * s;
    data[2] = axis.z() * s;
    data[3] = std::cos(angle / 2);
}

inline Quaternion::Quaternion(const Vector3& EulerAngles)
{
    const auto rs = std::sin(toRadians(EulerAngles.x()) / 2);
    const auto rc = std::cos(toRadians(EulerAngles.x()) / 2);
    const auto ps = std::sin(toRadians(EulerAngles.y()) / 2);
    const auto pc = std::cos(toRadians(EulerAngles.y()) / 2);
    const auto ys = std::sin(toRadians(EulerAngles.z()) / 2);
    const auto yc = std::cos(toRadians(EulerAngles.z()) / 2);

    data[0] = yc * pc * rs - ys * ps * rc;
    data[1] = yc * ps * rc + ys * pc * rs;
    data[2] = ys * pc * rc - yc * ps * rs;
    data[3] = yc * pc * rc + ys * ps * rs;
}

inline Quaternion::Quaternion(const Matrix4 &m) {

    auto tw = 1 + m[0][0] + m[1][1] + m[2][2];
    auto tx = 1 + m[0][0] - m[1][1] - m[2][2];
    auto ty = 1 - m[0][0] + m[1][1] - m[2][2];
    auto tz = 1 - m[0][0] - m[1][1] + m[2][2];

    auto max = std::numeric_limits<float>::min();
    if (tw > max)
        max = tw;
    if (ty > max)
        max = ty;
    if (tx > max)
        max = tx;
    if (tz > max)
        max = tz;

    float _w{};
    float _x{};
    float _y{};
    float _z{};

    if (tw == max)
    {
        _w = std::sqrt(tw) / 2.0f;
        _x = (m[2][1] - m[1][2]) / (4.0f * _w);
        _y = (m[0][2] - m[2][0]) / (4.0f * _w);
        _z = (m[1][0] - m[0][1]) / (4.0f * _w);
    }else if (tx == max)
    {
        _x = std::sqrt(tx) / 2.0f;
        _w = (m[2][1] - m[1][2]) / (4.0f * _x);
        _y = (m[0][1] + m[1][0]) / (4.0f * _x);
        _z = (m[0][2] + m[2][0]) / (4.0f * _x);
    }else if (ty == max)
    {
        _y = std::sqrt(ty) / 2.0f;
        _w = (m[0][2] - m[2][0]) / (4.0f * _y);
        _x = (m[0][1] + m[1][0]) / (4.0f * _y);
        _z = (m[1][2] + m[2][1]) / (4.0f * _y);
    }else
    {
        _z = std::sqrt(tz) / 2.0f;
        _w = (m[0][2] - m[2][0]) / (4.0f * _z);
        _x = (m[0][1] + m[1][0]) / (4.0f * _z);
        _y = (m[1][2] + m[2][1]) / (4.0f * _z);
    }

    data[0] = _x;
    data[1] = _y;
    data[2] = _z;
    data[3] = _w;
}

inline float Quaternion::magnitude() const
{
    return std::sqrt(x() * x() + y() * y() + z() * z() + w() * w());
}

inline void Quaternion::Normalize()
{
    const auto mag = magnitude();
    for (float & i : data)
    {
        i /= mag;
    }
}

inline Quaternion Quaternion::normalize() const
{
    Quaternion result = *this;
    result.Normalize();
    return result;
}

inline Vector3 Quaternion::eulerAngles() const
{
    return {
        std::atan2(2 * (w()*x() + y()*z()),1 - 2 * (x() * x() + y() * y())),
        std::asin(2 * (w()*y() + x()*z())),
        std::atan2(2 * (w()*z() + y()*x()),1 - 2 * (y() * y() + z() * z()))
    };
}

inline Matrix4 Quaternion::toMatrix() const
{
    Vector4 m[4] = {
        Vector4(1.0f - 2.0f*y()*y() - 2.0f*z()*z(),2.0f*x()*y() - 2.0f*z()*w(),2.0f*x()*z() + 2.0f*y()*w(), 0.f),
        Vector4(2.0f*x()*y() + 2.0f*z()*w(),            1.0f - 2.0f*x()*x() - 2.0f*z()*z(), 2.0f*y()*z() - 2.0f*x()*w(), 0.f),
        Vector4(2.0f*x()*z() - 2.0f*y()*w(),            2.0f*y()*z() + 2.0f*x()*w(),        1.0f - 2.0f*x()*x() - 2.0f*y()*y(), 0.f),
        Vector4(0.f,0.f,0.f,1.f)
    };

    return Matrix4(m);
}

inline float Quaternion::dot(const Quaternion& a, const Quaternion& b)
{
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z() + a.w() * b.w();
}

inline Quaternion Quaternion::slerp(Quaternion& a, Quaternion& b, const float t)
{
    const float angle = std::acos(std::abs(dot(a, b)));

    if (angle == 0)
        return a;

    const float sin_t = std::sin(t * angle);
    const float sin_one_minus_t = std::sin((1.0f-t) * angle);
    const float sin_angle = std::sin(angle);

    auto q = a * sin_one_minus_t + b * sin_t;
    return q / sin_angle;
}

inline Quaternion Quaternion::fromJson(nlohmann::json json)
{
    Quaternion result;
    result.data[0] = json["x"].get<float>();
    result.data[1] = json["y"].get<float>();
    result.data[2] = json["z"].get<float>();
    result.data[3] = json["w"].get<float>();
    return result;
}

inline nlohmann::json Quaternion::toJson() const
{
    nlohmann::json result;
    result["x"] = x();
    result["y"] = y();
    result["z"] = z();
    result["w"] = w();
    return result;
}
#pragma endregion Quaternion

#pragma region Matrix2

inline Matrix2::Matrix2(const Matrix3& a)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            data[i][j] = a[i][j];
        }
    }
}

inline Matrix2::Matrix2(const Matrix4& a)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            data[i][j] = a[i][j];
        }
    }
}

inline void Matrix2::Transpose()
{
    auto m = *this;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            data[j][i] = m.data[i][j];
        }
    }
}

inline Matrix2 Matrix2::transpose() const
{
    auto m = *this;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            m.data[j][i] = data[i][j];
        }
    }
    return m;
}

inline void Matrix2::Inverse()
{
    auto m = *this;
    const auto d = det2(*this);
    data[0][0] = m[1][1] / d;
    data[0][1] = -m[0][1] / d;
    data[1][0] = -m[1][0] / d;
    data[1][1] = m[0][0] / d;
}

inline Matrix2 Matrix2::inverse() const
{
    auto m = *this;
    const auto d = det2(*this);
    m[0][0] = data[1][1] / d;
    m[0][1] = -data[0][1] / d;
    m[1][0] = -data[1][0] / d;
    m[1][1] = data[0][0] / d;

    return m;
}

inline Matrix2& Matrix2::operator=(const Matrix3& other)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            data[i][j] = other.data[i][j];
        }
    }
    return *this;
}

inline Matrix2& Matrix2::operator=(const Matrix4& other)
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            data[i][j] = other.data[i][j];
        }
    }
    return *this;
}

inline Matrix2 Matrix2::fromJson(const nlohmann::json& json)
{
    Matrix2 result;
    result.data[0] = Vector2::fromJson(json["col1"]);
    result.data[1] = Vector2::fromJson(json["col2"]);
    return result;
}

inline nlohmann::json Matrix2::toJson()
{
    nlohmann::json result;
    result["col1"] = data[0].toJson();
    result["col2"] = data[1].toJson();
    return result;
}

#pragma endregion Matrix2

#pragma region Matrix3

inline Matrix3::Matrix3(const Matrix2& a)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (i == 2 && j == 2)
            {
                data[i][j] = 1.f;
                continue;
            }
            if (i >= 2 || j >= 2)
            {
                data[i][j] = 0.f;
                continue;
            }
            data[i][j] = a.data[i][j];
        }
    }
}

inline Matrix3::Matrix3(const Matrix4& a)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            data[i][j] = a[i][j];
        }
    }
}

inline void Matrix3::Transpose()
{
    auto m = *this;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            data[j][i] = m.data[i][j];
        }
    }
}

inline Matrix3 Matrix3::transpose() const
{
    auto m = *this;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            m.data[j][i] = data[i][j];
        }
    }
    return m;
}

inline void Matrix3::Inverse()
{
    auto d = det3(*this);

    Vector2 v00[2] = {
       Vector2(data[1][1], data[1][2]),
       Vector2(data[2][1], data[2][2])
    };
    const Matrix2 a00(v00);
    Vector2 v01[2] = {
       Vector2(data[1][0], data[1][2]),
       Vector2(data[2][0], data[2][2])
    };
    const Matrix2 a01(v01);
    Vector2 v02[2] = {
       Vector2(data[1][0], data[1][1]),
       Vector2(data[2][0], data[2][1])
    };
    const Matrix2 a02(v02);
    Vector2 v10[2] = {
       Vector2(data[0][1], data[0][2]),
       Vector2(data[2][1], data[2][2])
    };
    const Matrix2 a10(v10);
    Vector2 v11[2] = {
       Vector2(data[0][0], data[0][2]),
       Vector2(data[2][0], data[2][2])
    };
    const Matrix2 a11(v11);
    Vector2 v12[2] = {
       Vector2(data[0][0], data[0][1]),
       Vector2(data[2][0], data[2][1])
    };
    const Matrix2 a12(v12);
    Vector2 v20[2] = {
       Vector2(data[0][1], data[0][2]),
       Vector2(data[1][1], data[1][2])
    };
    const Matrix2 a20(v20);
    Vector2 v21[2] = {
       Vector2(data[0][0], data[0][2]),
       Vector2(data[1][0], data[1][2])
    };
    const Matrix2 a21(v21);
    Vector2 v22[2] = {
       Vector2(data[0][0], data[0][1]),
       Vector2(data[1][0], data[1][1])
    };
    const Matrix2 a22(v22);

    data[0][0] = det2(a00)/d;
    data[0][1] = -det2(a10)/d;
    data[0][2] = det2(a20)/d;
    data[1][0] = -det2(a01)/d;
    data[1][1] = det2(a11)/d;
    data[1][2] = -det2(a21)/d;
    data[2][0] = det2(a02)/d;
    data[2][1] = -det2(a12)/d;
    data[2][2] = det2(a22)/d;
}

inline Matrix3 Matrix3::inverse() const
{
    auto m = Matrix3{};
    auto d = det3(*this);

    Vector2 v00[2] = {
        Vector2(data[1][1], data[1][2]),
        Vector2(data[2][1], data[2][2])
     };
    const Matrix2 a00(v00);
    Vector2 v01[2] = {
        Vector2(data[1][0], data[1][2]),
        Vector2(data[2][0], data[2][2])
     };
    const Matrix2 a01(v01);
    Vector2 v02[2] = {
        Vector2(data[1][0], data[1][1]),
        Vector2(data[2][0], data[2][1])
     };
    const Matrix2 a02(v02);
    Vector2 v10[2] = {
        Vector2(data[0][1], data[0][2]),
        Vector2(data[2][1], data[2][2])
     };
    const Matrix2 a10(v10);
    Vector2 v11[2] = {
        Vector2(data[0][0], data[0][2]),
        Vector2(data[2][0], data[2][2])
     };
    const Matrix2 a11(v11);
    Vector2 v12[2] = {
        Vector2(data[0][0], data[0][1]),
        Vector2(data[2][0], data[2][1])
     };
    const Matrix2 a12(v12);
    Vector2 v20[2] = {
        Vector2(data[0][1], data[0][2]),
        Vector2(data[1][1], data[1][2])
     };
    const Matrix2 a20(v20);
    Vector2 v21[2] = {
        Vector2(data[0][0], data[0][2]),
        Vector2(data[1][0], data[1][2])
     };
    const Matrix2 a21(v21);
    Vector2 v22[2] = {
        Vector2(data[0][0], data[0][1]),
        Vector2(data[1][0], data[1][1])
     };
    const Matrix2 a22(v22);

    m[0][0] = det2(a00)/d;
    m[0][1] = -det2(a10)/d;
    m[0][2] = det2(a20)/d;
    m[1][0] = -det2(a01)/d;
    m[1][1] = det2(a11)/d;
    m[1][2] = -det2(a21)/d;
    m[2][0] = det2(a02)/d;
    m[2][1] = -det2(a12)/d;
    m[2][2] = det2(a22)/d;

    return m;
}

inline Matrix3& Matrix3::operator=(const Matrix2& other)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (i == 2 && j == 2)
            {
                data[i][j] = 1.f;
                continue;
            }
            if (i >= 2 || j >= 2)
            {
                data[i][j] = 0.f;
                continue;
            }
            data[i][j] = other.data[i][j];
        }
    }
    return *this;
}

inline Matrix3& Matrix3::operator=(const Matrix4& other)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            data[i][j] = other.data[i][j];
        }
    }
    return *this;
}

inline Matrix3 Matrix3::fromJson(nlohmann::json json)
{
    Matrix3 m;
    m[0] = Vector3::fromJson(json["col1"]);
    m[1] = Vector3::fromJson(json["col2"]);
    m[2] = Vector3::fromJson(json["col3"]);
    return m;
}

inline nlohmann::json Matrix3::toJson()
{
    nlohmann::json j;
    j["col1"] = data[0].toJson();
    j["col2"] = data[1].toJson();
    j["col3"] = data[2].toJson();
    return j;
}

#pragma endregion Matrix3

#pragma region Matrix4

inline Matrix4::Matrix4(const Matrix2& a)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((i == 3 && j == 3) || (i == 2 && j == 2))
            {
                data[i][j] = 1.f;
                continue;
            }
            if (i >= 2 || j >= 2)
            {
                data[i][j] = 0.f;
                continue;
            }
            data[i][j] = a.data[i][j];
        }
    }
}

inline Matrix4::Matrix4(const Matrix3& a)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == 3 && j == 3)
            {
                data[i][j] = 1.f;
                continue;
            }
            if (i >= 3 || j >= 3)
            {
                data[i][j] = 0.f;
                continue;
            }
            data[i][j] = a.data[i][j];
        }
    }
}

inline void Matrix4::Transpose()
{
    auto m = *this;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            data[i][j] = m.data[j][i];
        }
    }
}

inline Matrix4 Matrix4::transpose() const
{
    auto m = *this;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m.data[i][j] = data[j][i];
        }
    }
    return m;
}

inline void Matrix4::Inverse()
{
    const auto d = det4(*this);

    Vector3 v00[3] = {
       Vector3(data[1][1], data[1][2], data[1][3]),
       Vector3(data[2][1], data[2][2], data[2][3]),
       Vector3(data[3][1], data[3][2], data[3][3])
    };
    const Matrix3 a00(v00);
    Vector3 v01[3] = {
       Vector3(data[1][0], data[1][2], data[1][3]),
       Vector3(data[2][0], data[2][2], data[2][3]),
       Vector3(data[3][0], data[3][2], data[3][3])
    };
    const Matrix3 a01(v01);
    Vector3 v02[3] = {
       Vector3(data[1][0], data[1][1], data[1][3]),
       Vector3(data[2][0], data[2][1], data[2][3]),
       Vector3(data[3][0], data[3][1], data[3][3])
    };
    const Matrix3 a02(v02);
    Vector3 v03[3] = {
       Vector3(data[1][0], data[1][1], data[1][2]),
       Vector3(data[2][0], data[2][1], data[2][2]),
       Vector3(data[3][0], data[3][1], data[3][2])
    };
    const Matrix3 a03(v03);
    Vector3 v10[3] = {
       Vector3(data[0][1], data[0][2], data[0][3]),
       Vector3(data[2][1], data[2][2], data[2][3]),
       Vector3(data[3][1], data[3][2], data[3][3])
    };
    const Matrix3 a10(v10);
    Vector3 v11[3] = {
       Vector3(data[0][0], data[0][2], data[0][3]),
       Vector3(data[2][0], data[2][2], data[2][3]),
       Vector3(data[3][0], data[3][2], data[3][3])
    };
    const Matrix3 a11(v11);
    Vector3 v12[3] = {
       Vector3(data[0][0], data[0][1], data[0][3]),
       Vector3(data[2][0], data[2][1], data[2][3]),
       Vector3(data[3][0], data[3][1], data[3][3])
    };
    const Matrix3 a12(v12);
    Vector3 v13[3] = {
       Vector3(data[0][0], data[0][1], data[0][2]),
       Vector3(data[2][0], data[2][1], data[2][2]),
       Vector3(data[3][0], data[3][1], data[3][2])
    };
    const Matrix3 a13(v13);
    Vector3 v20[3] = {
       Vector3(data[0][1], data[0][2], data[0][3]),
       Vector3(data[1][1], data[1][2], data[1][3]),
       Vector3(data[3][1], data[3][2], data[3][3])
    };
    const Matrix3 a20(v20);
    Vector3 v21[3] = {
       Vector3(data[0][0], data[0][2], data[0][3]),
       Vector3(data[1][0], data[1][2], data[1][3]),
       Vector3(data[3][0], data[3][2], data[3][3])
    };
    const Matrix3 a21(v21);
    Vector3 v22[3] = {
       Vector3(data[0][0], data[0][1], data[0][3]),
       Vector3(data[1][0], data[1][1], data[1][3]),
       Vector3(data[3][0], data[3][1], data[3][3])
    };
    const Matrix3 a22(v22);
    Vector3 v23[3] = {
       Vector3(data[0][0], data[0][1], data[0][2]),
       Vector3(data[1][0], data[1][1], data[1][2]),
       Vector3(data[3][0], data[3][1], data[3][2])
    };
    const Matrix3 a23(v23);
    Vector3 v30[3] = {
       Vector3(data[0][1], data[0][2], data[0][3]),
       Vector3(data[1][1], data[1][2], data[1][3]),
       Vector3(data[2][1], data[2][2], data[2][3])
    };
    const Matrix3 a30(v30);
    Vector3 v31[3] = {
       Vector3(data[0][0], data[0][2], data[0][3]),
       Vector3(data[1][0], data[1][2], data[1][3]),
       Vector3(data[2][0], data[2][2], data[2][3])
    };
    const Matrix3 a31(v31);
    Vector3 v32[3] = {
       Vector3(data[0][0], data[0][1], data[0][3]),
       Vector3(data[1][0], data[1][1], data[1][3]),
       Vector3(data[2][0], data[2][1], data[2][3])
    };
    const Matrix3 a32(v32);
    Vector3 v33[3] = {
       Vector3(data[0][0], data[0][1], data[0][2]),
       Vector3(data[1][0], data[1][1], data[1][2]),
       Vector3(data[2][0], data[2][1], data[2][2])
    };
    const Matrix3 a33(v33);

    data[0][0] = det3(a00)/d;
    data[0][1] = -det3(a10)/d;
    data[0][2] = det3(a20)/d;
    data[0][3] = -det3(a30)/d;
    data[1][0] = -det3(a01)/d;
    data[1][1] = det3(a11)/d;
    data[1][2] = -det3(a21)/d;
    data[1][3] = det3(a31)/d;
    data[2][0] = det3(a02)/d;
    data[2][1] = -det3(a12)/d;
    data[2][2] = det3(a22)/d;
    data[2][3] = -det3(a32)/d;
    data[3][0] = -det3(a03)/d;
    data[3][1] = det3(a13)/d;
    data[3][2] = -det3(a23)/d;
    data[3][3] = det3(a33)/d;
}

inline Matrix4 Matrix4::inverse() const
{
    auto m = Matrix4{};
    const auto d = det4(*this);

    Vector3 v00[3] = {
       Vector3(data[1][1], data[1][2], data[1][3]),
       Vector3(data[2][1], data[2][2], data[2][3]),
       Vector3(data[3][1], data[3][2], data[3][3])
    };
    const Matrix3 a00(v00);
    Vector3 v01[3] = {
       Vector3(data[1][0], data[1][2], data[1][3]),
       Vector3(data[2][0], data[2][2], data[2][3]),
       Vector3(data[3][0], data[3][2], data[3][3])
    };
    const Matrix3 a01(v01);
    Vector3 v02[3] = {
       Vector3(data[1][0], data[1][1], data[1][3]),
       Vector3(data[2][0], data[2][1], data[2][3]),
       Vector3(data[3][0], data[3][1], data[3][3])
    };
    const Matrix3 a02(v02);
    Vector3 v03[3] = {
       Vector3(data[1][0], data[1][1], data[1][2]),
       Vector3(data[2][0], data[2][1], data[2][2]),
       Vector3(data[3][0], data[3][1], data[3][2])
    };
    const Matrix3 a03(v03);
    Vector3 v10[3] = {
       Vector3(data[0][1], data[0][2], data[0][3]),
       Vector3(data[2][1], data[2][2], data[2][3]),
       Vector3(data[3][1], data[3][2], data[3][3])
    };
    const Matrix3 a10(v10);
    Vector3 v11[3] = {
       Vector3(data[0][0], data[0][2], data[0][3]),
       Vector3(data[2][0], data[2][2], data[2][3]),
       Vector3(data[3][0], data[3][2], data[3][3])
    };
    const Matrix3 a11(v11);
    Vector3 v12[3] = {
       Vector3(data[0][0], data[0][1], data[0][3]),
       Vector3(data[2][0], data[2][1], data[2][3]),
       Vector3(data[3][0], data[3][1], data[3][3])
    };
    const Matrix3 a12(v12);
    Vector3 v13[3] = {
       Vector3(data[0][0], data[0][1], data[0][2]),
       Vector3(data[2][0], data[2][1], data[2][2]),
       Vector3(data[3][0], data[3][1], data[3][2])
    };
    const Matrix3 a13(v13);
    Vector3 v20[3] = {
       Vector3(data[0][1], data[0][2], data[0][3]),
       Vector3(data[1][1], data[1][2], data[1][3]),
       Vector3(data[3][1], data[3][2], data[3][3])
    };
    const Matrix3 a20(v20);
    Vector3 v21[3] = {
       Vector3(data[0][0], data[0][2], data[0][3]),
       Vector3(data[1][0], data[1][2], data[1][3]),
       Vector3(data[3][0], data[3][2], data[3][3])
    };
    const Matrix3 a21(v21);
    Vector3 v22[3] = {
       Vector3(data[0][0], data[0][1], data[0][3]),
       Vector3(data[1][0], data[1][1], data[1][3]),
       Vector3(data[3][0], data[3][1], data[3][3])
    };
    const Matrix3 a22(v22);
    Vector3 v23[3] = {
       Vector3(data[0][0], data[0][1], data[0][2]),
       Vector3(data[1][0], data[1][1], data[1][2]),
       Vector3(data[3][0], data[3][1], data[3][2])
    };
    const Matrix3 a23(v23);
    Vector3 v30[3] = {
       Vector3(data[0][1], data[0][2], data[0][3]),
       Vector3(data[1][1], data[1][2], data[1][3]),
       Vector3(data[2][1], data[2][2], data[2][3])
    };
    const Matrix3 a30(v30);
    Vector3 v31[3] = {
       Vector3(data[0][0], data[0][2], data[0][3]),
       Vector3(data[1][0], data[1][2], data[1][3]),
       Vector3(data[2][0], data[2][2], data[2][3])
    };
    const Matrix3 a31(v31);
    Vector3 v32[3] = {
       Vector3(data[0][0], data[0][1], data[0][3]),
       Vector3(data[1][0], data[1][1], data[1][3]),
       Vector3(data[2][0], data[2][1], data[2][3])
    };
    const Matrix3 a32(v32);
    Vector3 v33[3] = {
       Vector3(data[0][0], data[0][1], data[0][2]),
       Vector3(data[1][0], data[1][1], data[1][2]),
       Vector3(data[2][0], data[2][1], data[2][2])
    };
    const Matrix3 a33(v33);

    m[0][0] = det3(a00)/d;
    m[0][1] = -det3(a10)/d;
    m[0][2] = det3(a20)/d;
    m[0][3] = -det3(a30)/d;
    m[1][0] = -det3(a01)/d;
    m[1][1] = det3(a11)/d;
    m[1][2] = -det3(a21)/d;
    m[1][3] = det3(a31)/d;
    m[2][0] = det3(a02)/d;
    m[2][1] = -det3(a12)/d;
    m[2][2] = det3(a22)/d;
    m[2][3] = -det3(a32)/d;
    m[3][0] = -det3(a03)/d;
    m[3][1] = det3(a13)/d;
    m[3][2] = -det3(a23)/d;
    m[3][3] = det3(a33)/d;

    return m;
}

inline Matrix4 Matrix4::Translate(const Matrix4& org, const Vector3& translation)
{
    Matrix4 t{};
    t[3][0] = translation.x();
    t[3][1] = translation.y();
    t[3][2] = translation.z();

    return org * t;
}

inline Matrix4 Matrix4::Rotate(const Matrix4& org, Vector3 axis, const float angle)
{
    Quaternion q = Quaternion(axis, angle);

    return org * q.toMatrix();
}

inline Matrix4 Matrix4::Rotate(const Matrix4& org, const Quaternion& rotation)
{
    return org * rotation.toMatrix();
}

inline Matrix4 Matrix4::Scale(const Matrix4& org, const Vector3& scale)
{
    Matrix4 s{};

    s[0][0] = scale.x();
    s[1][1] = scale.y();
    s[2][2] = scale.z();

    return org * s;
}

inline Matrix4 Matrix4::ModelMatrix(const Vector3& position, const Quaternion& rotation, const Vector3& scale)
{
    return Scale(Rotate(Translate({}, position), rotation), scale);
}

inline Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    const auto f = (target - eye).normalize();
    const auto r = Vector3::cross(f, up).normalize();
    const auto u = Vector3::cross(r, f).normalize();

    Vector4 v[4] {
        Vector4(r.x(), u.x(), -f.x(), 0.f),
        Vector4(r.y(), u.y(), -f.y(), 0.f),
        Vector4(r.z(), u.z(), -f.z(), 0.f),
        Vector4(-Vector3::dot(r, eye), -Vector3::dot(u, eye), Vector3::dot(f, eye), 1.f)
    };

    const Matrix4 m{v};
    return m;
}

inline Matrix4 Matrix4::PerspectiveMatrix(const float fov, const float aspect, const float near, const float far)
{
    Vector4 v[4] {
        Vector4(cot(fov / 2.0f) / aspect, 0, 0, 0),
        Vector4(0, cot(fov / 2.0f), 0, 0),
        Vector4(0, 0, (near + far) / (near - far), -1),
        Vector4(0, 0, (2 * near * far) / (near - far), 0)
    };
    Matrix4 m{v};
    return m;
}

inline Matrix4 Matrix4::OrthographicMatrix(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
    Vector4 v[4] {
        Vector4(2.f / (right - left),0.f,0.f,-(right + left) / (right - left)),
        Vector4(0.f,2.f / (top - bottom),0.f,-(top + bottom) / (top - bottom)),
        Vector4(0.f,0.f,-2.f / (far - near),-(far + near) / (far - near)),
        Vector4(0.f, 0.f, 0.f,1.0f)
    };
    const Matrix4 m(v);
    return m;
}

inline Matrix4& Matrix4::operator=(const Matrix2& other)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((i == 3 && j == 3) || (i == 2 && j == 2))
            {
                data[i][j] = 1.f;
                continue;
            }
            if (i >= 2 || j >= 2)
            {
                data[i][j] = 0.f;
                continue;
            }
            data[i][j] = other.data[i][j];
        }
    }
    return *this;
}

inline Matrix4& Matrix4::operator=(const Matrix3& other)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if ((i == 3 && j == 3))
            {
                data[i][j] = 1.f;
                continue;
            }
            if (i >= 3 || j >= 3)
            {
                data[i][j] = 0.f;
                continue;
            }
            data[i][j] = other.data[i][j];
        }
    }
    return *this;
}

inline Matrix4 Matrix4::fromJson(nlohmann::json json)
{
    Matrix4 m{};
    m[0] = Vector4::fromJson(json["col1"]);
    m[1] = Vector4::fromJson(json["col2"]);
    m[2] = Vector4::fromJson(json["col3"]);
    m[3] = Vector4::fromJson(json["col4"]);
    return m;
}

inline nlohmann::json Matrix4::toJson() const
{
    nlohmann::json j;
    j["col1"] = data[0].toJson();
    j["col2"] = data[1].toJson();
    j["col3"] = data[2].toJson();
    j["col4"] = data[3].toJson();
    return j;
}

#pragma endregion Matrix4

inline std::vector<Vector4> getFrustumCornersWorldSpace(const Matrix4& projectionMatrix, const Matrix4& viewMatrix)
{
    const auto inv = (projectionMatrix * viewMatrix).inverse();

    std::vector<Vector4> frustumCorners;
    for (int x = 0; x < 2; x++)
    {
        for (int y = 0; y < 2; y++)
        {
            for (int z = 0; z < 2; z++)
            {
                Vector4 pt =
                    inv * Vector4(
                      2.0f * x - 1.0f,
                      2.0f * y - 1.0f,
                      2.0f * z - 1.0f,
                        1.0f
                      );
                frustumCorners.push_back(pt / pt.w());
            }
        }
    }
    return frustumCorners;
}