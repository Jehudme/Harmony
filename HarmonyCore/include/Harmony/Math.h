#pragma once

// 1. Include GLM Core and Extensions
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>       
#include <glm/gtx/quaternion.hpp>     
#include <glm/gtx/matrix_decompose.hpp> 

#include <limits>

// 2. Create Aliases
namespace Harmony {
    using Vector2 = glm::vec2;
    using Vector3 = glm::vec3;
    using Vector4 = glm::vec4;
    using Matrix3 = glm::mat3;
    using Matrix4 = glm::mat4;
    using Quaternion = glm::quat;
    using Color = glm::vec4;

    // 3. Static Math Helper Class
    struct Math {
        // --- Constants ---
        static constexpr float PI = 3.14159265358979323846f;
        static constexpr float Epsilon = std::numeric_limits<float>::epsilon();

        // --- Directions (Right-Handed / OpenGL Standard) ---
        static constexpr Vector3 Forward() { return Vector3(0.0f, 0.0f, -1.0f); }
        static constexpr Vector3 Backward() { return Vector3(0.0f, 0.0f, 1.0f); }
        static constexpr Vector3 Up() { return Vector3(0.0f, 1.0f, 0.0f); }
        static constexpr Vector3 Down() { return Vector3(0.0f, -1.0f, 0.0f); }
        static constexpr Vector3 Right() { return Vector3(1.0f, 0.0f, 0.0f); }
        static constexpr Vector3 Left() { return Vector3(-1.0f, 0.0f, 0.0f); }
        static constexpr Vector3 Zero() { return Vector3(0.0f); }
        static constexpr Vector3 One() { return Vector3(1.0f); }

        // --- Colors ---
        static constexpr Color White() { return Color(1.0f); }
        static constexpr Color Black() { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
        static constexpr Color Red() { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
        static constexpr Color Green() { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
        static constexpr Color Blue() { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
        static constexpr Color Clear() { return Color(0.0f); } // Transparent

        // --- Conversions ---
        static float Radians(float degrees) { return glm::radians(degrees); }
        static float Degrees(float radians) { return glm::degrees(radians); }

        // --- Vector Math ---
        static float Dot(const Vector3& a, const Vector3& b) { return glm::dot(a, b); }
        static Vector3 Cross(const Vector3& a, const Vector3& b) { return glm::cross(a, b); }
        static Vector3 Normalize(const Vector3& v) { return glm::normalize(v); }
        static float Length(const Vector3& v) { return glm::length(v); }
        static float LengthSquared(const Vector3& v) { return glm::dot(v, v); }
        static float Distance(const Vector3& a, const Vector3& b) { return glm::distance(a, b); }

        // Linear Interpolation
        template<typename T>
        static T Lerp(const T& a, const T& b, float t) { return glm::mix(a, b, t); }

        // --- Matrix Transformations ---
        static Matrix4 Identity() { return Matrix4(1.0f); }

        static Matrix4 Translate(const Vector3& translation) {
            return glm::translate(Matrix4(1.0f), translation);
        }

        static Matrix4 Rotate(float angleRadians, const Vector3& axis) {
            return glm::rotate(Matrix4(1.0f), angleRadians, axis);
        }

        static Matrix4 Scale(const Vector3& scale) {
            return glm::scale(Matrix4(1.0f), scale);
        }

        static Matrix4 Inverse(const Matrix4& m) { return glm::inverse(m); }
        static Matrix4 Transpose(const Matrix4& m) { return glm::transpose(m); }

        // Create a full Transform Matrix (Translation * Rotation * Scale)
        static Matrix4 Compose(const Vector3& pos, const Quaternion& rot, const Vector3& scale) {
            Matrix4 m = Translate(pos);
            m *= glm::toMat4(rot);
            m = glm::scale(m, scale);
            return m;
        }

        // Extract Position, Rotation, Scale from a Matrix
        static bool Decompose(const Matrix4& transform, Vector3& outTranslation, Quaternion& outRotation, Vector3& outScale) {
            Vector3 skew; Vector4 perspective;
            return glm::decompose(transform, outScale, outRotation, outTranslation, skew, perspective);
        }

        // --- Camera Math ---
        static Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& up = Up()) {
            return glm::lookAt(eye, target, up);
        }

        static Matrix4 Perspective(float fovRadians, float aspectRatio, float nearClip, float farClip) {
            return glm::perspective(fovRadians, aspectRatio, nearClip, farClip);
        }

        static Matrix4 Orthographic(float left, float right, float bottom, float top, float nearClip, float farClip) {
            return glm::ortho(left, right, bottom, top, nearClip, farClip);
        }

        // --- Quaternion Math ---
        static Quaternion Euler(const Vector3& eulerAnglesRadians) {
            return Quaternion(eulerAnglesRadians);
        }

        static Vector3 ToEulerAngles(const Quaternion& q) {
            return glm::eulerAngles(q);
        }

        static Quaternion Slerp(const Quaternion& a, const Quaternion& b, float t) {
            return glm::slerp(a, b, t);
        }

        static Quaternion AngleAxis(float angleRadians, const Vector3& axis) {
            return glm::angleAxis(angleRadians, axis);
        }

        // --- Utilities ---
        template<typename T>
        static T Clamp(T value, T min, T max) { return glm::clamp(value, min, max); }

        template<typename T>
        static T Min(T a, T b) { return glm::min(a, b); }

        template<typename T>
        static T Max(T a, T b) { return glm::max(a, b); }

        // Pointer access for shaders or Raylib interop
        static const float* ValuePtr(const Matrix4& m) { return glm::value_ptr(m); }
        static const float* ValuePtr(const Vector3& v) { return glm::value_ptr(v); }
    };
}