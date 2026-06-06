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
    Vector2();
    explicit Vector2(int x, int y);
    explicit Vector2(float x, float y);

    float x{};
    float y{};

    [[nodiscard]] float magnitude() const;
    void Normalize();
    [[nodiscard]] Vector2 normalize() const;
    void translate(const Vector2 &v);

    static float dot(const Vector2 &a, const Vector2 &b);
    static Vector2 lerp(Vector2 a, Vector2 b, float t);

    static Vector2 fromJson(nlohmann::json json);
    nlohmann::json toJson();

};

class Vector3 {
public:
    Vector3();
    explicit Vector3(int x, int y, int z);
    explicit Vector3(float x, float y, float z);
    explicit Vector3(const Vector2 &v);
    explicit Vector3(const Vector2 &v, int z);
    explicit Vector3(const Vector2 &v, float z);
    explicit Vector3(int x, const Vector2 &v);
    explicit Vector3(float x, const Vector2 &v);
    explicit Vector3(const Vector4 &v);

    float x{};
    float y{};
    float z{};

    [[nodiscard]] float magnitude() const;
    void Normalize();
    [[nodiscard]] Vector3 normalize() const;
    void translate(const Vector3 &v);

    [[nodiscard]] static Vector3 cross(const Vector3 &a, const Vector3 &b);
    [[nodiscard]] static float dot(const Vector3 &a, const Vector3 &b);
    [[nodiscard]] static Vector3 lerp(const Vector3 &a, const Vector3 &b, float t);
    [[nodiscard]] static Vector3 forward();
    [[nodiscard]] static Vector3 right();
    [[nodiscard]] static Vector3 up();

    static Vector3 fromJson(nlohmann::json json);
    nlohmann::json toJson();
};

class Vector4 {
public:
    Vector4();
    explicit Vector4(int x, int y, int z, int w);
    explicit Vector4(float x, float y, float z, float w);
    explicit Vector4(const Vector3 &v);
    explicit Vector4(const Vector3 &v, int w);
    explicit Vector4(const Vector3 &v, float w);
    explicit Vector4(int x, const Vector3 &v);
    explicit Vector4(float x, const Vector3 &v);
    explicit Vector4(const Vector2 &v);
    explicit Vector4(const Vector2 &v, int z, int w);
    explicit Vector4(const Vector2 &v, float z, float w);
    explicit Vector4(int x, const Vector2 &v, int w);
    explicit Vector4(float x, const Vector2 &v, float w);
    explicit Vector4(int x, int y, const Vector2 &v);
    explicit Vector4(float x, float y, const Vector2 &v);

    float x{};
    float y{};
    float z{};
    float w{};

    [[nodiscard]] float magnitude() const;
    void Normalize();
    [[nodiscard]] Vector4 normalize() const;

    static float dot(const Vector4 &a, const Vector4 &b);
    static Vector4 lerp(const Vector4 &a, const Vector4 &b, float t);

    static Vector4 fromJson(nlohmann::json json);
    nlohmann::json toJson();
};

class Quaternion {
public:
    Quaternion();
    explicit Quaternion(int x, int y, int z, int w);
    explicit Quaternion(float x, float y, float z, float w);
    explicit Quaternion(const Vector3 &axis, float angle);
    explicit Quaternion(const Vector3 &EulerAngles);
    explicit Quaternion(const Matrix4 &m);

    float x{};
    float y{};
    float z{};
    float w{};

    [[nodiscard]] float magnitude() const;
    void Normalize();
    [[nodiscard]] Quaternion normalize() const;
    [[nodiscard]] Vector3 eulerAngles() const;
    [[nodiscard]] Matrix4 toMatrix() const;
    void rotate(const Vector3 &euler);

    static float dot(const Quaternion &a, const Quaternion &b);
    static Quaternion slerp(const Quaternion &a, const Quaternion &b, float t);

    static Quaternion fromJson(nlohmann::json json);
    nlohmann::json toJson();
};

class Matrix2 {
public:
    Matrix2();
    Matrix2(
        float a1, float a2,
        float b1, float b2
    );
    explicit Matrix2(const Matrix3 &a);
    explicit Matrix2(const Matrix4 &a);

    float a1{};
    float a2{};
    float b1{};
    float b2{};

    void Transpose();
    [[nodiscard]] Matrix2 transpose() const;

    void Inverse();
    [[nodiscard]] Matrix2 inverse() const;

    float* toFloatArray() {
        floatArray[0] = a1;
        floatArray[1] = b1;
        floatArray[2] = a2;
        floatArray[3] = b2;

        return floatArray;
    };

    static Matrix2 fromJson(nlohmann::json json);
    nlohmann::json toJson();

private:
    float floatArray[4] {};
};

class Matrix3 {
public:
    Matrix3();
    Matrix3(
        float a1, float a2, float a3,
        float b1, float b2, float b3,
        float c1, float c2, float c3
    );
    explicit Matrix3(const Matrix4 &a);

    float a1{};
    float a2{};
    float a3{};
    float b1{};
    float b2{};
    float b3{};
    float c1{};
    float c2{};
    float c3{};

    void Transpose();
    [[nodiscard]] Matrix3 transpose() const;

    void Inverse();
    [[nodiscard]] Matrix3 inverse() const;

    float* toFloatArray() {
        floatArray[0] = a1;
        floatArray[1] = b1;
        floatArray[2] = c1;
        floatArray[3] = a2;
        floatArray[4] = b2;
        floatArray[5] = c2;
        floatArray[6] = a3;
        floatArray[7] = b3;
        floatArray[8] = c3;

        return floatArray;
    };

    static Matrix3 fromJson(nlohmann::json json);
    nlohmann::json toJson();

private:
    float floatArray[9] {};
};

class Matrix4 {
public:
    Matrix4();
    Matrix4(
        float a1, float a2, float a3, float a4,
        float b1, float b2, float b3, float b4,
        float c1, float c2, float c3, float c4,
        float d1, float d2, float d3, float d4
    );
    explicit Matrix4(const Matrix3 &a);

    float a1{};
    float a2{};
    float a3{};
    float a4{};
    float b1{};
    float b2{};
    float b3{};
    float b4{};
    float c1{};
    float c2{};
    float c3{};
    float c4{};
    float d1{};
    float d2{};
    float d3{};
    float d4{};

    void Transpose();
    [[nodiscard]] Matrix4 transpose() const;

    void Inverse();
    [[nodiscard]] Matrix4 inverse() const;

    [[nodiscard]] static Matrix4 makeModelMatrix(const Vector3 &position, const Quaternion &rotation, const Vector3 &scale);
    [[nodiscard]] static Matrix4 lookAt(const Vector3 &eye, const Vector3 &target, const Vector3 &up);

