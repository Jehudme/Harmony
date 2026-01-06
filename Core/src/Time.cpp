#include "Harmony/Time.h"
#include "Harmony/Assert.h"

namespace Harmony
{
// ========================================================
// Time Constants
// ========================================================

const Time Time::Zero = Time(0);

// ========================================================
// Time Constructors
// ========================================================

Time::Time() : microseconds_(0) {}

Time::Time(int64_t microseconds) : microseconds_(microseconds) {}

// ========================================================
// Time Conversion Functions
// ========================================================

float Time::AsSeconds() const
{
    return microseconds_ / 1000000.0f;
}

int32_t Time::AsMilliseconds() const
{
    return static_cast<int32_t>(microseconds_ / 1000);
}

int64_t Time::AsMicroseconds() const
{
    return microseconds_;
}

Time Time::FromSeconds(float seconds)
{
    return Time(static_cast<int64_t>(seconds * 1000000.0f));
}

Time Time::FromMilliseconds(int32_t milliseconds)
{
    return Time(static_cast<int64_t>(milliseconds) * 1000);
}

Time Time::FromMicroseconds(int64_t microseconds)
{
    return Time(microseconds);
}

// ========================================================
// Time Comparison Operators
// ========================================================

bool operator==(Time left, Time right)
{
    return left.AsMicroseconds() == right.AsMicroseconds();
}

bool operator!=(Time left, Time right)
{
    return left.AsMicroseconds() != right.AsMicroseconds();
}

bool operator<(Time left, Time right)
{
    return left.AsMicroseconds() < right.AsMicroseconds();
}

bool operator>(Time left, Time right)
{
    return left.AsMicroseconds() > right.AsMicroseconds();
}

bool operator<=(Time left, Time right)
{
    return left.AsMicroseconds() <= right.AsMicroseconds();
}

bool operator>=(Time left, Time right)
{
    return left.AsMicroseconds() >= right.AsMicroseconds();
}

// ========================================================
// Time Arithmetic Operators
// ========================================================

Time operator-(Time right)
{
    return Time::FromMicroseconds(-right.AsMicroseconds());
}

Time operator+(Time left, Time right)
{
    return Time::FromMicroseconds(left.AsMicroseconds() + right.AsMicroseconds());
}

Time operator-(Time left, Time right)
{
    return Time::FromMicroseconds(left.AsMicroseconds() - right.AsMicroseconds());
}

Time operator*(Time left, float right)
{
    return Time::FromSeconds(left.AsSeconds() * right);
}

Time operator*(Time left, int64_t right)
{
    return Time::FromMicroseconds(left.AsMicroseconds() * right);
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
    if (right == 0.0f)
    {
        HARMONY_ERROR("Time division by zero detected, returning Time::Zero");
        return Time::Zero;
    }
    return Time::FromSeconds(left.AsSeconds() / right);
}

Time operator/(Time left, int64_t right)
{
    HARMONY_ASSERT_WARN(right != 0, "Division by zero in Time operation");
    if (right == 0)
    {
        HARMONY_ERROR("Time division by zero detected, returning Time::Zero");
        return Time::Zero;
    }
    return Time::FromMicroseconds(left.AsMicroseconds() / right);
}

float operator/(Time left, Time right)
{
    HARMONY_ASSERT_WARN(right != Time::Zero, "Division by zero Time");
    if (right == Time::Zero)
    {
        HARMONY_ERROR("Time division by Time::Zero detected, returning 0.0f");
        return 0.0f;
    }
    return left.AsSeconds() / right.AsSeconds();
}

// ========================================================
// Time Compound Assignment Operators
// ========================================================

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
    if (right == 0.0f)
    {
        HARMONY_ERROR("Time division by zero detected, setting to Time::Zero");
        left = Time::Zero;
        return left;
    }
    return left = left / right;
}

Time& operator/=(Time& left, int64_t right)
{
    HARMONY_ASSERT_WARN(right != 0, "Division by zero in Time operation");
    if (right == 0)
    {
        HARMONY_ERROR("Time division by zero detected, setting to Time::Zero");
        left = Time::Zero;
        return left;
    }
    return left = left / right;
}

} // namespace Harmony