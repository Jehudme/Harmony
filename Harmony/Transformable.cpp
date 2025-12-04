#include "pch.h"
#include "Transformable.h"
#include "raymath.h"
namespace Harmony::Utilities {

    Transformable::Transformable()
        : position_{ 0.0f, 0.0f, 0.0f }
        , rotation_{ 0.0f, 0.0f, 0.0f, 1.0f }
        , scale_{ 1.0f, 1.0f, 1.0f }  
        , origin_{ 0.0f, 0.0f, 0.0f }
        , cachedMatrix_(MatrixIdentity())
        , isDirty_(true)
    {
    }

	Transformable::~Transformable() = default;

    Transformable& Transformable::translate(const Vector3& translation)
    {
        position_ = Vector3Add(position_, translation);
        isDirty_ = true;
        return *this;
    }

    Transformable& Transformable::rotate(const Quaternion& rotation)
    {
        rotation_ = QuaternionMultiply(rotation_, rotation);
        rotation_ = QuaternionNormalize(rotation_);
        isDirty_ = true;
        return *this;
    }

    Transformable& Transformable::scale(const Vector3& scaleFactor)
    {
        scale_ = Vector3Multiply(scale_, scaleFactor);
        isDirty_ = true;
        return *this;
    }

    void Transformable::setPosition(const Vector3& position)
    {
        position_ = position;
        isDirty_ = true;
    }

    void Transformable::setRotation(const Quaternion& rotation)
    {
        rotation_ = rotation;
        isDirty_ = true;
    }

    void Transformable::setScale(const Vector3& scale)
    {
        scale_ = scale;
        isDirty_ = true;
    }

    void Transformable::setOrigin(const Vector3& origin)
    {
        origin_ = origin;
        isDirty_ = true;
    }

    const Vector3& Transformable::getPosition() const { return position_; }
    const Quaternion& Transformable::getRotation() const { return rotation_; }
    const Vector3& Transformable::getScale() const { return scale_; }
    const Vector3& Transformable::getOrigin() const { return origin_; }

    void Transformable::updateMatrix() const
    {
        Matrix matOrigin = MatrixTranslate(-origin_.x, -origin_.y, -origin_.z);
        Matrix matScale = MatrixScale(scale_.x, scale_.y, scale_.z);
        Matrix matRotation = QuaternionToMatrix(rotation_);
        Matrix matTranslation = MatrixTranslate(position_.x, position_.y, position_.z);
        Matrix temp = MatrixMultiply(matOrigin, matScale);
        
        temp = MatrixMultiply(temp, matRotation);
        cachedMatrix_ = MatrixMultiply(temp, matTranslation);

        isDirty_ = false;
    }

    const Matrix& Transformable::getMatrix() const
    {
        if (isDirty_) {
            updateMatrix();
        }
        return cachedMatrix_;
    }

    Vector3 Transformable::getForward() const {
        return Vector3RotateByQuaternion(Vector3(0.0f, 0.0f, 1.0f), rotation_);
    }

    Vector3 Transformable::getUp() const {
        return Vector3RotateByQuaternion(Vector3(0.0f, 1.0f, 0.0f), rotation_);
    }

    Vector3 Transformable::getRight() const {
        return Vector3RotateByQuaternion(Vector3(1.0f, 0.0f, 0.0f), rotation_);
    }

}