    const float* toFloatArray() {
        floatArray[0]=a1; floatArray[1]=b1; floatArray[2]=c1; floatArray[3]=d1;
        floatArray[4]=a2; floatArray[5]=b2; floatArray[6]=c2; floatArray[7]=d2;
        floatArray[8]=a3; floatArray[9]=b3; floatArray[10]=c3; floatArray[11]=d3;
        floatArray[12]=a4; floatArray[13]=b4; floatArray[14]=c4; floatArray[15]=d4;
        return floatArray;
    }

    static Matrix4 fromJson(nlohmann::json json);
    nlohmann::json toJson();

private:
    float floatArray[16] {};
};

inline float det2(const Matrix2 &a) {
    return a.a1 * a.b2 - a.a2 * a.b1;
}

inline float det3(const Matrix3 &a) {
    return (
          a.a1 * a.b2 * a.c3
        + a.a2 * a.b3 * a.c1
        + a.a3 * a.b1 * a.c2
        - a.a3 * a.b2 * a.c1
        - a.a2 * a.b1 * a.c3
        - a.a1 * a.b3 * a.c2
    );
}

inline float det4(const Matrix4 &a) {

    const Matrix3 m0 = {
        a.b2, a.b3, a.b4,
        a.c2, a.c3, a.c4,
        a.d2, a.d3, a.d4,
    };

    const Matrix3 m1 = {
        a.b1, a.b3, a.b4,
        a.c1, a.c3, a.c4,
        a.d1, a.d3, a.d4
    };

    const Matrix3 m2 = {
        a.b1, a.b2, a.b4,
        a.c1, a.c2, a.c4,
        a.d1, a.d2, a.d4
    };

    const Matrix3 m3 = {
        a.b1, a.b2, a.b3,
        a.c1, a.c2, a.c3,
        a.d1, a.d2, a.d3
    };

    return a.a1 * det3(m0) - a.a2 * det3(m1) + a.a3 * det3(m2) - a.a4 * det3(m3);
}

#pragma region operations
inline  Vector2 operator+(const Vector2 a, const Vector2 b) {
    return Vector2(a.x + b.x, a.y + b.y);
}
inline Vector2 operator-(const Vector2 a, const Vector2 b) {
    return Vector2(a.x - b.x, a.y - b.y);
}
inline Vector2 operator*(const Vector2 a, const Vector2 b) {
    return Vector2(a.x * b.x, a.y * b.y);
}
inline Vector2 operator*(const int a, const Vector2 b) {
    return Vector2(a * b.x, a * b.y);
}
inline Vector2 operator*(const float a, const Vector2 b) {
    return Vector2(a * b.x, a * b.y);
}
inline Vector2 operator*(const Vector2 a, const int b) {
    return Vector2(b * a.x, b * a.y);
}
inline Vector2 operator*(const Vector2 a, const float b) {
    return Vector2(b * a.x, b * a.y);
}
inline Vector2 operator/(const Vector2 a, const Vector2 b) {
    return Vector2(a.x / b.x, a.y / b.y);
}
inline Vector2 operator/(const int a, const Vector2 b) {
    return Vector2(a / b.x, a / b.y);
}
inline Vector2 operator/(const float a, const Vector2 b) {
    return Vector2(a / b.x, a / b.y);
}
inline Vector2 operator/(const Vector2 a, const int b) {
    return Vector2(a.x / b, a.y / b);
}
inline Vector2 operator/(const Vector2 a, const float b) {
    return Vector2(a.x / b, a.y / b);
}
inline Vector2 operator*(const Matrix2 &a, const Vector2 b) {
    return Vector2(
        a.a1 * b.x + a.a2 * b.y,
        a.b1 * b.x + a.b2 * b.y
    );
}

inline  Vector3 operator+(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}
inline Vector3 operator-(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}
inline Vector3 operator*(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}
inline Vector3 operator*(const int a, const Vector3 &b) {
    return Vector3(a * b.x, a * b.y, a * b.z);
}
inline Vector3 operator*(const float a, const Vector3 &b) {
    return Vector3(a * b.x, a * b.y, a * b.z);
}
inline Vector3 operator*(const Vector3 &a, const int b) {
    return Vector3(b * a.x, b * a.y, b * a.z);
}
inline Vector3 operator*(const Vector3 &a, const float b) {
    return Vector3(b * a.x, b * a.y, b * a.z);
}
inline Vector3 operator/(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
}
inline Vector3 operator/(const int a, const Vector3 &b) {
    return Vector3(a / b.x, a / b.y, a / b.z);
}
inline Vector3 operator/(const float a, const Vector3 &b) {
    return Vector3(a / b.x, a / b.y, a / b.z);
}
inline Vector3 operator/(const Vector3 &a, const int b) {
    return Vector3(a.x / b, a.y / b, a.z / b);
}
inline Vector3 operator/(const Vector3 &a, const float b) {
    return Vector3(a.x / b, a.y / b, a.z / b);
}
inline Vector3 operator*(const Matrix3 &a, const Vector3 &b) {
    return Vector3(
        a.a1 * b.x + a.a2 * b.y + a.a3 * b.z,
        a.b1 * b.x + a.b2 * b.y + a.b3 * b.z,
        a.c1 * b.x + a.c2 * b.y + a.c3 * b.z
    );
}

inline  Vector4 operator+(const Vector4 &a, const Vector4 &b) {
    return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}
inline Vector4 operator-(const Vector4 &a, const Vector4 &b) {
    return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}
inline Vector4 operator*(const Vector4 &a, const Vector4 &b) {
    return Vector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}
inline Vector4 operator*(const int a, const Vector4 &b) {
    return Vector4(a * b.x, a * b.y, a * b.z, a * b.w);
}
inline Vector4 operator*(const float a, const Vector4 &b) {
    return Vector4(a * b.x, a * b.y, a * b.z, a * b.w);
}
inline Vector4 operator*(const Vector4 &a, const int b) {
    return Vector4(b * a.x, b * a.y, b * a.z, b * a.w);
}
inline Vector4 operator*(const Vector4 &a, const float b) {
    return Vector4(b * a.x, b * a.y, b * a.z, b * a.w);
}
inline Vector4 operator/(const Vector4 &a, const Vector4 &b) {
    return Vector4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}
