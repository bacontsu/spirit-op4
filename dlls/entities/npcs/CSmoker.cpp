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

#include "CSmoker.h"
#include "weapons.h"

LINK_ENTITY_TO_CLASS(env_smoker, CSmoker);

void CSmoker::Spawn()
{
    pev->movetype = MOVETYPE_NONE;
    SetNextThink(0);
    pev->solid = SOLID_NOT;
    UTIL_SetSize(pev, g_vecZero, g_vecZero);
    pev->effects |= EF_NODRAW;
    pev->angles = g_vecZero;
}


void CSmoker::Think()
{
    // lots of smoke
    MESSAGE_BEGIN(MSG_PVS, SVC_TEMPENTITY, pev->origin);
    WRITE_BYTE(TE_SMOKE);
    WRITE_COORD(pev->origin.x + RANDOM_FLOAT(-pev->dmg, pev->dmg));
    WRITE_COORD(pev->origin.y + RANDOM_FLOAT(-pev->dmg, pev->dmg));
    WRITE_COORD(pev->origin.z);
    WRITE_SHORT(g_sModelIndexSmoke);
    WRITE_BYTE(RANDOM_LONG(pev->scale, pev->scale * 1.1));
    WRITE_BYTE(RANDOM_LONG(8, 14)); // framerate
    MESSAGE_END();

    pev->health--;
    if (pev->health > 0)
        SetNextThink(RANDOM_FLOAT(0.1, 0.2));
    else
        UTIL_Remove(this);
}
