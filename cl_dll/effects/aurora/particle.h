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

#include "hud/hud.h"

class ParticleType;

struct particle
{
    particle* nextpart;

    particle* m_pOverlay; // for making multi-layered particles

    ParticleType* pType;

    Vector origin;
    Vector velocity;
    Vector accel;
    Vector m_vecWind;

    int m_iEntIndex; // if non-zero, this particle is tied to the given entity

    float m_fRed;
    float m_fGreen;
    float m_fBlue;
    float m_fRedStep;
    float m_fGreenStep;
    float m_fBlueStep;

    float m_fAlpha;
    float m_fAlphaStep;

    float frame;
    float m_fFrameStep;

    float m_fAngle;
    float m_fAngleStep;

    float m_fSize;
    float m_fSizeStep;

    float m_fDrag;

    float age;
    float age_death;
    float age_spray;
};
