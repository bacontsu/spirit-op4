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

#include "CTriggerRotTest.h"

#include "util/findentity.h"

LINK_ENTITY_TO_CLASS(trigger_rottest, CTriggerRotTest);

TYPEDESCRIPTION CTriggerRotTest::m_SaveData[] =
{
    DEFINE_FIELD(CTriggerRotTest, m_pMarker, FIELD_CLASSPTR),
    DEFINE_FIELD(CTriggerRotTest, m_pReference, FIELD_CLASSPTR),
    DEFINE_FIELD(CTriggerRotTest, m_pBridge, FIELD_CLASSPTR),
    DEFINE_FIELD(CTriggerRotTest, m_pHinge, FIELD_CLASSPTR),
};

IMPLEMENT_SAVERESTORE(CTriggerRotTest, CBaseDelay);

void CTriggerRotTest::PostSpawn()
{
    m_pMarker = UTIL_FindEntityByTargetname(NULL, STRING(pev->target));
    m_pReference = UTIL_FindEntityByTargetname(NULL, STRING(pev->netname));
    m_pBridge = UTIL_FindEntityByTargetname(NULL, STRING(pev->noise1));
    m_pHinge = UTIL_FindEntityByTargetname(NULL, STRING(pev->message));
    pev->armorvalue = 0; // initial angle
    if (pev->armortype == 0) //angle offset
        pev->armortype = 30;
    SetNextThink(1);
}

void CTriggerRotTest::Think()
{
    //    ALERT(at_console, "Using angle = %.2f\n", pev->armorvalue);
    if (m_pReference)
    {
        m_pReference->pev->origin = pev->origin;
        m_pReference->pev->origin.x = m_pReference->pev->origin.x + pev->health;
        //        ALERT(at_console, "Set Reference = %.2f %.2f %.2f\n", m_pReference->pev->origin.x, m_pReference->pev->origin.y, m_pReference->pev->origin.z);
    }
    if (m_pMarker)
    {
        Vector vecTemp = UTIL_AxisRotationToVec((m_pHinge->pev->origin - pev->origin).Normalize(), pev->armorvalue);
        m_pMarker->pev->origin = pev->origin + pev->health * vecTemp;

        //        ALERT(at_console, "vecTemp = %.2f %.2f %.2f\n", vecTemp.x, vecTemp.y, vecTemp.z);
        //        ALERT(at_console, "Set Marker = %.2f %.2f %.2f\n", m_pMarker->pev->origin.x, m_pMarker->pev->origin.y, m_pMarker->pev->origin.z);
    }
    if (m_pBridge)
    {
        Vector vecTemp = UTIL_AxisRotationToAngles((m_pHinge->pev->origin - pev->origin).Normalize(), pev->armorvalue);
        m_pBridge->pev->origin = pev->origin;
        m_pBridge->pev->angles = vecTemp;

        //        ALERT(at_console, "vecTemp = %.2f %.2f %.2f\n", vecTemp.x, vecTemp.y, vecTemp.z);
        //        ALERT(at_console, "Set Marker = %.2f %.2f %.2f\n", m_pMarker->pev->origin.x, m_pMarker->pev->origin.y, m_pMarker->pev->origin.z);
    }
    pev->armorvalue += pev->armortype * 0.1;
    SetNextThink(0.1);
}
