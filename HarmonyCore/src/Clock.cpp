#include "Harmony/Utilities/Color.h"
#include "Harmony/Exceptions.h"
#include "Harmony/Logger.h"
#include "Harmony/Assert.h"

namespace Harmony {

    Color::Color(int color) : 
        r(static_cast<unsigned char>((color >> 24) & 0xFF)),
        g(static_cast<unsigned char>((color >> 16) & 0xFF)),
        b(static_cast<unsigned char>((color >> 8) & 0xFF)),
        a(static_cast<unsigned char>(color & 0xFF)) {
        HARMONY_TRACE("Color created from integer: 0x{:08X}", color);
    }

    Color::Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
        : r(red), g(green), b(blue), a(alpha) {
        // Note: unsigned char is already constrained to 0-255, so no validation needed
        HARMONY_TRACE("Color created: RGBA({}, {}, {}, {})", r, g, b, a);
    }

    bool operator==(const Color& left, const Color& right) {
        return left.r == right.r &&
            left.g == right.g &&
            left.b == right.b &&
            left.a == right.a;
    }

    bool operator!=(const Color& left, const Color& right) {
        return !(left == right);
    }

    Color operator+(const Color& left, const Color& right) {
        return Color(
            static_cast<unsigned char>((left.r + right.r > 255) ? 255 : left.r + right.r),
            static_cast<unsigned char>((left.g + right.g > 255) ? 255 : left.g + right.g),
            static_cast<unsigned char>((left.b + right.b > 255) ? 255 : left.b + right.b),
            static_cast<unsigned char>((left.a + right.a > 255) ? 255 : left.a + right.a)
        );
    }

    Color operator-(const Color& left, const Color& right) {
        return Color(
            static_cast<unsigned char>((left.r - right.r < 0) ? 0 : left.r - right.r),
            static_cast<unsigned char>((left.g - right.g < 0) ? 0 : left.g - right.g),
            static_cast<unsigned char>((left.b - right.b < 0) ? 0 : left.b - right.b),
            static_cast<unsigned char>((left.a - right.a < 0) ? 0 : left.a - right.a)
        );
    }

    Color operator*(const Color& left, const Color& right) {
        return Color(
            static_cast<unsigned char>((left.r * right.r) / 255),
            static_cast<unsigned char>((left.g * right.g) / 255),
            static_cast<unsigned char>((left.b * right.b) / 255),
            static_cast<unsigned char>((left.a * right.a) / 255)
        );
    }

    Color& operator+=(Color& left, const Color& right) {
        left = left + right;
        return left;
    }

    Color& operator-=(Color& left, const Color& right) {
        left = left - right;
        return left;
    }

    Color& operator*=(Color& left, const Color& right) {
        left = left * right;
        return left;
    }
}