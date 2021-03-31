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

#include <cmath>

#include "mathdefs.h"
#include "types.h"
#include "Vector2D.h"

/**
* 3D Vector - same data-layout as engine's vec3_t, which is a vec_t[3]
*/
class Vector
{
public:
    // Construction/destruction
    constexpr Vector() : x{}, y{}, z{}
    {
    }

    constexpr Vector(const float X, const float Y, const float Z) : x(X), y(Y), z(Z)
    {
    }

    constexpr Vector(const float rgfl[3]) : x(rgfl[0]), y(rgfl[1]), z(rgfl[2])
    {
    }

    // Initializes all members with flScalar. Explicit to prevent implicit conversion issues.
    explicit Vector(vec_t flScalar) : x(flScalar), y(flScalar), z(flScalar)
    {
    }

    constexpr Vector(const Vector& v) = default;
    Vector(Vector&&) noexcept = default;

    ~Vector() = default;
    Vector& operator=(const Vector& v) = default;
    Vector& operator=(Vector&& v) = default;

    // Operators

    Vector operator-() const
    {
        return Vector(-x, -y, -z);
    }

    int operator==(const Vector& v) const
    {
        return x == v.x && y == v.y && z == v.z;
    }

    int operator!=(const Vector& v) const
    {
        return !(*this == v);
    }

