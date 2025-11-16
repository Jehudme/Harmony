#pragma once
#include <AccCtrl.h>

namespace Harmony::Utilities
{
    class Color
    {
    public:
        Color();
        Color(int color);
        Color(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha = 255);

        int toInteger() const;

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

    bool operator ==(const Color& left, const Color& right);
    bool operator !=(const Color& left, const Color& right);

    Color operator +(const Color& left, const Color& right);
    Color operator -(const Color& left, const Color& right);
    Color operator *(const Color& left, const Color& right);

    Color& operator +=(Color& left, const Color& right);
    Color& operator -=(Color& left, const Color& right);
    Color& operator *=(Color& left, const Color& right);
}