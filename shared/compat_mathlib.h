/***
*
* Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
* This product contains software technology licensed from Id
* Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
* All Rights Reserved.
*
* Use, distribution, and modification of this source code and/or resulting
* object code is restricted to non-commercial enhancements to products from
* Valve LLC.All other use, distribution, or modification is prohibited
* without written permission from Valve LLC.
*
****/
#pragma once
#pragma warning(disable : 4244) // conversion from 'double' to 'float', possible loss of data

#include <memory.h>

#include "Vector.h"

// Compatibility with old mathlib.h

//[[deprecated]]
inline float DotProduct(const float* a, const float* b)
{
    return Vector(a).Dot(b);
}

//[[deprecated]]
inline void CrossProduct(const float* v1, const float* v2, float* cross)
{
    cross[0] = v1[1] * v2[2] - v1[2] * v2[1];
    cross[1] = v1[2] * v2[0] - v1[0] * v2[2];
    cross[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

//[[deprecated]]
inline void VectorAdd(const float* a, const float* b, float* c)
{
    c[0] = a[0] + b[0];
    c[1] = a[1] + b[1];
    c[2] = a[2] + b[2];
}

//[[deprecated]]
inline void VectorSubtract(const float* a, const float* b, float* c)
{
    c[0] = a[0] - b[0];
    c[1] = a[1] - b[1];
    c[2] = a[2] - b[2];
}

//[[deprecated]]
inline void VectorCopy(const float* a, float* b)
{
    b[0] = a[0];
    b[1] = a[1];
    b[2] = a[2];
}

//[[deprecated]]
inline void VectorClear(float* a)
{
    a[0] = 0.0;
    a[1] = 0.0;
    a[2] = 0.0;
}

//[[deprecated]]
inline void VectorMA(const float* veca, const float scale, const float* vecb, float* vecc)
{
    vecc[0] = veca[0] + scale * vecb[0];
    vecc[1] = veca[1] + scale * vecb[1];
    vecc[2] = veca[2] + scale * vecb[2];
}

//[[deprecated]]
inline int VectorCompare(const Vector& v1, const Vector& v2)
{
    return v1 == v2;
}

//[[deprecated]]
inline float Length(const Vector& v)
{
    return v.Length();
}

//[[deprecated]]
inline float VectorNormalize(float* v)
{
    auto length = v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
    length = sqrt(length); // FIXME

    if (length)
    {
        const auto ilength = 1 / length;
        v[0] *= ilength;
        v[1] *= ilength;
        v[2] *= ilength;
    }

    return length;
}

//[[deprecated]]
inline void VectorInverse(float* v)
{
    v[0] = -v[0];
    v[1] = -v[1];
    v[2] = -v[2];
}

//[[deprecated]]
inline void VectorScale(const float* in, const float scale, float* out)
{
    out[0] = in[0] * scale;
    out[1] = in[1] * scale;
    out[2] = in[2] * scale;
}

//[[deprecated]]
inline void AngleVectors(const Vector& angles, Vector* forward, Vector* right, Vector* up)
{
    return angles.AngleVectors(forward, right, up);
}

//[[deprecated]]
inline void AngleVectorsTranspose(const Vector& angles, Vector* forward, Vector* right, Vector* up)
{
    return angles.AngleVectorsTranspose(forward, right, up);
}

//[[deprecated]]
inline void AngleMatrix(const Vector& angles, float matrix[3][4])
{
    angles.AngleMatrix(matrix);
}

//[[deprecated]]
inline void VectorTransform(const Vector& in1, float in2[3][4], Vector& out)
{
    out = in1.Transform(in2);
}

//[[deprecated]]
inline void NormalizeAngles(float* angles)
{
    // Normalize angles
    for (auto i = 0; i < 3; i++)
    {
        if (angles[i] > 180.0f) angles[i] -= 360.0f;
        else if (angles[i] < -180.0f) angles[i] += 360.0f;
    }
}

//[[deprecated]]
inline void VectorAngles(const float* forward, float* angles)
{
    Vector::FromAngles(forward).CopyToArray(angles);
}

// Not deprecated... I guess?

inline float anglemod(const float a)
{
    return 360.0f / 65536 * static_cast<float>((static_cast<int>(a * (65536 / 360.0f)) & 65535));
}

inline float Distance(const float* v1, const float* v2)
{
    Vector d;
    VectorSubtract(v2, v1, d);
    return Length(d);
}

// Model stuff

inline void ConcatTransforms(const float in1[3][4], const float in2[3][4], float out[3][4])
{
    out[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] + in1[0][2] * in2[2][0];
    out[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] + in1[0][2] * in2[2][1];
    out[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] + in1[0][2] * in2[2][2];
    out[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] + in1[0][2] * in2[2][3] + in1[0][3];
    out[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] + in1[1][2] * in2[2][0];
    out[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] + in1[1][2] * in2[2][1];
    out[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] + in1[1][2] * in2[2][2];
    out[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] + in1[1][2] * in2[2][3] + in1[1][3];
    out[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] + in1[2][2] * in2[2][0];
    out[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] + in1[2][2] * in2[2][1];
    out[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] + in1[2][2] * in2[2][2];
    out[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] + in1[2][2] * in2[2][3] + in1[2][3];
}

inline void QuaternionMatrix(const vec4_t quaternion, float (*matrix)[4])
{
    matrix[0][0] = 1.0 - 2.0 * quaternion[1] * quaternion[1] - 2.0 * quaternion[2] * quaternion[2];
    matrix[1][0] = 2.0 * quaternion[0] * quaternion[1] + 2.0 * quaternion[3] * quaternion[2];
    matrix[2][0] = 2.0 * quaternion[0] * quaternion[2] - 2.0 * quaternion[3] * quaternion[1];

    matrix[0][1] = 2.0 * quaternion[0] * quaternion[1] - 2.0 * quaternion[3] * quaternion[2];
    matrix[1][1] = 1.0 - 2.0 * quaternion[0] * quaternion[0] - 2.0 * quaternion[2] * quaternion[2];
    matrix[2][1] = 2.0 * quaternion[1] * quaternion[2] + 2.0 * quaternion[3] * quaternion[0];

    matrix[0][2] = 2.0 * quaternion[0] * quaternion[2] + 2.0 * quaternion[3] * quaternion[1];
    matrix[1][2] = 2.0 * quaternion[1] * quaternion[2] - 2.0 * quaternion[3] * quaternion[0];
    matrix[2][2] = 1.0 - 2.0 * quaternion[0] * quaternion[0] - 2.0 * quaternion[1] * quaternion[1];
}

inline void QuaternionSlerp(const vec4_t p, vec4_t q, const float t, vec4_t qt)
{
    int i;
    float sclp, sclq;

    // decide if one of the quaternions is backwards
    float a = 0;
    float b = 0;

    for (i = 0; i < 4; i++)
    {
        a += (p[i] - q[i]) * (p[i] - q[i]);
        b += (p[i] + q[i]) * (p[i] + q[i]);
    }
    if (a > b)
    {
        for (i = 0; i < 4; i++)
        {
            q[i] = -q[i];
        }
    }

    const auto cosom = p[0] * q[0] + p[1] * q[1] + p[2] * q[2] + p[3] * q[3];

    if ((1.0 + cosom) > 0.000001)
    {
        if ((1.0 - cosom) > 0.000001)
        {
            const auto omega = acos(cosom);
            const auto sinom = sin(omega);
            sclp = sin((1.0 - t) * omega) / sinom;
            sclq = sin(t * omega) / sinom;
        }
        else
        {
            sclp = 1.0 - t;
            sclq = t;
        }
        for (i = 0; i < 4; i++)
        {
            qt[i] = sclp * p[i] + sclq * q[i];
        }
    }
    else
    {
        qt[0] = -q[1];
        qt[1] = q[0];
        qt[2] = -q[3];
        qt[3] = q[2];
        sclp = sin((1.0 - t) * (0.5 * M_PI));
        sclq = sin(t * (0.5 * M_PI));
        for (i = 0; i < 3; i++)
        {
            qt[i] = sclp * p[i] + sclq * qt[i];
        }
    }
}

inline void AngleQuaternion(const float* angles, vec4_t quaternion)
{
    // FIXME: rescale the inputs to 1/2 angle
    float angle = angles[2] * 0.5;
    const auto sy = sin(angle);
    const auto cy = cos(angle);
    angle = angles[1] * 0.5;
    const auto sp = sin(angle);
    const auto cp = cos(angle);
    angle = angles[0] * 0.5;
    const auto sr = sin(angle);
    const auto cr = cos(angle);

    quaternion[0] = sr * cp * cy - cr * sp * sy; // X
    quaternion[1] = cr * sp * cy + sr * cp * sy; // Y
    quaternion[2] = cr * cp * sy - sr * sp * cy; // Z
    quaternion[3] = cr * cp * cy + sr * sp * sy; // W
}

inline void MatrixCopy(const float in[3][4], float out[3][4])
{
    memcpy(out, in, sizeof(float) * 3 * 4);
}
