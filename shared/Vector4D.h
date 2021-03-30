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

/**
*	Same data-layout as engine's vec4_t, which is a vec_t[4].
*/
class Vector4D final
{
public:
    Vector4D()
    {
        x = y = z = w = 0;
    }

    Vector4D(const float x, const float y, const float z, const float w): x(x), y(y), z(z), w(w)
    {
    }

    explicit Vector4D(float flScalar) : x(flScalar), y(flScalar), z(flScalar), w(flScalar)
    {
    }

    Vector4D(const Vector4D& other) = default;

    Vector4D(Vector4D&&) noexcept = default;

    Vector4D(const float rgfl[4])
    {
        x = rgfl[0];
        y = rgfl[1];
        z = rgfl[2];
        w = rgfl[3];
    }

    ~Vector4D() = default;
    Vector4D& operator=(const Vector4D& v) = default;
    Vector4D& operator=(Vector4D&& v) = default;

    // Operators

    Vector4D operator-() const
    {
        return Vector4D(-x, -y, -z, -w);
    }

    bool operator==(const Vector4D& other) const
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    bool operator!=(const Vector4D& other) const
    {
        return !(*this == other);
    }

    Vector4D operator+(const Vector4D& v) const
    {
        return Vector4D(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    Vector4D operator-(const Vector4D& v) const
    {
        return Vector4D(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    Vector4D operator*(const float fl) const
    {
        return Vector4D(x * fl, y * fl, z * fl, w * fl);
    }

    Vector4D operator/(const float fl) const
    {
        return Vector4D(x / fl, y / fl, z / fl, w / fl);
    }

    /// <summary>
    /// Vectors will now automatically convert to float * when needed
    /// </summary>
    operator float* ()
    {
        return &x;
    }

    /// <summary>
    /// Vectors will now automatically convert to float * when needed
    /// </summary>
    operator const float* () const
    {
        return &x;
    }

    float x, y, z, w;
};

inline Vector4D operator*(const float fl, const Vector4D& v)
{
    return v * fl;
}