inline Vector4 operator/(const int a, const Vector4 &b) {
    return Vector4(a / b.x, a / b.y, a / b.z, a / b.w);
}
inline Vector4 operator/(const float a, const Vector4 &b) {
    return Vector4(a / b.x, a / b.y, a / b.z, a / b.w);
}
inline Vector4 operator/(const Vector4 &a, const int b) {
    return Vector4(a.x / b, a.y / b, a.z / b, a.w / b);
}
inline Vector4 operator/(const Vector4 &a, const float b) {
    return Vector4(a.x / b, a.y / b, a.z / b, a.w / b);
}
inline Vector4 operator*(const Matrix4 &a, const Vector4 &b) {
    return Vector4(
        a.a1 * b.x + a.a2 * b.y + a.a3 * b.z + a.a4 * b.w,
        a.b1 * b.x + a.b2 * b.y + a.b3 * b.z + a.b4 * b.w,
        a.c1 * b.x + a.c2 * b.y + a.c3 * b.z + a.c4 * b.w,
        a.d1 * b.x + a.d2 * b.y + a.d3 * b.z + a.d4 * b.w
    );
}

inline  Quaternion operator+(const Quaternion &a, const Quaternion &b) {
    return Quaternion(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}
inline Quaternion operator-(const Quaternion &a, const Quaternion &b) {
    return Quaternion(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}
inline Quaternion operator*(const int a, const Quaternion &b) {
    return Quaternion(a * b.x, a * b.y, a * b.z, a * b.w);
}
inline Quaternion operator*(const float a, const Quaternion &b) {
    return Quaternion(a * b.x, a * b.y, a * b.z, a * b.w);
}
inline Quaternion operator*(const Quaternion &a, const int b) {
    return Quaternion(b * a.x, b * a.y, b * a.z, b * a.w);
}
inline Quaternion operator*(const Quaternion &a, const float b) {
    return Quaternion(b * a.x, b * a.y, b * a.z, b * a.w);
}
inline Quaternion operator/(const int a, const Quaternion &b) {
    return Quaternion(a / b.x, a / b.y, a / b.z, a / b.w);
}
inline Quaternion operator/(const float a, const Quaternion &b) {
    return Quaternion(a / b.x, a / b.y, a / b.z, a / b.w);
}
inline Quaternion operator/(const Quaternion &a, const int b) {
    return Quaternion(a.x / b, a.y / b, a.z / b, a.w / b);
}
inline Quaternion operator/(const Quaternion &a, const float b) {
    return Quaternion(a.x / b, a.y / b, a.z / b, a.w / b);
}

inline Matrix2 operator+(const Matrix2 &a, const Matrix2 &b) {
    return {
        a.a1 + b.a1, a.a2 + b.a2,
        a.b1 + b.b1, a.b2 + b.b2
    };
}
inline Matrix2 operator-(const Matrix2 &a, const Matrix2 &b) {
    return {
        a.a1 - b.a1, a.a2 - b.a2,
        a.b1 - b.b1, a.b2 - b.b2
    };
}
inline Matrix2 operator*(const Matrix2 &a, const Matrix2 &b) {
    return {
        a.a1 * b.a1 + a.a2 * b.b1, a.a1 * b.a2 + a.a2 * b.b2,
        a.b1 * b.a1 + a.b2 * b.b1, a.b1 * b.a2 + a.b2 * b.b2
    };
}
inline Matrix2 operator*(const Matrix2 &a, const int b) {
    return {
        a.a1 * b, a.a2 * b,
        a.b1 * b, a.a2 * b
    };
}
inline Matrix2 operator*(const Matrix2 &a, const float b) {
    return {
        a.a1 * b, a.a2 * b,
        a.b1 * b, a.a2 * b
    };
}
inline Matrix2 operator*(const int a, const Matrix2 &b) {
    return {
        a * b.a1, a * b.a2,
        a * b.b1, a * b.b2,
    };
}
inline Matrix2 operator*(const float a, const Matrix2 &b) {
    return {
        a * b.a1, a * b.a2,
        a * b.b1, a * b.b2,
    };
}

inline Matrix3 operator+(const Matrix3 &a, const Matrix3 &b) {
    return {
        a.a1 + b.a1, a.a2 + b.a2, a.a3 + b.a3,
        a.b1 + b.b1, a.b2 + b.b2, a.b3 + b.b3,
        a.c1 + a.c1, a.c2 + a.c2, a.c3 + b.c3
    };
}
inline Matrix3 operator-(const Matrix3 &a, const Matrix3 &b) {
    return {
        a.a1 - b.a1, a.a2 - b.a2, a.a3 - b.a3,
        a.b1 - b.b1, a.b2 - b.b2, a.b3 - b.b3,
        a.c1 - b.c1, a.c2 - b.c2, a.c3 - b.c3
    };
}
inline Matrix3 operator*(const Matrix3 &a, const Matrix3 &b) {
    return {
        (a.a1 * b.a1) + (a.a2 * b.b1) + (a.a3 * b.c1), (a.a1 * b.a2) + (a.a2 * b.b2) + (a.a3 * b.c2), (a.a1 * b.a3) + (a.a2 * b.b3) + (a.a3 * b.c3),
        (a.b1 * b.a1) + (a.b2 * b.b1) + (a.b3 * b.c1), (a.b1 * b.a2) + (a.b2 * b.b2) + (a.b3 * b.c2), (a.b1 * b.a3) + (a.b2 * b.b3) + (a.b3 * b.c3),
        (a.c1 * b.a1) + (a.c2 * b.b1) + (a.c3 * b.c1), (a.c1 * b.a2) + (a.c2 * b.b2) + (a.c3 * b.c2), (a.c1 * b.a3) + (a.c2 * b.b3) + (a.c3 * b.c3)
    };
}
inline Matrix3 operator*(const int &a, const Matrix3 &b) {
    return {
        a * b.a1, a * b.a2, a * b.a3,
        a * b.b1, a * b.b2, a * b.b3,
        a * b.c1, a * b.c2, a * b.c3
    };
}
inline Matrix3 operator*(const float &a, const Matrix3 &b) {
    return {
        a * b.a1, a * b.a2, a * b.a3,
        a * b.b1, a * b.b2, a * b.b3,
        a * b.c1, a * b.c2, a * b.c3
    };
}
inline Matrix3 operator*(const Matrix3 &a, const int &b) {
    return {
        b * a.a1, b * a.a2, b * a.a3,
        b * a.b1, b * a.b2, b * a.b3,
        b * a.c1, b * a.c2, b * a.c3
    };
}
inline Matrix3 operator*(const Matrix3 &a, const float &b) {
    return {
        b * a.a1, b * a.a2, b * a.a3,
        b * a.b1, b * a.b2, b * a.b3,
        b * a.c1, b * a.c2, b * a.c3
    };
}

inline Matrix4 operator+(const Matrix4 &a, const Matrix4 &b) {
    return {
        a.a1 + b.a1, a.a2 + b.a2, a.a3 + b.a3, a.a4 + b.a4,
        a.b1 + b.b1, a.b2 + b.b2, a.b3 + b.b3, a.b4 + b.b4,
        a.c1 + a.c1, a.c2 + a.c2, a.c3 + b.c3, a.c4 + b.c4,
        a.d1 + a.d1, a.d2 + a.d2, a.d3 + b.d3, a.d4 + b.d4
    };
}
inline Matrix4 operator-(const Matrix4 &a, const Matrix4 &b) {
    return {
        a.a1 - b.a1, a.a2 - b.a2, a.a3 - b.a3, a.a4 - b.a4,
        a.b1 - b.b1, a.b2 - b.b2, a.b3 - b.b3, a.b4 - b.b4,
        a.c1 - b.c1, a.c2 - b.c2, a.c3 - b.c3, a.c4 - b.c4,
        a.d1 - b.d1, a.d2 - b.d2, a.d3 - b.d3, a.d4 - b.d4
    };
}
inline Matrix4 operator*(const Matrix4 &a, const Matrix4 &b) {
    return {
        (a.a1 * b.a1) + (a.a2 * b.b1) + (a.a3 * b.c1) + (a.a4 * b.d1),
        (a.a1 * b.a2) + (a.a2 * b.b2) + (a.a3 * b.c2) + (a.a4 * b.d2),
        (a.a1 * b.a3) + (a.a2 * b.b3) + (a.a3 * b.c3) + (a.a4 * b.d3),
        (a.a1 * b.a4) + (a.a2 * b.b4) + (a.a3 * b.c4) + (a.a4 * b.d4),
        (a.b1 * b.a1) + (a.b2 * b.b1) + (a.b3 * b.c1) + (a.b4 * b.d1),
        (a.b1 * b.a2) + (a.b2 * b.b2) + (a.b3 * b.c2) + (a.b4 * b.d2),
        (a.b1 * b.a3) + (a.b2 * b.b3) + (a.b3 * b.c3) + (a.b4 * b.d3),
        (a.b1 * b.a4) + (a.b2 * b.b4) + (a.b3 * b.c4) + (a.b4 * b.d4),
        (a.c1 * b.a1) + (a.c2 * b.b1) + (a.c3 * b.c1) + (a.c4 * b.d1),
        (a.c1 * b.a2) + (a.c2 * b.b2) + (a.c3 * b.c2) + (a.c4 * b.d2),
        (a.c1 * b.a3) + (a.c2 * b.b3) + (a.c3 * b.c3) + (a.c4 * b.d3),
        (a.c1 * b.a4) + (a.c2 * b.b4) + (a.c3 * b.c4) + (a.c4 * b.d4),
        (a.d1 * b.a1) + (a.d2 * b.b1) + (a.d3 * b.c1) + (a.d4 * b.d1),
        (a.d1 * b.a2) + (a.d2 * b.b2) + (a.d3 * b.c2) + (a.d4 * b.d2),
        (a.d1 * b.a3) + (a.d2 * b.b3) + (a.d3 * b.c3) + (a.d4 * b.d3),
        (a.d1 * b.a4) + (a.d2 * b.b4) + (a.d3 * b.c4) + (a.d4 * b.d4),
    };
}
inline Matrix4 operator*(const int a, const Matrix4 &b) {
    return {
        a * b.a1, a * b.a2, a * b.a3, a * b.a4,
        a * b.b1, a * b.b2, a * b.b3, a * b.b4,
        a * b.c1, a * b.c2, a * b.c3, a * b.c4,
        a * b.d1, a * b.d2, a * b.d3, a * b.d4,
    };
}
inline Matrix4 operator*(const float a, const Matrix4 &b) {
    return {
        a * b.a1, a * b.a2, a * b.a3, a * b.a4,
        a * b.b1, a * b.b2, a * b.b3, a * b.b4,
        a * b.c1, a * b.c2, a * b.c3, a * b.c4,
        a * b.d1, a * b.d2, a * b.d3, a * b.d4,
    };
}
inline Matrix4 operator*(const Matrix4 &a, const int b) {
    return {
        b * a.a1, b * a.a2, b * a.a3, b * a.a4,
        b * a.b1, b * a.b2, b * a.b3, b * a.b4,
        b * a.c1, b * a.c2, b * a.c3, b * a.c4,
        b * a.d1, b * a.d2, b * a.d3, b * a.d4
    };
}
inline Matrix4 operator*(const Matrix4 &a, const float b) {
    return {
        b * a.a1, b * a.a2, b * a.a3, b * a.a4,
        b * a.b1, b * a.b2, b * a.b3, b * a.b4,
        b * a.c1, b * a.c2, b * a.c3, b * a.c4,
        b * a.d1, b * a.d2, b * a.d3, b * a.d4
    };
}
#pragma endregion

inline Vector2::Vector2() {
    x = 0.0;
    y = 0.0;
}

inline Vector2::Vector2(const int x, const int y) {
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
}

inline Vector2::Vector2(const float x, const float y) {
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
}


inline float Vector2::magnitude() const {
    return ::sqrt(x * x + y * y);
}

inline void Vector2::Normalize() {
    const float magnitude = this->magnitude();
    if (magnitude == 0)
        return;
    x /= magnitude;
    y /= magnitude;
}

inline Vector2 Vector2::normalize() const {
    const float magnitude = this->magnitude();
    if (magnitude == 0) {
        return {};
    }
    return Vector2(this->x / magnitude, this->y / magnitude);
}

inline void Vector2::translate(const Vector2 &v) {
    *this = *this + v;
}

inline float Vector2::dot(const Vector2 &a, const Vector2 &b) {
    return a.x * b.x + a.y * b.y;
}

inline Vector2 Vector2::lerp(const Vector2 a, const Vector2 b, const float t) {
    return  a * (1 - t) + (t) * b;
}

inline Vector2 Vector2::fromJson(nlohmann::json json)
{
    return Vector2(json["x"].get<float>(), json["y"].get<float>());
}

inline nlohmann::json Vector2::toJson()
{
    nlohmann::json j;

    j["x"] = this->x;
    j["y"] = this->y;
    return j;
}

inline Vector3::Vector3() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

inline Vector3::Vector3(const int x, const int y, const int z) {
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
    this->z = static_cast<float>(z);
}

inline Vector3::Vector3(const float x, const float y, const float z) {
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
    this->z = static_cast<float>(z);
}

inline Vector3::Vector3(const Vector2 &v) {
    x = v.x;
    y = v.y;
    z = 0.0;
}

inline Vector3::Vector3(const Vector2 &v, int z) {
    x = v.x;
    y = v.y;
    this->z = static_cast<float>(z);
}

inline Vector3::Vector3(const Vector2 &v, float z) {
    x = v.x;
    y = v.y;
    this->z = z;
}

inline Vector3::Vector3(int x, const Vector2 &v) {
    this->x = static_cast<float>(x);
    y = v.x;
    z = v.y;
}

inline Vector3::Vector3(float x, const Vector2 &v) {
    this->x = x;
    y = v.x;
    z = v.y;
}

inline Vector3::Vector3(const Vector4 &v) {
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

inline float Vector3::magnitude() const {
    return ::sqrt(x * x + y * y + z * z);
}

inline void Vector3::Normalize() {
    const float magnitude = this->magnitude();
    if (magnitude == 0)
        return;
    this->x /= magnitude;
    this->y /= magnitude;
    this->z /= magnitude;
}

inline Vector3 Vector3::cross(const Vector3 &a, const Vector3 &b) {
    return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline float Vector3::dot(const Vector3 &a, const Vector3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector3 Vector3::normalize() const {
    const float magnitude = this->magnitude();
    if (magnitude == 0) {
        return {};
    }
    return Vector3(this->x / magnitude, this->y / magnitude, this->z / magnitude);
}

inline Vector3 Vector3::lerp(const Vector3 &a, const Vector3 &b, const float t) {
    return  a * (1 - t) + (t) * b;
}

inline Vector3 Vector3::forward() {
    return Vector3(0, 0, -1);
}

inline Vector3 Vector3::right() {
    return Vector3(1, 0, 0);
}

inline Vector3 Vector3::up() {
    return Vector3(0, 1, 0);
}

inline Vector3 Vector3::fromJson(nlohmann::json json)
{
    return Vector3(json["x"].get<float>(), json["y"].get<float>(), json["z"].get<float>());
}

inline nlohmann::json Vector3::toJson()
{
    nlohmann::json j;

    j["x"] = this->x;
    j["y"] = this->y;
    j["z"] = this->z;
    return j;
}

inline void Vector3::translate(const Vector3 &v) {
    *this = *this + v;
}

inline Vector4::Vector4() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
    w = 1.0;
}

inline Vector4::Vector4(int x, int y, int z, int w) {
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
    this->z = static_cast<float>(z);
    this->w = static_cast<float>(w);
}

inline Vector4::Vector4(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

inline Vector4::Vector4(const Vector3 &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = 1.0;
}

inline Vector4::Vector4(const Vector3 &v, int w) {
    x = v.x;
    y = v.y;
    z = v.z;
    this->w = static_cast<float>(w);
}

inline Vector4::Vector4(const Vector3 &v, float w) {
    x = v.x;
    y = v.y;
    z = v.z;
    this->w = w;
}

inline Vector4::Vector4(int x, const Vector3 &v) {
    this->x = static_cast<float>(x);
    y = v.x;
    z = v.y;
    w = v.z;
}

inline Vector4::Vector4(float x, const Vector3 &v) {
    this->x = x;
    y = v.x;
    z = v.y;
    w = v.z;
}

inline Vector4::Vector4(const Vector2 &v) {
    x = v.x;
    y = v.y;
    z = 0.0;
    w = 1.0;
}

inline Vector4::Vector4(const Vector2 &v, int z, int w) {
    x = v.x;
    y = v.y;
    this->z = static_cast<float>(z);;
    this->w = static_cast<float>(w);;
}

inline Vector4::Vector4(const Vector2 &v, float z, float w) {
    x = v.x;
    y = v.y;
    this->z = z;
    this->w = w;
}

inline Vector4::Vector4(int x, const Vector2 &v, int w) {
    this->x = static_cast<float>(x);
    y = v.x;
    z = v.y;
    this->w = static_cast<float>(w);
}

inline Vector4::Vector4(float x, const Vector2 &v, float w) {
    this->x = x;
    y = v.x;
    z = v.y;
    this->w = w;
}

inline Vector4::Vector4(int x, int y, const Vector2 &v) {
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
    this->z = v.x;
    this->w = v.y;
}

inline Vector4::Vector4(float x, float y, const Vector2 &v) {
    this->x = x;
    this->y = y;
    this->z = v.x;
    this->w = v.y;
}

inline float Vector4::magnitude() const {
    return x*x + y*y + z*z + w*w;
}

inline void Vector4::Normalize() {
    *this = *this / magnitude();
}

inline Vector4 Vector4::normalize() const {
    return *this / magnitude();
}

inline float Vector4::dot(const Vector4 &a, const Vector4 &b) {
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

inline Vector4 Vector4::lerp(const Vector4 &a, const Vector4 &b, float t) {
    return (1-t)*a + t*b;
}

inline Vector4 Vector4::fromJson(nlohmann::json json)
{
    return Vector4(json["x"].get<float>(), json["y"].get<float>(), json["z"].get<float>(), json["w"].get<float>());
}

inline nlohmann::json Vector4::toJson()
{
    nlohmann::json j;

    j["x"] = x;
    j["y"] = y;
    j["z"] = z;
    j["w"] = w;
    return j;
}

inline Quaternion::Quaternion() {
    x = 0.0;
    y = 0.0;
    z = 0.0;
    w = 1.0;
}

inline Quaternion::Quaternion(int x, int y, int z, int w) {
    this->x = static_cast<float>(x);
    this->y = static_cast<float>(y);
    this->z = static_cast<float>(z);
    this->w = static_cast<float>(w);
}

inline Quaternion::Quaternion(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

inline Quaternion::Quaternion(const Vector3 &axis, float angle) {
    Vector3 u = axis.normalize();
    u = u * std::sin(angle / 2.0f);
    const float _w = cos(angle / 2.0f);

    x = u.x;
    y = u.y;
    z = u.z;
    this->w = _w;
}

inline Quaternion::Quaternion(const Vector3 &EulerAngles) {
    float phi = (PI / 180.0f) * EulerAngles.x;
    float theta = (PI / 180.0f) * EulerAngles.y;
    float psi = (PI / 180.0f) * EulerAngles.z;

    psi = psi / 2.0f;
    theta = theta / 2.0f;
    phi = phi / 2.0f;

    float sin_psi = sin(psi);
    float cos_psi = cos(psi);
    float sin_theta = sin(theta);
    float cos_theta = cos(theta);
    float sin_phi = sin(phi);
    float cos_phi = cos(phi);

    x = sin_phi*cos_theta*cos_psi - cos_phi*sin_theta*sin_psi;
    y = cos_phi*sin_theta*cos_psi + sin_phi*cos_theta*sin_psi;
    z = cos_phi*cos_theta*sin_psi - sin_phi*sin_theta*cos_psi;
    w = cos_phi*cos_theta*cos_psi + sin_phi*sin_theta*sin_psi;
}

inline Quaternion::Quaternion(const Matrix4 &m) {
    const float _w = std::sqrt(1.0f + m.a1 + m.b2 + m.c3) / 2.0f;
    const float _x = (m.c2 - m.b3) / (4.0f * _w);
    const float _y = (m.a3 - m.c1) / (4.0f * _w);
    const float _z = (m.b1 - m.a2) / (4.0f * _w);
    x = _x;
    y = _y;
    z = _z;
    w = _w;
}

inline void Quaternion::Normalize() {
    const float n = 1.0f/sqrt(x * x + y * y + z * z + w * w);

    x *= n;
    y *= n;
    z *= n;
    w *= n;
}

inline float Quaternion::magnitude() const {
    return x * x + y * y + z * z + w * w;
}

inline Vector3 Quaternion::eulerAngles() const {
    return Vector3(
      std::atan2(2 * (w * y + z * x), 1 - 2 * (y * y + z * z)),
      std::asin(2 * (w * y - z * x)),
      std::atan2(2 * (w * z + x * y), 1 - 2 * (x * x + y * y))
    );
}

inline Matrix4 Quaternion::toMatrix() const {
    Quaternion q = this->normalize();

    return {
        1.0f - 2.0f*q.y*q.y - 2.0f*q.z*q.z,     2.0f*q.x*q.y - 2.0f*q.z*q.w,        2.0f*q.x*q.z + 2.0f*q.y*q.w,        0.0f,
        2.0f*q.x*q.y + 2.0f*q.z*q.w,            1.0f - 2.0f*q.x*q.x - 2.0f*q.z*q.z, 2.0f*q.y*q.z - 2.0f*q.x*q.w,        0.0f,
        2.0f*q.x*q.z - 2.0f*q.y*q.w,            2.0f*q.y*q.z + 2.0f*q.x*q.w,        1.0f - 2.0f*q.x*q.x - 2.0f*q.y*q.y, 0.0f,
        0.0f,                                   0.0f,                               0.0f,                               1.0f
    };
}

inline void Quaternion::rotate(const Vector3 &euler) {
    *this = Quaternion(this->eulerAngles() + euler);
}

inline Quaternion Quaternion::normalize() const {
    const float n = 1.0f/sqrt(x * x + y * y + z * z + w * w);

    const float _x = x * n;
    const float _y = y * n;
    const float _z = z * n;
    const float _w = w * n;

    return Quaternion(_x, _y, _z, _w);
}

inline float Quaternion::dot(const Quaternion &a, const Quaternion &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline Quaternion Quaternion::slerp(const Quaternion &a, const Quaternion &b, const float t) {
    const float angle = ::acos(abs(dot(a, b)));

    if (angle == 0)
        return a;

    const float sin_t = ::sin(t * angle);
    const float sin_one_minus_t = ::sin((1.0f-t) * angle);
    const float sin_angle = ::sin(angle);

    return ((a * sin_one_minus_t) + b * sin_t) / sin_angle;
}

inline Quaternion Quaternion::fromJson(nlohmann::json json)
{
    return Quaternion(json["x"].get<float>(), json["y"].get<float>(), json["z"].get<float>(), json["w"].get<float>());
}

inline nlohmann::json Quaternion::toJson()
{
    nlohmann::json j;

    j["x"] = this->x;
    j["y"] = this->y;
    j["z"] = this->z;
    j["w"] = this->w;
    return j;
}

inline Matrix2::Matrix2() {
    a1 = 1.0; a2 = 0.0;
    b1 = 0.0; b2 = 1.0;
}

inline Matrix2::Matrix2(float a1, float a2, float b1, float b2) {
    this->a1 = a1; this->a2 = a2;
    this->b1 = b1; this->b2 = b2;
}

inline Matrix2::Matrix2(const Matrix3 &a) {
    a1 = a.a1; a2 = a.a2;
    b1 = a.b1; b2 = a.b2;
}

inline Matrix2::Matrix2(const Matrix4 &a) {
    a1 = a.a1; a2 = a.a2;
    b1 = a.b1; b2 = a.b2;
}

inline void Matrix2::Transpose() {
    float _a1 = a1; float _a2 = b1;
    float _b1 = a2; float _b2 = b2;

    a1 = _a1; a2 = _a2;
    b1 = _b1; b2 = _b2;
}

inline Matrix2 Matrix2::transpose() const {
    float _a1 = a1; float _a2 = b1;
    float _b1 = a2; float _b2 = b2;

    return {
        _a1, _a2,
        _b1, _b2
    };
}

inline void Matrix2::Inverse() {
    float d = det2(*this);
    *this = {
        b2 / d, -a2 / d,
        -b1 / d, a1 / d,
    };
}

inline Matrix2 Matrix2::inverse() const {
    float d = det2(*this);

    return {
        b2 / d, -a2 / d,
        -b1 / d, a1 / d,
    };
}

inline Matrix2 Matrix2::fromJson(nlohmann::json json)
{
    return {json["a1"].get<float>(), json["a2"].get<float>(),
     json["b1"].get<float>(), json["b2"].get<float>()};
}

inline nlohmann::json Matrix2::toJson()
{
    nlohmann::json j;

    j["a1"] = this->a1;
    j["a2"] = this->a2;
    j["b1"] = this->b1;
    j["b2"] = this->b2;
    return j;
}

inline Matrix3::Matrix3() {
    a1 = 1.0; a2 = 0.0; a3 = 0.0;
    b1 = 0.0; b2 = 1.0; b3 = 0.0;
    c1 = 0.0; c2 = 0.0; c3 = 1.0;
}

inline Matrix3::Matrix3(float a1, float a2, float a3, float b1, float b2, float b3, float c1, float c2,
    float c3) {
    this->a1 = a1; this->a2 = a2; this->a3 = a3;
    this->b1 = b1; this->b2 = b2; this->b3 = b3;
    this->c1 = c1; this->c2 = c2; this->c3 = c3;
}

inline Matrix3::Matrix3(const Matrix4 &a) {
    a1 = a.a1, a2 = a.a2, a3 = a.a3,
    b1 = a.b1, b2 = a.b2, b3 = a.b3,
    c1 = a.c1, c2 = a.c2, c3 = a.c3;
}


inline void Matrix3::Transpose() {
    const float _a1 = a1; const float _a2 = b1; const float _a3 = c1;
    const float _b1 = a2; const float _b2 = b2; const float _b3 = c2;
    const float _c1 = a3; const float _c2 = b3; const float _c3 = c3;

    a1 = _a1; a2 = _a2; a3 = _a3;
    b1 = _b1; b2 = _b2; b3 = _b3;
    c1 = _c1; c2 = _c2; c3 = _c3;
}

inline Matrix3 Matrix3::transpose() const {
    const float _a1 = a1; const float _a2 = b1; const float _a3 = c1;
    const float _b1 = a2; const float _b2 = b2; const float _b3 = c2;
    const float _c1 = a3; const float _c2 = b3; const float _c3 = c3;

    return {
        _a1, _a2, _a3,
        _b1, _b2, _b3,
        _c1, _c2, _c3
    };
}

inline void Matrix3::Inverse() {
    float d = det3(*this);

    Matrix2 a00 = {
        b2, b3,
        c2, c3
    };

    Matrix2 a01 = {
        b1, b3,
        c1, c3
    };

    Matrix2 a02 = {
        b1, b2,
        c1, c2
    };

    Matrix2 a10 = {
        a2, a3,
        c2, c3
    };

    Matrix2 a11 = {
        a1, a3,
        c1, c3
    };

    Matrix2 a12 = {
        a1, a2,
        c1, c2
    };

    Matrix2 a20 = {
        a2, a3,
        b2, b3
    };

    Matrix2 a21 = {
        a1, a3,
        b1, b3
    };

    Matrix2 a22 = {
        a1, a2,
        b1, b2
    };

    *this = {
        det2(a00)/d, -det2(a01)/d, det2(a02)/d,
        -det2(a10)/d, det2(a11)/d, -det2(a12)/d,
        det2(a20)/d, -det2(a21)/d, det2(a22)/d
    };

}

inline Matrix3 Matrix3::inverse() const {
    float d = det3(*this);

    Matrix2 a00 = {
        b2, b3,
        c2, c3
    };

    Matrix2 a01 = {
        b1, b3,
        c1, c3
    };

    Matrix2 a02 = {
        b1, b2,
        c1, c2
    };

    Matrix2 a10 = {
        a2, a3,
        c2, c3
    };

    Matrix2 a11 = {
        a1, a3,
        c1, c3
    };

    Matrix2 a12 = {
        a1, a2,
        c1, c2
    };

    Matrix2 a20 = {
        a2, a3,
        b2, b3
    };

    Matrix2 a21 = {
        a1, a3,
        b1, b3
    };

    Matrix2 a22 = {
        a1, a2,
        b1, b2
    };

    return {
        det2(a00)/d, -det2(a01)/d, det2(a02)/d,
        -det2(a10)/d, det2(a11)/d, -det2(a12)/d,
        det2(a20)/d, -det2(a21)/d, det2(a22)/d
    };
}

inline Matrix3 Matrix3::fromJson(nlohmann::json json)
{
    return {json["a1"].get<float>(), json["a2"].get<float>(), json["a3"].get<float>(),
    json["b1"].get<float>(), json["b2"].get<float>(), json["b3"].get<float>(),
    json["c1"].get<float>(), json["c2"].get<float>(), json["c3"].get<float>()};
}

inline nlohmann::json Matrix3::toJson()
{
    nlohmann::json json;

    json["a1"] = this->a1;
    json["a2"] = this->a2;
    json["a3"] = this->a3;
    json["b1"] = this->b1;
    json["b2"] = this->b2;
    json["b3"] = this->b3;
    json["c1"] = this->c1;
    json["c2"] = this->c2;
    json["c3"] = this->c3;
    return json;
}

inline Matrix4::Matrix4() {
    a1 = 1.0; a2 = 0.0; a3 = 0.0; a4 = 0.0;
    b1 = 0.0; b2 = 1.0; b3 = 0.0; b4 = 0.0;
    c1 = 0.0; c2 = 0.0; c3 = 1.0; c4 = 0.0;
    d1 = 0.0; d2 = 0.0; d3 = 0.0; d4 = 1.0;
}

inline Matrix4::Matrix4(float a1, float a2, float a3, float a4, float b1, float b2, float b3, float b4,
    float c1, float c2, float c3, float c4, float d1, float d2, float d3, float d4)
{
    this->a1 = a1; this->a2 = a2; this->a3 = a3; this->a4 = a4;
    this->b1 = b1; this->b2 = b2; this->b3 = b3; this->b4 = b4;
    this->c1 = c1; this->c2 = c2; this->c3 = c3; this->c4 = c4;
    this->d1 = d1; this->d2 = d2; this->d3 = d3; this->d4 = d4;
}

inline Matrix4::Matrix4(const Matrix3 &a) {
    a1 = a.a1; a2 = a.a2; a3 = a.a3; a4 = 0.0;
    b1 = a.b1; b2 = a.b2; b3 = a.b3; b4 = 0.0;
    c1 = a.c1; c2 = a.c2; c3 = a.c3; c4 = 0.0;
    d1 = 0.0; d2 = 0.0; d3 = 0.0; d4 = 1.0;
}

inline void Matrix4::Transpose() {
    const float _a1 = a1; const float _a2 = b1; const float _a3 = c1; const float _a4 = d1;
    const float _b1 = a2; const float _b2 = b2; const float _b3 = c2; const float _b4 = d2;
    const float _c1 = a3; const float _c2 = b3; const float _c3 = c3; const float _c4 = d3;
    const float _d1 = a4; const float _d2 = b4; const float _d3 = c4; const float _d4 = d4;

    a1 = _a1; a2 = _a2; a3 = _a3; a4 = _a4;
    b1 = _b1; b2 = _b2; b3 = _b3; b4 = _b4;
    c1 = _c1; c2 = _c2; c3 = _c3; c4 = _c4;
    d1 = _d1; d2 = _d2; d3 = _d3; d4 = _d4;
}

inline Matrix4 Matrix4::transpose() const {
    return {
        a1, b1, c1, d1,
        a2, b2, c2, d2,
        a3, b3, c3, d3,
        a4, b4, c4, d4,
    };
}

inline void Matrix4::Inverse() {
    float d = det4(*this);

    Matrix3 a00 = {
        b2, b3, b4,
        c2, c3, c4,
        d2, d3, d4
    };
    Matrix3 a01 = {
        b1, b3, b4,
        c1, c3, c4,
        d1, d3, d4
    };
    Matrix3 a02 = {
        b1, b2, b4,
        c1, c2, c4,
        d1, d2, d4
    };
    Matrix3 a03 = {
        b1, b2, b3,
        c1, c2, c3,
        d1, d2, d3
    };
    Matrix3 a10 = {
        a2, a3, a4,
        c2, c3, c4,
        d2, d3, d4
    };
    Matrix3 a11 = {
        a1, a3, a4,
        c1, c3, c4,
        d1, d3, d4
    };
    Matrix3 a12 = {
        a1, a2, a4,
        c1, c2, c4,
        d1, d2, d4
    };
    Matrix3 a13 = {
        a1, a2, a3,
        c1, c2, c3,
        d1, d2, d3
    };
    Matrix3 a20 = {
        a2, a3, a4,
        b2, b3, b4,
        d2, d3, d4
    };
    Matrix3 a21 = {
        a1, a3, a4,
        b1, b3, b4,
        d1, d3, d4
    };
    Matrix3 a22 = {
        a1, a2, a4,
        b1, b2, b4,
        d1, d2, d4
    };
    Matrix3 a23 = {
        a1, a2, a3,
        b1, b2, b3,
        d1, d2, d3
    };

    Matrix3 a30 = {
        a2, a3, a4,
        b2, b3, b4,
        c2, c3, c4
    };
    Matrix3 a31 = {
        a1, a3, a4,
        b1, b3, b4,
        c1, c3, c4
    };
    Matrix3 a32 = {
        a1, a2, a4,
        b1, b2, b4,
        c1, c2, c4
    };
    Matrix3 a33 = {
        a1, a2, a3,
        b1, b2, b3,
        c1, c2, c3
    };

    float _a1 = det3(a00)/d;
    float _a2 = -det3(a10)/d;
    float _a3 = det3(a20)/d;
    float _a4 = -det3(a30)/d;
    float _b1 = -det3(a01)/d;
    float _b2 = det3(a11)/d;
    float _b3 = -det3(a21)/d;
    float _b4 = det3(a31)/d;
    float _c1 = det3(a02)/d;
    float _c2 = -det3(a12)/d;
    float _c3 = det3(a22)/d;
    float _c4 = -det3(a32)/d;
    float _d1 = -det3(a03)/d;
    float _d2 = det3(a13)/d;
    float _d3 = -det3(a23)/d;
    float _d4 = det3(a33)/d;

    *this = {
        _a1, _a2, _a3, _a4,
        _b1, _b2, _b3, _b4,
        _c1, _c2, _c3, _c4,
        _d1, _d2, _d3, _d4
    };
}

inline Matrix4 Matrix4::inverse() const {
    float d = det4(*this);

    Matrix3 a00 = {
        b2, b3, b4,
        c2, c3, c4,
        d2, d3, d4
    };
    Matrix3 a01 = {
        b1, b3, b4,
        c1, c3, c4,
        d1, d3, d4
    };
    Matrix3 a02 = {
        b1, b2, b4,
        c1, c2, c4,
        d1, d2, d4
    };
    Matrix3 a03 = {
        b1, b2, b3,
        c1, c2, c3,
        d1, d2, d3
    };
    Matrix3 a10 = {
        a2, a3, a4,
        c2, c3, c4,
        d2, d3, d4
    };
    Matrix3 a11 = {
        a1, a3, a4,
        c1, c3, c4,
        d1, d3, d4
    };
    Matrix3 a12 = {
        a1, a2, a4,
        c1, c2, c4,
        d1, d2, d4
    };
    Matrix3 a13 = {
        a1, a2, a3,
        c1, c2, c3,
        d1, d2, d3
    };
    Matrix3 a20 = {
        a2, a3, a4,
        b2, b3, b4,
        d2, d3, d4
    };
    Matrix3 a21 = {
        a1, a3, a4,
        b1, b3, b4,
        d1, d3, d4
    };
    Matrix3 a22 = {
        a1, a2, a4,
        b1, b2, b4,
        d1, d2, d4
    };
    Matrix3 a23 = {
        a1, a2, a3,
        b1, b2, b3,
        d1, d2, d3
    };

    Matrix3 a30 = {
        a2, a3, a4,
        b2, b3, b4,
        c2, c3, c4
    };
    Matrix3 a31 = {
        a1, a3, a4,
        b1, b3, b4,
        c1, c3, c4
    };
    Matrix3 a32 = {
        a1, a2, a4,
        b1, b2, b4,
        c1, c2, c4
    };
    Matrix3 a33 = {
        a1, a2, a3,
        b1, b2, b3,
        c1, c2, c3
    };

    float _a1 = det3(a00)/d;
    float _a2 = -det3(a10)/d;
    float _a3 = det3(a20)/d;
    float _a4 = -det3(a30)/d;
    float _b1 = -det3(a01)/d;
    float _b2 = det3(a11)/d;
    float _b3 = -det3(a21)/d;
    float _b4 = det3(a31)/d;
    float _c1 = det3(a02)/d;
    float _c2 = -det3(a12)/d;
    float _c3 = det3(a22)/d;
    float _c4 = -det3(a32)/d;
    float _d1 = -det3(a03)/d;
    float _d2 = det3(a13)/d;
    float _d3 = -det3(a23)/d;
    float _d4 = det3(a33)/d;

    return {
        _a1, _a2, _a3, _a4,
        _b1, _b2, _b3, _b4,
        _c1, _c2, _c3, _c4,
        _d1, _d2, _d3, _d4
    };
}

inline Matrix4 Matrix4::makeModelMatrix(const Vector3 &position, const Quaternion &rotation, const Vector3 &scale) {
    const Matrix4 t = {
        1, 0, 0, position.x,
        0, 1, 0, position.y,
        0, 0, 1, position.z,
        0, 0, 0, 1,
    };
    const Matrix4 r = rotation.toMatrix();
    const Matrix4 s = {
        scale.x, 0, 0, 0,
        0, scale.y, 0, 0,
        0, 0, scale.z, 0,
        0, 0, 0, 1
    };
    return t * r * s;
}

inline Matrix4 Matrix4::lookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    const Vector3 f = (target - eye).normalize();
    const Vector3 r = Vector3::cross(f, up).normalize();
    const Vector3 u = Vector3::cross(r, f);

    return {
        r.x,  r.y,  r.z, -Vector3::dot(r, eye),
        u.x,  u.y,  u.z, -Vector3::dot(u, eye),
       -f.x, -f.y, -f.z,  Vector3::dot(f, eye),
        0.0,  0.0,  0.0,  1.0
   };
}

inline Matrix4 Matrix4::fromJson(nlohmann::json json)
{
    return {json["a1"].get<float>(), json["a2"].get<float>(), json["a3"].get<float>(), json["a4"].get<float>(),
    json["b1"].get<float>(), json["b2"].get<float>(), json["b3"].get<float>(), json["b4"].get<float>(),
    json["c1"].get<float>(), json["c2"].get<float>(), json["c3"].get<float>(), json["c4"].get<float>(),
    json["d1"].get<float>(), json["d2"].get<float>(), json["d3"].get<float>(), json["d4"].get<float>()};
}

inline nlohmann::json Matrix4::toJson()
{
    nlohmann::json j;

    j["a1"] = this->a1;
    j["a2"] = this->a2;
    j["a3"] = this->a3;
    j["a4"] = this->a4;
    j["b1"] = this->b1;
    j["b2"] = this->b2;
    j["b3"] = this->b3;
    j["b4"] = this->b4;
    j["c1"] = this->c1;
    j["c2"] = this->c2;
    j["c3"] = this->c3;
    j["c4"] = this->c4;
    j["d1"] = this->d1;
    j["d2"] = this->d2;
    j["d3"] = this->d3;
    j["d4"] = this->d4;
    return j;
}
