#pragma once
#include <AccCtrl.h>

namespace Harmony::Utilities
{
    class Color
    {
    public:
        Color(int color);
        Color(unsigned char red = 255, unsigned char green = 255, unsigned char blue = 255, unsigned char alpha = 255);

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