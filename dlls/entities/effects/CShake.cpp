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

#include "CShake.h"

LINK_ENTITY_TO_CLASS(env_shake, CShake);

// pev->scale is amplitude
// pev->dmg_save is frequency
// pev->dmg_take is duration
// pev->dmg is radius
// radius of 0 means all players
// NOTE: UTIL_ScreenShake() will only shake players who are on the ground

#define SF_SHAKE_EVERYONE    0x0001        // Don't check radius
// UNDONE: These don't work yet
#define SF_SHAKE_DISRUPT    0x0002        // Disrupt controls
#define SF_SHAKE_INAIR        0x0004        // Shake players in air

void CShake::Spawn()
{
    pev->solid = SOLID_NOT;
    pev->movetype = MOVETYPE_NONE;
    pev->effects = 0;
    pev->frame = 0;

    m_iState = STATE_OFF; //LRC

    if (pev->spawnflags & SF_SHAKE_EVERYONE)
        pev->dmg = 0;
}


void CShake::KeyValue(KeyValueData* pkvd)
{
    if (FStrEq(pkvd->szKeyName, "amplitude"))
    {
        SetAmplitude(atof(pkvd->szValue));
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "frequency"))
    {
        SetFrequency(atof(pkvd->szValue));
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "duration"))
    {
        SetDuration(atof(pkvd->szValue));
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "radius"))
    {
        SetRadius(atof(pkvd->szValue));
        pkvd->fHandled = TRUE;
    }
    else
        CPointEntity::KeyValue(pkvd);
}


void CShake::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
    UTIL_ScreenShake(pev->origin, Amplitude(), Frequency(), Duration(), Radius());
    m_iState = STATE_ON; //LRC
    SetNextThink(Duration()); //LRC
}
