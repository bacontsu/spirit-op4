/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#pragma once

#include "math.h"
#include "types.h"

/// <summary>
/// 2DVector - used for many pathfinding and many other 
/// operations that are treated as planar rather than 3d. 
/// </summary>
class Vector2D
{
public:
    Vector2D()
    {
        x = y = 0;
    }

    Vector2D(float X, float Y)
    {
        x = X;
        y = Y;
    }

    Vector2D operator+(const Vector2D& v) const { return Vector2D(x + v.x, y + v.y); }
    Vector2D operator-(const Vector2D& v) const { return Vector2D(x - v.x, y - v.y); }
    Vector2D operator*(const float fl) const { return Vector2D(x * fl, y * fl); }
    Vector2D operator/(const float fl) const { return Vector2D(x / fl, y / fl); }

    float Length() const
    {
        const auto xx = static_cast<double>(x);
        const auto yy = static_cast<double>(y);
        return static_cast<float>(sqrt(xx * xx + yy * yy));
    }

    Vector2D Normalize() const
    {
        Vector2D vec2;

        auto flLen = Length();
        if (flLen == 0)
        {
            return Vector2D(0.0f, 0.0f);
        }
        else
        {
            flLen = 1.0f / flLen;
            return Vector2D(x * flLen, y * flLen);
        }
    }

    static float DotProduct(const Vector2D& a, const Vector2D& b) { return a.x * b.x + a.y * b.y; }

    vec_t x, y;
};

inline Vector2D operator*(const float fl, const Vector2D& v)
{
    return v * fl;
}
