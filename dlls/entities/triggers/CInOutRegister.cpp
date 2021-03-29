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

#include "CInOutRegister.h"
#include "CTriggerInOut.h"

LINK_ENTITY_TO_CLASS(inout_register, CInOutRegister);

TYPEDESCRIPTION CInOutRegister::m_SaveData[] =
{
    DEFINE_FIELD(CInOutRegister, m_pField, FIELD_CLASSPTR),
    DEFINE_FIELD(CInOutRegister, m_pNext, FIELD_CLASSPTR),
    DEFINE_FIELD(CInOutRegister, m_hValue, FIELD_EHANDLE),
};

IMPLEMENT_SAVERESTORE(CInOutRegister, CPointEntity);

BOOL CInOutRegister::IsRegistered(CBaseEntity* pValue)
{
    if (m_hValue == pValue)
        return TRUE;
    else if (m_pNext)
        return m_pNext->IsRegistered(pValue);
    else
        return FALSE;
}

CInOutRegister* CInOutRegister::Add(CBaseEntity* pValue)
{
    if (m_hValue == pValue)
    {
        // it's already in the list, don't need to do anything
        return this;
    }
    else if (m_pNext)
    {
        // keep looking
        m_pNext = m_pNext->Add(pValue);
        return this;
    }
    else
    {
        // reached the end of the list; add the new entry, and trigger
        CInOutRegister* pResult = GetClassPtr((CInOutRegister*)NULL);
        pResult->m_hValue = pValue;
        pResult->m_pNext = this;
        pResult->m_pField = m_pField;
        pResult->pev->classname = MAKE_STRING("inout_register");

        //        ALERT(at_console, "adding; max %.2f %.2f %.2f, min %.2f %.2f %.2f is inside max %.2f %.2f %.2f, min %.2f %.2f %.2f\n", pResult->m_hValue->pev->absmax.x, pResult->m_hValue->pev->absmax.y, pResult->m_hValue->pev->absmax.z, pResult->m_hValue->pev->absmin.x, pResult->m_hValue->pev->absmin.y, pResult->m_hValue->pev->absmin.z, pResult->m_pField->pev->absmax.x, pResult->m_pField->pev->absmax.y, pResult->m_pField->pev->absmax.z, pResult->m_pField->pev->absmin.x, pResult->m_pField->pev->absmin.y, pResult->m_pField->pev->absmin.z); //LRCT

        m_pField->FireOnEntry(pValue);
        return pResult;
    }
}

CInOutRegister* CInOutRegister::Prune()
{
    if (m_hValue)
    {
        ASSERTSZ(m_pNext != NULL, "invalid InOut registry terminator\n");
        if (m_pField->Intersects(m_hValue))
        {
            // this entity is still inside the field, do nothing
            m_pNext = m_pNext->Prune();
            return this;
        }
        else
        {
            //            ALERT(at_console, "removing; max %.2f %.2f %.2f, min %.2f %.2f %.2f is outside max %.2f %.2f %.2f, min %.2f %.2f %.2f\n", m_hValue->pev->absmax.x, m_hValue->pev->absmax.y, m_hValue->pev->absmax.z, m_hValue->pev->absmin.x, m_hValue->pev->absmin.y, m_hValue->pev->absmin.z, m_pField->pev->absmax.x, m_pField->pev->absmax.y, m_pField->pev->absmax.z, m_pField->pev->absmin.x, m_pField->pev->absmin.y, m_pField->pev->absmin.z); //LRCT

            // this entity has just left the field, trigger
            m_pField->FireOnLeaving(m_hValue);
            SetThink(&CInOutRegister::SUB_Remove);
            SetNextThink(0.1);
            return m_pNext->Prune();
        }
    }
    else
    {
        // this register has a missing or null value
        if (m_pNext)
        {
            // this is an invalid list entry, remove it
            SetThink(&CInOutRegister::SUB_Remove);
            SetNextThink(0.1);
            return m_pNext->Prune();
        }
        else
        {
            // this is the list terminator, leave it.
            return this;
        }
    }
}

CBaseEntity* CInOutRegister::GetFirstEntityFrom(CBaseEntity* pStartEntity)
{
    CBaseEntity* result = NULL;
    int startOffset;
    if (pStartEntity)
    {
        startOffset = OFFSET(pStartEntity->pev);
    }
    int resultOffset = 0;

    for (CInOutRegister* current = this; current != NULL; current = current->m_pNext)
    {
        if (current->m_hValue != NULL)
        {
            int testOffset = OFFSET(current->m_hValue->pev);
            if ((pStartEntity == NULL || testOffset > startOffset) && (result == NULL || resultOffset > testOffset))
            {
                result = current->m_hValue;
                resultOffset = testOffset;
            }
        }
    }

    return result;
}
