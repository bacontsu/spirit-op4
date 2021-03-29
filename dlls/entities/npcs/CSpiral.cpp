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

#include "CSpiral.h"
#include "CGargantua.h"

LINK_ENTITY_TO_CLASS(streak_spiral, CSpiral);

void CSpiral::Spawn()
{
    pev->movetype = MOVETYPE_NONE;
    SetNextThink(0);
    pev->solid = SOLID_NOT;
    UTIL_SetSize(pev, g_vecZero, g_vecZero);
    pev->effects |= EF_NODRAW;
    pev->angles = g_vecZero;
}

CSpiral* CSpiral::Create(const Vector& origin, float height, float radius, float duration)
{
    if (duration <= 0)
        return NULL;

    CSpiral* pSpiral = GetClassPtr((CSpiral*)NULL);
    pSpiral->Spawn();
    pSpiral->pev->dmgtime = pSpiral->m_fNextThink;
    pSpiral->pev->origin = origin;
    pSpiral->pev->scale = radius;
    pSpiral->pev->dmg = height;
    pSpiral->pev->speed = duration;
    pSpiral->pev->health = 0;
    pSpiral->pev->angles = g_vecZero;

    return pSpiral;
}

#define SPIRAL_INTERVAL        0.1 //025

void CSpiral::Think()
{
    float time = gpGlobals->time - pev->dmgtime;

    while (time > SPIRAL_INTERVAL)
    {
        Vector position = pev->origin;
        Vector direction = Vector(0, 0, 1);

        float fraction = 1.0 / pev->speed;

        float radius = (pev->scale * pev->health) * fraction;

        position.z += (pev->health * pev->dmg) * fraction;
        pev->angles.y = (pev->health * 360 * 8) * fraction;
        UTIL_MakeVectors(pev->angles);
        position = position + gpGlobals->v_forward * radius;
        direction = (direction + gpGlobals->v_forward).Normalize();

        StreakSplash(position, Vector(0, 0, 1), RANDOM_LONG(8, 11), 20, RANDOM_LONG(50, 150), 400);

        // Jeez, how many counters should this take ? :)
        pev->dmgtime += SPIRAL_INTERVAL;
        pev->health += SPIRAL_INTERVAL;
        time -= SPIRAL_INTERVAL;
    }

    SetNextThink(0);

    if (pev->health >= pev->speed)
        UTIL_Remove(this);
}
