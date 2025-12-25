#pragma once

#include <cstdint>

namespace Harmony
{
    // ========================================================
    // Time Value with High Precision
    // ========================================================
    
    /// @brief Represents a time value with high precision
    /// @details Similar to sf::Time but as a native Harmony utility.
    /// Supports construction from and conversion to various time units.
    class Time
    {
    public:
        /// @brief Default constructor - creates a zero time value
        Time();

        /// @brief Get the time value in seconds
        /// @return Time in seconds as a float
        float AsSeconds() const;

        /// @brief Get the time value in milliseconds
        /// @return Time in milliseconds as an int32_t
        int32_t AsMilliseconds() const;

        /// @brief Get the time value in microseconds
        /// @return Time in microseconds as an int64_t
        int64_t AsMicroseconds() const;

        /// @brief Create a Time value from seconds
        /// @param seconds The time value in seconds
        /// @return Time object representing the given seconds
        static Time FromSeconds(float seconds);

        /// @brief Create a Time value from milliseconds
        /// @param milliseconds The time value in milliseconds
        /// @return Time object representing the given milliseconds
        static Time FromMilliseconds(int32_t milliseconds);

        /// @brief Create a Time value from microseconds
        /// @param microseconds The time value in microseconds
        /// @return Time object representing the given microseconds
        static Time FromMicroseconds(int64_t microseconds);

        /// @brief Zero time constant
        static const Time Zero;

    private:
        /// @brief Private constructor from microseconds
        /// @param microseconds Time value in microseconds
        explicit Time(int64_t microseconds);

    private:
        int64_t microseconds_;
    };

    // ========================================================
    // Time Comparison Operators
    // ========================================================
    
    bool operator==(Time left, Time right);
    bool operator!=(Time left, Time right);
    bool operator<(Time left, Time right);
    bool operator>(Time left, Time right);
    bool operator<=(Time left, Time right);
    bool operator>=(Time left, Time right);

    // ========================================================
    // Time Arithmetic Operators
    // ========================================================
    
    Time operator-(Time right);
    Time operator+(Time left, Time right);
    Time operator-(Time left, Time right);
    Time operator*(Time left, float right);
    Time operator*(Time left, int64_t right);
    Time operator*(float left, Time right);
    Time operator*(int64_t left, Time right);
    Time operator/(Time left, float right);
    Time operator/(Time left, int64_t right);
    float operator/(Time left, Time right);

    // ========================================================
    // Time Compound Assignment Operators
    // ========================================================
    
    Time& operator+=(Time& left, Time right);
    Time& operator-=(Time& left, Time right);
    Time& operator*=(Time& left, float right);
    Time& operator*=(Time& left, int64_t right);
    Time& operator/=(Time& left, float right);
    Time& operator/=(Time& left, int64_t right);

} // namespace Harmony