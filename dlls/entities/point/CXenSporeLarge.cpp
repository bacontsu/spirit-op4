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
* Valve LLC.  All other use, distribution, or modification is prohibited
* without written permission from Valve LLC.
*
****/

#include "CXenSporeLarge.h"
#include "CXenHull.h"

LINK_ENTITY_TO_CLASS(xen_spore_large, CXenSporeLarge);

// I just eyeballed these -- fill in hulls for the legs
const Vector CXenSporeLarge::m_hullSizes[] =
{
    Vector(90, -25, 0),
    Vector(25, 75, 0),
    Vector(-15, -100, 0),
    Vector(-90, -35, 0),
    Vector(-90, 60, 0),
};

void CXenSporeLarge::Spawn()
{
    pev->skin = 2;
    CXenSpore::Spawn();
    UTIL_SetSize(pev, Vector(-48, -48, 110), Vector(48, 48, 240));

    Vector forward, right;

    UTIL_MakeVectorsPrivate(pev->angles, forward, right, NULL);

    // Rotate the leg hulls into position
    for (int i = 0; i < ARRAYSIZE(m_hullSizes); i++)
        CXenHull::CreateHull(this, Vector(-12, -12, 0), Vector(12, 12, 120), (m_hullSizes[i].x * forward) + (m_hullSizes[i].y * right));
}
