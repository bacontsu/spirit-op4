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

#include "CApacheHVR.h"

LINK_ENTITY_TO_CLASS(hvr_rocket, CApacheHVR);

TYPEDESCRIPTION CApacheHVR::m_SaveData[] =
{
    //    DEFINE_FIELD( CApacheHVR, m_iTrail, FIELD_INTEGER ),    // Dont' save, precache
    DEFINE_FIELD(CApacheHVR, m_vecForward, FIELD_VECTOR),
};

IMPLEMENT_SAVERESTORE(CApacheHVR, CGrenade);

void CApacheHVR::Spawn()
{
    Precache();
    // motor
    pev->movetype = MOVETYPE_FLY;
    pev->solid = SOLID_BBOX;

    SET_MODEL(ENT(pev), "models/HVR.mdl");
    UTIL_SetSize(pev, Vector(0, 0, 0), Vector(0, 0, 0));
    UTIL_SetOrigin(this, pev->origin);

    SetThink(&CApacheHVR::IgniteThink);
    SetTouch(&CApacheHVR::ExplodeTouch);

    UTIL_MakeAimVectors(pev->angles);
    m_vecForward = gpGlobals->v_forward;
    pev->gravity = 0.5;

    SetNextThink(0.1);

    pev->dmg = 150;
}


void CApacheHVR::Precache()
{
    PRECACHE_MODEL("models/HVR.mdl");
    m_iTrail = PRECACHE_MODEL("sprites/smoke.spr");
    PRECACHE_SOUND("weapons/rocket1.wav");
}


void CApacheHVR::IgniteThink()
{
    // pev->movetype = MOVETYPE_TOSS;

    // pev->movetype = MOVETYPE_FLY;
    pev->effects |= EF_LIGHT;

    // make rocket sound
    EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/rocket1.wav", 1, 0.5);

    // rocket trail
    MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY);

    WRITE_BYTE(TE_BEAMFOLLOW);
    WRITE_SHORT(entindex()); // entity
    WRITE_SHORT(m_iTrail); // model
    WRITE_BYTE(15); // life
    WRITE_BYTE(5); // width
    WRITE_BYTE(224); // r, g, b
    WRITE_BYTE(224); // r, g, b
    WRITE_BYTE(255); // r, g, b
    WRITE_BYTE(255); // brightness

    MESSAGE_END(); // move PHS/PVS data sending into here (SEND_ALL, SEND_PVS, SEND_PHS)

    // set to accelerate
    SetThink(&CApacheHVR::AccelerateThink);
    SetNextThink(0.1);
}


void CApacheHVR::AccelerateThink()
{
    // check world boundaries
    if (pev->origin.x < -4096 || pev->origin.x > 4096 || pev->origin.y < -4096 || pev->origin.y > 4096 || pev->origin.z < -4096 || pev->origin.z > 4096)
    {
        UTIL_Remove(this);
        return;
    }

    // accelerate
    float flSpeed = pev->velocity.Length();
    if (flSpeed < 1800)
    {
        pev->velocity = pev->velocity + m_vecForward * 200;
    }

    // re-aim
    pev->angles = UTIL_VecToAngles(pev->velocity);

    SetNextThink(0.1);
}
