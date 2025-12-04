#pragma once

namespace Harmony::Utilities {
    class Transformable
    {
    public:
        Transformable();

        Transformable& translate(const Vector3& translation);
        Transformable& rotate(const Quaternion& rotation);
        Transformable& scale(const Vector3& scaleFactor);

        void setPosition(const Vector3& position);
        void setRotation(const Quaternion& rotation);
        void setScale(const Vector3& scale);
        void setOrigin(const Vector3& origin);

        const Vector3& getPosition() const;
        const Quaternion& getRotation() const;
        const Vector3& getScale() const;
        const Vector3& getOrigin() const;

        const Matrix& getMatrix() const;
        void updateMatrix() const;

        Vector3 getForward() const;
        Vector3 getUp() const;
        Vector3 getRight() const;

    private:
        Vector3 position_;
        Quaternion rotation_;
        Vector3 scale_;
        Vector3 origin_;

        mutable Matrix cachedMatrix_;
        mutable bool isDirty_;
    };
}
