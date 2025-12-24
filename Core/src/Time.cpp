#include "Harmony/Utilities/Time.h"

namespace Harmony
{
    // Zero time constant definition
    const Time Time::Zero = Time(0);

    Time::Time()
        : microseconds_(0)
    {
    }

    Time::Time(int64_t microseconds)
        : microseconds_(microseconds)
    {
    }

    float Time::asSeconds() const
    {
        return microseconds_ / 1000000.0f;
    }

    int32_t Time::asMilliseconds() const
    {
        return static_cast<int32_t>(microseconds_ / 1000);
    }

    int64_t Time::asMicroseconds() const
    {
        return microseconds_;
    }

    Time Time::fromSeconds(float seconds)
    {
        return Time(static_cast<int64_t>(seconds * 1000000.0f));
    }

    Time Time::fromMilliseconds(int32_t milliseconds)
    {
        return Time(static_cast<int64_t>(milliseconds) * 1000);
    }

    Time Time::fromMicroseconds(int64_t microseconds)
    {
        return Time(microseconds);
    }

    // Comparison operators
    bool operator==(Time left, Time right)
    {
        return left.asMicroseconds() == right.asMicroseconds();
    }

    bool operator!=(Time left, Time right)
    {
        return left.asMicroseconds() != right.asMicroseconds();
    }

    bool operator<(Time left, Time right)
    {
        return left.asMicroseconds() < right.asMicroseconds();
    }

    bool operator>(Time left, Time right)
    {
        return left.asMicroseconds() > right.asMicroseconds();
    }

    bool operator<=(Time left, Time right)
    {
        return left.asMicroseconds() <= right.asMicroseconds();
    }

    bool operator>=(Time left, Time right)
    {
        return left.asMicroseconds() >= right.asMicroseconds();
    }

    // Arithmetic operators
    Time operator-(Time right)
    {
        return Time::fromMicroseconds(-right.asMicroseconds());
    }

    Time operator+(Time left, Time right)
    {
        return Time::fromMicroseconds(left.asMicroseconds() + right.asMicroseconds());
    }

    Time operator-(Time left, Time right)
    {
        return Time::fromMicroseconds(left.asMicroseconds() - right.asMicroseconds());
    }

    Time operator*(Time left, float right)
    {
        return Time::fromSeconds(left.asSeconds() * right);
    }

    Time operator*(Time left, int64_t right)
    {
        return Time::fromMicroseconds(left.asMicroseconds() * right);
    }

    Time operator*(float left, Time right)
    {
        return right * left;
    }

    Time operator*(int64_t left, Time right)
    {
        return right * left;
    }

    Time operator/(Time left, float right)
    {
        HARMONY_ASSERT_WARN(right != 0.0f, "Division by zero in Time operation");
        if (right == 0.0f) return Time::Zero;
        return Time::fromSeconds(left.asSeconds() / right);
    }

    Time operator/(Time left, int64_t right)
    {
        HARMONY_ASSERT_WARN(right != 0, "Division by zero in Time operation");
        if (right == 0) return Time::Zero;
        return Time::fromMicroseconds(left.asMicroseconds() / right);
    }

    float operator/(Time left, Time right)
    {
        HARMONY_ASSERT_WARN(right != Time::Zero, "Division by zero Time");
        if (right == Time::Zero) return 0.0f;
        return left.asSeconds() / right.asSeconds();
    }

    // Compound assignment operators
    Time& operator+=(Time& left, Time right)
    {
        return left = left + right;
    }

    Time& operator-=(Time& left, Time right)
    {
        return left = left - right;
    }

    Time& operator*=(Time& left, float right)
    {
        return left = left * right;
    }

    Time& operator*=(Time& left, int64_t right)
    {
        return left = left * right;
    }

    Time& operator/=(Time& left, float right)
    {
        HARMONY_ASSERT_WARN(right != 0.0f, "Division by zero in Time operation");
        if (right == 0.0f) {
            left = Time::Zero;
            return left;
        }
        return left = left / right;
    }

    Time& operator/=(Time& left, int64_t right)
    {
        HARMONY_ASSERT_WARN(right != 0, "Division by zero in Time operation");
        if (right == 0) {
            left = Time::Zero;
            return left;
        }
        return left = left / right;
    }

} // namespace Harmony