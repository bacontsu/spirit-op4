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

#include "CXenSpore.h"

void CXenSpore::Spawn()
{
    Precache();

    SET_MODEL(ENT(pev), pModelNames[pev->skin]);
    pev->movetype = MOVETYPE_NONE;
    pev->solid = SOLID_BBOX;
    pev->takedamage = DAMAGE_YES;

    //    SetActivity( ACT_IDLE );
    pev->sequence = 0;
    pev->frame = RANDOM_FLOAT(0, 255);
    pev->framerate = RANDOM_FLOAT(0.7, 1.4);
    ResetSequenceInfo();
    SetNextThink(RANDOM_FLOAT(0.1, 0.4)); // Load balance these a bit
}

const char* CXenSpore::pModelNames[] =
{
    "models/fungus(small).mdl",
    "models/fungus.mdl",
    "models/fungus(large).mdl",
};

void CXenSpore::Precache()
{
    PRECACHE_MODEL((char*)pModelNames[pev->skin]);
}

void CXenSpore::Touch(CBaseEntity* pOther)
{
}

void CXenSpore::Think()
{
    float flInterval = StudioFrameAdvance();
    SetNextThink(0.1);

#if 0
    DispatchAnimEvents(flInterval);

    switch (GetActivity())
    {
    default:
    case ACT_IDLE:
        break;

    }
#endif
}
