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

#include "CControllerZapBall.h"
#include "util/skill.h"
#include "weapons.h"

LINK_ENTITY_TO_CLASS(controller_energy_ball, CControllerZapBall);

void CControllerZapBall::Spawn()
{
    Precache();
    // motor
    pev->movetype = MOVETYPE_FLY;
    pev->solid = SOLID_BBOX;

    SET_MODEL(ENT(pev), "sprites/xspark4.spr");
    pev->rendermode = kRenderTransAdd;
    pev->rendercolor.x = 255;
    pev->rendercolor.y = 255;
    pev->rendercolor.z = 255;
    pev->renderamt = 255;
    pev->scale = 0.5;

    UTIL_SetSize(pev, Vector(0, 0, 0), Vector(0, 0, 0));
    UTIL_SetOrigin(this, pev->origin);

    SetThink(&CControllerZapBall::AnimateThink);
    SetTouch(&CControllerZapBall::ExplodeTouch);

    m_hOwner = Instance(pev->owner);
    pev->dmgtime = gpGlobals->time; // keep track of when ball spawned
    SetNextThink(0.1);
}


void CControllerZapBall::Precache()
{
    PRECACHE_MODEL("sprites/xspark4.spr");
    // PRECACHE_SOUND("debris/zap4.wav");
    // PRECACHE_SOUND("weapons/electro4.wav");
}


void CControllerZapBall::AnimateThink()
{
    SetNextThink(0.1);

    pev->frame = ((int)pev->frame + 1) % 11;

    if (gpGlobals->time - pev->dmgtime > 5 || pev->velocity.Length() < 10)
    {
        SetTouch(NULL);
        UTIL_Remove(this);
    }
}


void CControllerZapBall::ExplodeTouch(CBaseEntity* pOther)
{
    if (pOther->pev->takedamage)
    {
        TraceResult tr = UTIL_GetGlobalTrace();

        entvars_t* pevOwner;
        if (m_hOwner != NULL)
        {
            pevOwner = m_hOwner->pev;
        }
        else
        {
            pevOwner = pev;
        }

        ClearMultiDamage();
        pOther->TraceAttack(pevOwner, gSkillData.controllerDmgBall, pev->velocity.Normalize(), &tr, DMG_ENERGYBEAM);
        ApplyMultiDamage(pevOwner, pevOwner);

        UTIL_EmitAmbientSound(ENT(pev), tr.vecEndPos, "weapons/electro4.wav", 0.3, ATTN_NORM, 0, RANDOM_LONG(90, 99));
    }

    UTIL_Remove(this);
}
