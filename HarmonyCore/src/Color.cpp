#include "Harmony/Utilities/Color.h"
#include "Harmony/Assert.h"
#include "Harmony/Logger.h"

namespace Harmony
{
// ========================================================
// Color Constructors
// ========================================================

Color::Color(int color)
    : r(static_cast<unsigned char>((color >> 24) & 0xFF)),
      g(static_cast<unsigned char>((color >> 16) & 0xFF)),
      b(static_cast<unsigned char>((color >> 8) & 0xFF)),
      a(static_cast<unsigned char>(color & 0xFF))
{
    HARMONY_TRACE("Color created from integer: 0x{:08X}", color);
}

Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
    : r(red), g(green), b(blue), a(alpha)
{
    HARMONY_TRACE("Color created: RGBA({}, {}, {}, {})", r, g, b, a);
}

// ========================================================
// Color Comparison Operators
// ========================================================

bool operator==(const Color& left, const Color& right)
{
    return left.r == right.r && left.g == right.g && left.b == right.b && left.a == right.a;
}

bool operator!=(const Color& left, const Color& right)
{
    return !(left == right);
}

// ========================================================
// Color Arithmetic Operators
// ========================================================

Color operator+(const Color& left, const Color& right)
{
    int sumRed = static_cast<int>(left.r) + static_cast<int>(right.r);
    int sumGreen = static_cast<int>(left.g) + static_cast<int>(right.g);
    int sumBlue = static_cast<int>(left.b) + static_cast<int>(right.b);
    int sumAlpha = static_cast<int>(left.a) + static_cast<int>(right.a);

    unsigned char newRed = (sumRed > 255) ? 255 : static_cast<unsigned char>(sumRed);
    unsigned char newGreen = (sumGreen > 255) ? 255 : static_cast<unsigned char>(sumGreen);
    unsigned char newBlue = (sumBlue > 255) ? 255 : static_cast<unsigned char>(sumBlue);
    unsigned char newAlpha = (sumAlpha > 255) ? 255 : static_cast<unsigned char>(sumAlpha);

    return Color(newRed, newGreen, newBlue, newAlpha);
}

Color operator-(const Color& left, const Color& right)
{
    unsigned char newRed = (left.r < right.r) ? 0 : left.r - right.r;
    unsigned char newGreen = (left.g < right.g) ? 0 : left.g - right.g;
    unsigned char newBlue = (left.b < right.b) ? 0 : left.b - right.b;
    unsigned char newAlpha = (left.a < right.a) ? 0 : left.a - right.a;

    return Color(newRed, newGreen, newBlue, newAlpha);
}

Color operator*(const Color& left, const Color& right)
{
    unsigned char newRed = static_cast<unsigned char>((left.r * right.r) / 255);
    unsigned char newGreen = static_cast<unsigned char>((left.g * right.g) / 255);
    unsigned char newBlue = static_cast<unsigned char>((left.b * right.b) / 255);
    unsigned char newAlpha = static_cast<unsigned char>((left.a * right.a) / 255);

    return Color(newRed, newGreen, newBlue, newAlpha);
}

// ========================================================
// Color Compound Assignment Operators
// ========================================================

Color& operator+=(Color& left, const Color& right)
{
    left = left + right;
    return left;
}

Color& operator-=(Color& left, const Color& right)
{
    left = left - right;
    return left;
}

Color& operator*=(Color& left, const Color& right)
{
    left = left * right;
    return left;
}

} // namespace Harmony