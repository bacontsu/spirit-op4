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

#include "CStomp.h"
#include "CGargantua.h"
#include "entities/effects/CSprite.h"

LINK_ENTITY_TO_CLASS(garg_stomp, CStomp);

CStomp* CStomp::StompCreate(const Vector& origin, const Vector& end, float speed)
{
    CStomp* pStomp = GetClassPtr((CStomp*)NULL);

    pStomp->pev->origin = origin;
    Vector dir = (end - origin);
    pStomp->pev->scale = dir.Length();
    pStomp->pev->movedir = dir.Normalize();
    pStomp->pev->speed = speed;
    pStomp->Spawn();

    return pStomp;
}

void CStomp::Spawn()
{
    SetNextThink(0);
    pev->classname = MAKE_STRING("garg_stomp");
    pev->dmgtime = gpGlobals->time;

    pev->framerate = 30;
    pev->model = MAKE_STRING(GARG_STOMP_SPRITE_NAME);
    pev->rendermode = kRenderTransTexture;
    pev->renderamt = 0;
    EMIT_SOUND_DYN(edict(), CHAN_BODY, GARG_STOMP_BUZZ_SOUND, 1, ATTN_NORM, 0, PITCH_NORM * 0.55);
}

void CStomp::Think()
{
    TraceResult tr;

    SetNextThink(0.1);

    // Do damage for this frame
    Vector vecStart = pev->origin;
    vecStart.z += 30;
    Vector vecEnd = vecStart + (pev->movedir * pev->speed * gpGlobals->frametime);

    UTIL_TraceHull(vecStart, vecEnd, dont_ignore_monsters, head_hull, ENT(pev), &tr);

    if (tr.pHit && tr.pHit != pev->owner)
    {
        CBaseEntity* pEntity = CBaseEntity::Instance(tr.pHit);
        entvars_t* pevOwner = pev;
        if (pev->owner)
            pevOwner = VARS(pev->owner);

        if (pEntity)
            pEntity->TakeDamage(pev, pevOwner, gSkillData.gargantuaDmgStomp, DMG_SONIC);
    }

    // Accelerate the effect
    pev->speed = pev->speed + (gpGlobals->frametime) * pev->framerate;
    pev->framerate = pev->framerate + (gpGlobals->frametime) * 1500;

    // Move and spawn trails
    while (gpGlobals->time - pev->dmgtime > STOMP_INTERVAL)
    {
        pev->origin = pev->origin + pev->movedir * pev->speed * STOMP_INTERVAL;
        for (int i = 0; i < 2; i++)
        {
            CSprite* pSprite = CSprite::SpriteCreate(GARG_STOMP_SPRITE_NAME, pev->origin, TRUE);
            if (pSprite)
            {
                UTIL_TraceLine(pev->origin, pev->origin - Vector(0, 0, 500), ignore_monsters, edict(), &tr);
                pSprite->pev->origin = tr.vecEndPos;
                pSprite->pev->velocity = Vector(RANDOM_FLOAT(-200, 200), RANDOM_FLOAT(-200, 200), 175);
                // pSprite->AnimateAndDie( RANDOM_FLOAT( 8.0, 12.0 ) );
                pSprite->SetNextThink(0.3);
                pSprite->SetThink(&CSprite::SUB_Remove);
                pSprite->SetTransparency(kRenderTransAdd, 255, 255, 255, 255, kRenderFxFadeFast);
            }
        }
        pev->dmgtime += STOMP_INTERVAL;
        // Scale has the "life" of this effect
        pev->scale -= STOMP_INTERVAL * pev->speed;
        if (pev->scale <= 0)
        {
            // Life has run out
            UTIL_Remove(this);
            STOP_SOUND(edict(), CHAN_BODY, GARG_STOMP_BUZZ_SOUND);
        }
    }
}
