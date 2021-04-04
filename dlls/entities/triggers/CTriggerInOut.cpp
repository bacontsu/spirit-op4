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

#include "CTriggerInOut.h"

/**
 * trigger_inout
 * -------------
 * Fires `target` with USE_TOGGLE and `m_iszBothTarget` with USE_ON when an entity enters the trigger.
 * Fires `m_iszAltTarget` with USE_TOGGLE and `m_iszBothTarget` with USE_OFF when an entity enters the trigger.
 * Acts as a multisource (has state) - is ON when any entity is inside, and OFF when no entity is inside.
 */

LINK_ENTITY_TO_CLASS(trigger_inout, CTriggerInOut);

TYPEDESCRIPTION CTriggerInOut::m_SaveData[] =
{
    DEFINE_FIELD(CTriggerInOut, m_iszAltTarget, FIELD_STRING),
    DEFINE_FIELD(CTriggerInOut, m_iszBothTarget, FIELD_STRING),
    DEFINE_STD_VECTOR(CTriggerInOut, entitiesInside, FIELD_CLASSPTR)
};

IMPLEMENT_SAVERESTORE(CTriggerInOut, CBaseTrigger);

void CTriggerInOut::KeyValue(KeyValueData* pkvd)
{
    if (FStrEq(pkvd->szKeyName, "m_iszAltTarget"))
    {
        m_iszAltTarget = ALLOC_STRING(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iszBothTarget"))
    {
        m_iszBothTarget = ALLOC_STRING(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else
    {
        CBaseTrigger::KeyValue(pkvd);
    }
}

void CTriggerInOut::Spawn()
{
    InitTrigger();
    entitiesInside = std::vector<CBaseEntity*>();
}

void CTriggerInOut::OnBeforeSave(CSave& save)
{
    RemoveNullEntities();
}

void CTriggerInOut::Touch(CBaseEntity* pOther)
{
    if (!CanTouch(pOther->pev)) return;

    auto alreadyInside = false;
    auto iter = entitiesInside.begin();
    while (iter != entitiesInside.end())
    {
        auto* ent = *iter;
        if (ent == pOther)
        {
            alreadyInside = true;
            break;
        }
        ++iter;
    }

    if (!alreadyInside)
    {
        entitiesInside.push_back(pOther);
        FireOnEntry(pOther);
    }

    if (m_fNextThink <= 0 && !entitiesInside.empty())
    {
        SetNextThink(0.1f);
    }
}

void CTriggerInOut::RemoveNullEntities()
{
    entitiesInside.erase(std::remove_if(entitiesInside.begin(), entitiesInside.end(), [](CBaseEntity* ent)
    {
        return FNullEnt(ent);
    }), entitiesInside.end());
}

void CTriggerInOut::Think()
{
    RemoveNullEntities();
    auto iter = entitiesInside.begin();
    while (iter != entitiesInside.end())
    {
        auto* ent = *iter;
        if (!Intersects(ent))
        {
            // This entity has just left the field, trigger and stop tracking
            FireOnLeaving(ent);
            iter = entitiesInside.erase(iter);
        }
        else
        {
            // This entity is still inside the field, do nothing
            ++iter;
        }
    }
    if (entitiesInside.empty()) DontThink();
    else SetNextThink(0.1f);
}

void CTriggerInOut::FireOnEntry(CBaseEntity* pOther)
{
    if (UTIL_IsMasterTriggered(m_sMaster, pOther))
    {
        FireTargets(STRING(m_iszBothTarget), pOther, this, USE_ON, 0);
        FireTargets(STRING(pev->target), pOther, this, USE_TOGGLE, 0);
    }
}

void CTriggerInOut::FireOnLeaving(CBaseEntity* pEnt)
{
    if (UTIL_IsMasterTriggered(m_sMaster, pEnt))
    {
        FireTargets(STRING(m_iszBothTarget), pEnt, this, USE_OFF, 0);
        FireTargets(STRING(m_iszAltTarget), pEnt, this, USE_TOGGLE, 0);
    }
}

CBaseEntity* CTriggerInOut::FollowAlias(CBaseEntity* pStartEntity)
{
    CBaseEntity* result = nullptr;

    auto currentOffset = -1;
    if (pStartEntity) currentOffset = OFFSET(pStartEntity->pev);

    auto iter = entitiesInside.begin();
    while (iter != entitiesInside.end())
    {
        auto* current = *iter;
        if (!FNullEnt(current))
        {
            const auto testOffset = OFFSET(current->pev);
            if (testOffset > currentOffset)
            {
                result = current;
                currentOffset = testOffset;
            }
        }
        ++iter;
    }

    return result;
}
