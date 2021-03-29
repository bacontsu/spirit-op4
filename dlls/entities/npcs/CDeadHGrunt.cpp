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

#include "CDeadHGrunt.h"
#include "CHGrunt.h"

const char* CDeadHGrunt::m_szPoses[] = {"deadstomach", "deadside", "deadsitting"};

void CDeadHGrunt::KeyValue(KeyValueData* pkvd)
{
    if (FStrEq(pkvd->szKeyName, "pose"))
    {
        m_iPose = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else
        CBaseMonster::KeyValue(pkvd);
}

LINK_ENTITY_TO_CLASS(monster_hgrunt_dead, CDeadHGrunt);

//=========================================================
// ********** DeadHGrunt SPAWN **********
//=========================================================
void CDeadHGrunt::Spawn()
{
    int oldBody;

    PRECACHE_MODEL("models/hgrunt.mdl");
    SET_MODEL(ENT(pev), "models/hgrunt.mdl");

    pev->effects = 0;
    pev->yaw_speed = 8;
    pev->sequence = 0;
    m_bloodColor = BLOOD_COLOR_RED;

    pev->sequence = LookupSequence(m_szPoses[m_iPose]);

    if (pev->sequence == -1)
    {
        ALERT(at_debug, "Dead hgrunt with bad pose\n");
    }

    // Corpses have less health
    pev->health = 8;

    oldBody = pev->body;
    pev->body = 0;

    if (oldBody >= 5 && oldBody <= 7)
        pev->skin = 1;
    else
        pev->skin = 0;

    switch (pev->weapons)
    {
    case 0: // MP5
        SetBodygroup(GUN_GROUP, GUN_MP5);
        break;
    case 1: // Shotgun
        SetBodygroup(GUN_GROUP, GUN_SHOTGUN);
        break;
    case 2: // No gun
        SetBodygroup(GUN_GROUP, GUN_NONE);
        break;
    }

    switch (oldBody)
    {
    case 2: // Gasmask, no gun
        SetBodygroup(GUN_GROUP, GUN_NONE); //fall through
    case 0:
    case 6: // Gasmask (white/black)
        SetBodygroup(HEAD_GROUP, HEAD_GRUNT);
        break;
    case 3: // Commander, no gun
        SetBodygroup(GUN_GROUP, GUN_NONE); //fall through
    case 1: // Commander
        SetBodygroup(HEAD_GROUP, HEAD_COMMANDER);
        break;
    case 4:
    case 7: // Skimask (white/black)
        SetBodygroup(HEAD_GROUP, HEAD_SHOTGUN);
        break;
    case 5: // Commander
        SetBodygroup(HEAD_GROUP, HEAD_M203);
        break;
    }

    MonsterInitDead();
}
