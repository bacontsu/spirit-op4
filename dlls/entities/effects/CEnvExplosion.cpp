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

#include "CEnvExplosion.h"

#include "util/decals.h"
#include "util/locus.h"
#include "weapons.h"

TYPEDESCRIPTION CEnvExplosion::m_SaveData[] =
{
    DEFINE_FIELD(CEnvExplosion, m_iMagnitude, FIELD_INTEGER),
    DEFINE_FIELD(CEnvExplosion, m_spriteScale, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CEnvExplosion, CBaseMonster);

LINK_ENTITY_TO_CLASS(env_explosion, CEnvExplosion);

void CEnvExplosion::KeyValue(KeyValueData* pkvd)
{
    if (FStrEq(pkvd->szKeyName, "iMagnitude"))
    {
        m_iMagnitude = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else
        CBaseEntity::KeyValue(pkvd);
}

void CEnvExplosion::Spawn()
{
    pev->solid = SOLID_NOT;
    pev->effects = EF_NODRAW;

    pev->movetype = MOVETYPE_NONE;
    /*
    if ( m_iMagnitude > 250 )
    {
        m_iMagnitude = 250;
    }
    */

    float flSpriteScale;
    flSpriteScale = (m_iMagnitude - 50) * 0.6;

    /*
    if ( flSpriteScale > 50 )
    {
        flSpriteScale = 50;
    }
    */
    if (flSpriteScale < 10)
    {
        flSpriteScale = 10;
    }

    m_spriteScale = (int)flSpriteScale;
}

void CEnvExplosion::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
    TraceResult tr;

    pev->model = iStringNull; //invisible
    pev->solid = SOLID_NOT; // intangible

    Vector vecSpot; // trace starts here!

    //LRC
    if (FStringNull(pev->target))
    {
        vecSpot = pev->origin;
    }
    else
    {
        vecSpot = CalcLocus_Position(this, pActivator, STRING(pev->target));
    }

    UTIL_TraceLine(vecSpot + Vector(0, 0, 8), vecSpot + Vector(0, 0, -32), ignore_monsters, ENT(pev), &tr);

    // Pull out of the wall a bit
    if (tr.flFraction != 1.0)
    {
        pev->origin = tr.vecEndPos + (tr.vecPlaneNormal * (m_iMagnitude - 24) * 0.6);
    }
    else
    {
        pev->origin = vecSpot; //LRC
    }

    // draw decal
    if (!(pev->spawnflags & SF_ENVEXPLOSION_NODECAL))
    {
        if (RANDOM_FLOAT(0, 1) < 0.5)
        {
            UTIL_DecalTrace(&tr, DECAL_SCORCH1);
        }
        else
        {
            UTIL_DecalTrace(&tr, DECAL_SCORCH2);
        }
    }

    // draw fireball
    if (!(pev->spawnflags & SF_ENVEXPLOSION_NOFIREBALL))
    {
        MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pev->origin);
        WRITE_BYTE(TE_EXPLOSION);
        WRITE_COORD(pev->origin.x);
        WRITE_COORD(pev->origin.y);
        WRITE_COORD(pev->origin.z);
        WRITE_SHORT(g_sModelIndexFireball);
        WRITE_BYTE((BYTE)m_spriteScale); // scale * 10
        WRITE_BYTE(15); // framerate
        WRITE_BYTE(TE_EXPLFLAG_NONE);
        MESSAGE_END();
    }
    else
    {
        MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pev->origin);
        WRITE_BYTE(TE_EXPLOSION);
        WRITE_COORD(pev->origin.x);
        WRITE_COORD(pev->origin.y);
        WRITE_COORD(pev->origin.z);
        WRITE_SHORT(g_sModelIndexFireball);
        WRITE_BYTE(0); // no sprite
        WRITE_BYTE(15); // framerate
        WRITE_BYTE(TE_EXPLFLAG_NONE);
        MESSAGE_END();
    }

    // do damage
    if (!(pev->spawnflags & SF_ENVEXPLOSION_NODAMAGE))
    {
        RadiusDamage(pev, pev, m_iMagnitude, CLASS_NONE, DMG_BLAST);
    }

    SetThink(&CEnvExplosion::Smoke);
    SetNextThink(0.3);

    // draw sparks
    if (!(pev->spawnflags & SF_ENVEXPLOSION_NOSPARKS))
    {
        int sparkCount = RANDOM_LONG(0, 3);

        for (int i = 0; i < sparkCount; i++)
        {
            Create("spark_shower", pev->origin, tr.vecPlaneNormal, NULL);
        }
    }
}

void CEnvExplosion::Smoke()
{
    if (!(pev->spawnflags & SF_ENVEXPLOSION_NOSMOKE))
    {
        MESSAGE_BEGIN(MSG_PAS, SVC_TEMPENTITY, pev->origin);
        WRITE_BYTE(TE_SMOKE);
        WRITE_COORD(pev->origin.x);
        WRITE_COORD(pev->origin.y);
        WRITE_COORD(pev->origin.z);
        WRITE_SHORT(g_sModelIndexSmoke);
        WRITE_BYTE((BYTE)m_spriteScale); // scale * 10
        WRITE_BYTE(12); // framerate
        MESSAGE_END();
    }

    if (!(pev->spawnflags & SF_ENVEXPLOSION_REPEATABLE))
    {
        UTIL_Remove(this);
    }
}

// HACKHACK -- create one of these and fake a keyvalue to get the right explosion setup
void ExplosionCreate(const Vector& center, const Vector& angles, edict_t* pOwner, int magnitude, BOOL doDamage)
{
    KeyValueData kvd;
    char buf[128];

    CBaseEntity* pExplosion = CBaseEntity::Create("env_explosion", center, angles, pOwner);
    sprintf(buf, "%3d", magnitude);
    kvd.szKeyName = "iMagnitude";
    kvd.szValue = buf;
    pExplosion->KeyValue(&kvd);
    if (!doDamage)
        pExplosion->pev->spawnflags |= SF_ENVEXPLOSION_NODAMAGE;

    pExplosion->Spawn();
    pExplosion->Use(NULL, NULL, USE_TOGGLE, 0);
}