    Vector operator+(const Vector& v) const
    {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vector operator-(const Vector& v) const
    {
        return Vector(x - v.x, y - v.y, z - v.z);
    }

    Vector operator*(const float fl) const
    {
        return Vector(x * fl, y * fl, z * fl);
    }

    Vector operator/(const float fl) const
    {
        return Vector(x / fl, y / fl, z / fl);
    }

    /** Vectors will now automatically convert to float * when needed */
    operator float*()
    {
        return &x;
    }

    /** Vectors will now automatically convert to float * when needed */
    operator const float*() const
    {
        return &x;
    }

    // Mutating methods

    /**
    * Normalizes the vector's components. Mutates this object.
    * @return Old length
    */
    vec_t NormalizeInPlace()
    {
        const auto flLen = Length();

        if (flLen == 0)
        {
            x = y = 0;
            z = 1;
            return flLen;
        }

        const auto flInvertedLen = 1 / flLen;

        x *= flInvertedLen;
        y *= flInvertedLen;
        z *= flInvertedLen;

        return flLen;
    }

    /**
     * Resets this vector to 0,0,0. Mutates this object.
     */
    void Clear()
    {
        x = y = z = 0;
    }

    /**
     * Copies the values of the provided vector into this one. Mutates this object.
     * @param v The values to set this vector to
     */
    void Set(const Vector& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }

    // Const methods

    void CopyToArray(float* rgfl) const
    {
        rgfl[0] = x;
        rgfl[1] = y;
        rgfl[2] = z;
    }

    float Length() const
    {
        const auto xx = static_cast<double>(x);
        const auto yy = static_cast<double>(y);
        const auto zz = static_cast<double>(z);
        return static_cast<float>(sqrt(xx * xx + yy * yy + zz * zz));
    }

    float Length2D() const
    {
        const auto xx = static_cast<double>(x);
        const auto yy = static_cast<double>(y);
        return static_cast<float>(sqrt(xx * xx + yy * yy));
    }

    Vector Normalize() const
    {
        auto flLen = Length();
        if (flLen == 0) return Vector(0, 0, 1);
        flLen = 1 / flLen;
        return Vector(x * flLen, y * flLen, z * flLen);
    }

    Vector2D Make2D() const
    {
        return Vector2D(x, y);
    }

    float Dot(const Vector& b) const
    {
        return x * b.x + y * b.y + z * b.z;
    }

    Vector Cross(const Vector& b) const
    {
        return Vector(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
    }

    /**
     * Apply a matrix transformation to this vector
     * @param matrix The transormation matrix
     * @return Transformed vector
     */
    Vector Transform(const float matrix[3][4]) const
    {
        return Vector(
            Dot(matrix[0]) + matrix[0][3],
            Dot(matrix[1]) + matrix[1][3],
            Dot(matrix[2]) + matrix[2][3]
        );
    }

    // Euler angles

    /**
     * Normalises the angles in this vector to lie between -180 and +180
     * @return Normalised angle vector
     */
    Vector NormalizeAngles() const
    {
        auto angles = *this;
        for (auto i = 0; i < 3; i++)
        {
            if (angles[i] > 180.0f) angles[i] -= 360.0f;
            else if (angles[i] < -180.0f) angles[i] += 360.0f;
        }
        return Vector(angles);
    }

    /**
     * Convert Euler angles into directional forward, right, and up vectors
     * @param forward The vector to put the forward result into
     * @param right The vector to put the right result into
     * @param up The vector to put the up result into
     */
    void AngleVectors(Vector* forward, Vector* right, Vector* up) const
    {
        const auto& angles = *this;

        float angle = angles[YAW] * (M_PI * 2 / 360);
        const auto sy = sin(angle);
        const auto cy = cos(angle);
        angle = angles[PITCH] * (M_PI * 2 / 360);
        const auto sp = sin(angle);
        const auto cp = cos(angle);
        angle = angles[ROLL] * (M_PI * 2 / 360);
        const auto sr = sin(angle);
        const auto cr = cos(angle);

        if (forward)
        {
            forward->x = cp * cy;
            forward->y = cp * sy;
            forward->z = -sp;
        }
        if (right)
        {
            right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
            right->y = (-1 * sr * sp * sy + -1 * cr * cy);
            right->z = -1 * sr * cp;
        }
        if (up)
        {
            up->x = (cr * sp * cy + -sr * -sy);
            up->y = (cr * sp * sy + -sr * cy);
            up->z = cr * cp;
        }
    }

    void AngleVectorsTranspose(Vector* forward, Vector* right, Vector* up) const
    {
        auto angles = *this;

        float angle = angles[YAW] * (M_PI * 2 / 360);
        const auto sy = sin(angle);
        const auto cy = cos(angle);
        angle = angles[PITCH] * (M_PI * 2 / 360);
        const auto sp = sin(angle);
        const auto cp = cos(angle);
        angle = angles[ROLL] * (M_PI * 2 / 360);
        const auto sr = sin(angle);
        const auto cr = cos(angle);

        if (forward)
        {
            forward->x = cp * cy;
            forward->y = (sr * sp * cy + cr * -sy);
            forward->z = (cr * sp * cy + -sr * -sy);
        }
        if (right)
        {
            right->x = cp * sy;
            right->y = (sr * sp * sy + cr * cy);
            right->z = (cr * sp * sy + -sr * cy);
        }
        if (up)
        {
            up->x = -sp;
            up->y = sr * cp;
            up->z = cr * cp;
        }
    }

    /**
     * Convert Euler angles into a rotation matrix
     * @param matrix The array to put the result into
     */
    void AngleMatrix(float matrix[3][4]) const
    {
        auto angles = *this;

        auto angle = static_cast<double>(angles[YAW]) * (M_PI * 2 / 360);
        const auto sy = sin(angle);
        const auto cy = cos(angle);
        angle = static_cast<double>(angles[PITCH]) * (M_PI * 2 / 360);
        const auto sp = sin(angle);
        const auto cp = cos(angle);
        angle = static_cast<double>(angles[ROLL]) * (M_PI * 2 / 360);
        const auto sr = sin(angle);
        const auto cr = cos(angle);

        // matrix = (YAW * PITCH) * ROLL
        matrix[0][0] = static_cast<float>(cp * cy);
        matrix[1][0] = static_cast<float>(cp * sy);
        matrix[2][0] = static_cast<float>(-sp);
        matrix[0][1] = static_cast<float>(sr * sp * cy + cr * -sy);
        matrix[1][1] = static_cast<float>(sr * sp * sy + cr * cy);
        matrix[2][1] = static_cast<float>(sr * cp);
        matrix[0][2] = static_cast<float>(cr * sp * cy + -sr * -sy);
        matrix[1][2] = static_cast<float>(cr * sp * sy + -sr * cy);
        matrix[2][2] = static_cast<float>(cr * cp);
        matrix[0][3] = 0.0;
        matrix[1][3] = 0.0;
        matrix[2][3] = 0.0;
    }

    /**
     * Convert Euler angles into an inverted rotation matrix
     * @param matrix The array to put the result into
     */
    void AngleIMatrix(float matrix[3][4]) const
    {
        const auto angles = *this;

        auto angle = static_cast<double>(angles[YAW]) * (M_PI * 2 / 360);
        const auto sy = sin(angle);
        const auto cy = cos(angle);
        angle = static_cast<double>(angles[PITCH]) * (M_PI * 2 / 360);
        const auto sp = sin(angle);
        const auto cp = cos(angle);
        angle = static_cast<double>(angles[ROLL]) * (M_PI * 2 / 360);
        const auto sr = sin(angle);
        const auto cr = cos(angle);

        // matrix = (YAW * PITCH) * ROLL
        matrix[0][0] = static_cast<float>(cp * cy);
        matrix[0][1] = static_cast<float>(cp * sy);
        matrix[0][2] = static_cast<float>(-sp);
        matrix[1][0] = static_cast<float>(sr * sp * cy + cr * -sy);
        matrix[1][1] = static_cast<float>(sr * sp * sy + cr * cy);
        matrix[1][2] = static_cast<float>(sr * cp);
        matrix[2][0] = static_cast<float>(cr * sp * cy + -sr * -sy);
        matrix[2][1] = static_cast<float>(cr * sp * sy + -sr * cy);
        matrix[2][2] = static_cast<float>(cr * cp);
        matrix[0][3] = 0.0;
        matrix[1][3] = 0.0;
        matrix[2][3] = 0.0;
    }

    // Members

    vec_t x, y, z;

    // Static functions

    static float DotProduct(const Vector& a, const Vector& b)
    {
        return a.Dot(b);
    }

    static Vector CrossProduct(const Vector& a, const Vector& b)
    {
        return a.Cross(b);
    }

    /**
     * Interpolate Euler angles
     * @param start Start Euler angle
     * @param end End Euler angle
     * @param frac Interpolation percentage from 0.0 to 1.0 (i.e., should probably be clamped, but doesn't have to be)
     */
    static Vector InterpolateAngles(const Vector& start, const Vector& end, const float frac)
    {
        auto s = start.NormalizeAngles();
        auto e = end.NormalizeAngles();
        auto output = Vector();

        for (auto i = 0; i < 3; i++)
        {
            const auto ang1 = s[i];
            const auto ang2 = e[i];

            auto d = ang2 - ang1;
            if (d > 180) d -= 360;
            else if (d < -180) d += 360;

            output[i] = ang1 + d * frac;
        }

        return output.NormalizeAngles();
    }

    /**
     * Convert a directional vector into Euler angles
     * @param forward The directional vector
     * @return The Euler angles 
     */
    static Vector FromAngles(const Vector& forward)
    {
        double yaw, pitch;

        if (forward[1] == 0 && forward[0] == 0)
        {
            yaw = 0;
            if (forward[2] > 0) pitch = 90;
            else pitch = 270;
        }
        else
        {
            yaw = atan2(forward[1], forward[0]) * 180 / M_PI;
            if (yaw < 0) yaw += 360;

            const auto fx = static_cast<double>(forward.x);
            const auto fy = static_cast<double>(forward.y);
            const auto tmp = sqrt(fx * fx + fy * fy);
            pitch = atan2(forward[2], tmp) * 180 / M_PI;
            if (pitch < 0) pitch += 360;
        }

        return Vector(static_cast<float>(pitch), static_cast<float>(yaw), 0);
    }
};

inline Vector operator*(const float fl, const Vector& v)
{
    return v * fl;
}

constexpr Vector vec3_origin(0, 0, 0);
constexpr Vector g_vecZero(0, 0, 0);
