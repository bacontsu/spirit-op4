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

#include "CDeadGenericMonster.h"

LINK_ENTITY_TO_CLASS(monster_generic_dead, CDeadGenericMonster);

TYPEDESCRIPTION CDeadGenericMonster::m_SaveData[] =
{
    DEFINE_FIELD(CDeadGenericMonster, m_iszGibModel, FIELD_STRING),
};

IMPLEMENT_SAVERESTORE(CDeadGenericMonster, CBaseMonster);

void CDeadGenericMonster::KeyValue(KeyValueData* pkvd)
{
    if (FStrEq(pkvd->szKeyName, "m_bloodColor"))
    {
        m_bloodColor = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iszGibModel"))
    {
        m_iszGibModel = ALLOC_STRING(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else
        CBaseMonster::KeyValue(pkvd);
}

//=========================================================
// ********** DeadGenericMonster SPAWN **********
//=========================================================
void CDeadGenericMonster::Spawn()
{
    Precache();
    SET_MODEL(ENT(pev), STRING(pev->model));

    pev->effects = 0;
    pev->yaw_speed = 8; //LRC -- what?
    pev->sequence = 0;

    if (pev->netname)
    {
        pev->sequence = LookupSequence(STRING(pev->netname));

        if (pev->sequence == -1)
        {
            ALERT(at_debug, "Invalid sequence name \"%s\" in monster_generic_dead\n", STRING(pev->netname));
        }
    }
    else
    {
        pev->sequence = LookupActivity(pev->frags);
        //        if (pev->sequence == -1)
        //        {
        //            ALERT ( at_error, "monster_generic_dead - specify a sequence name or choose a different death type: model \"%s\" has no available death sequences.\n", STRING(pev->model) );
        //        }
        //...and if that doesn't work, forget it.
    }

    // Corpses have less health
    pev->health = 8;

    MonsterInitDead();

    ResetSequenceInfo();
    pev->frame = 255; // pose at the _end_ of its death sequence.
}

void CDeadGenericMonster::Precache()
{
    PRECACHE_MODEL((char*)STRING(pev->model));
    if (m_iszGibModel)
        PRECACHE_MODEL((char*)STRING(m_iszGibModel)); //LRC
}
