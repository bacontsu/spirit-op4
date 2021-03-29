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

LINK_ENTITY_TO_CLASS(trigger_inout, CTriggerInOut);

TYPEDESCRIPTION CTriggerInOut::m_SaveData[] =
{
    DEFINE_FIELD(CTriggerInOut, m_iszAltTarget, FIELD_STRING),
    DEFINE_FIELD(CTriggerInOut, m_iszBothTarget, FIELD_STRING),
    DEFINE_FIELD(CTriggerInOut, m_pRegister, FIELD_CLASSPTR),
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
        CBaseTrigger::KeyValue(pkvd);
}

void CTriggerInOut::Spawn()
{
    InitTrigger();
    // create a null-terminator for the registry
    m_pRegister = GetClassPtr((CInOutRegister*)NULL);
    m_pRegister->m_hValue = NULL;
    m_pRegister->m_pNext = NULL;
    m_pRegister->m_pField = this;
    m_pRegister->pev->classname = MAKE_STRING("inout_register");
}

void CTriggerInOut::Touch(CBaseEntity* pOther)
{
    if (!CanTouch(pOther->pev)) return;

    m_pRegister = m_pRegister->Add(pOther);

    if (m_fNextThink <= 0 && !m_pRegister->IsEmpty())
        SetNextThink(0.1);
}

void CTriggerInOut::Think()
{
    // Prune handles all Intersects tests and fires targets as appropriate
    m_pRegister = m_pRegister->Prune();

    if (m_pRegister->IsEmpty())
        DontThink();
    else
        SetNextThink(0.1);
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
    return m_pRegister->GetFirstEntityFrom(pStartEntity);
}
