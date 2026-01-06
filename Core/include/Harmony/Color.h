#pragma once

#include <shared_mutex>

namespace Harmony
{
    // ========================================================
    // RGBA Color Class
    // ========================================================
    
    class Color
    {
    public:
        Color(int color);
        Color(unsigned char red = 255, unsigned char green = 255, unsigned char blue = 255, unsigned char alpha = 255);

        int GetInteger() const;
        void SetInteger(int color);

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    // ========================================================
    // Color Comparison Operators
    // ========================================================
    
    bool operator==(const Color& left, const Color& right);
    bool operator!=(const Color& left, const Color& right);

    // ========================================================
    // Color Arithmetic Operators
    // ========================================================
    
    Color operator+(const Color& left, const Color& right);
    Color operator-(const Color& left, const Color& right);
    Color operator*(const Color& left, const Color& right);

    // ========================================================
    // Color Compound Assignment Operators
    // ========================================================
    
    Color& operator+=(Color& left, const Color& right);
    Color& operator-=(Color& left, const Color& right);
    Color& operator*=(Color& left, const Color& right);

} // namespace Harmony