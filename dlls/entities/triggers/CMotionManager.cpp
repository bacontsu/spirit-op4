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

#include "CMotionManager.h"
#include "CMotionThread.h"
#include "util/findentity.h"

LINK_ENTITY_TO_CLASS(motion_manager, CMotionManager);

TYPEDESCRIPTION    CMotionManager::m_SaveData[] =
{
    DEFINE_FIELD(CMotionManager, m_iszPosition, FIELD_STRING),
    DEFINE_FIELD(CMotionManager, m_iPosMode, FIELD_INTEGER),
    DEFINE_FIELD(CMotionManager, m_iszFacing, FIELD_STRING),
    DEFINE_FIELD(CMotionManager, m_iFaceMode, FIELD_INTEGER),
    DEFINE_FIELD(CMotionManager, m_iPosAxis, FIELD_INTEGER),//AJH
    DEFINE_FIELD(CMotionManager, m_iFaceAxis, FIELD_INTEGER),//AJH
    DEFINE_FIELD(CMotionManager, pThread, FIELD_CLASSPTR),//AJH
};

IMPLEMENT_SAVERESTORE(CMotionManager, CPointEntity);

void CMotionManager::KeyValue(KeyValueData* pkvd)
{
    if (FStrEq(pkvd->szKeyName, "m_iszPosition"))
    {
        m_iszPosition = ALLOC_STRING(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iPosMode"))
    {
        m_iPosMode = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iPosAxis"))//AJH
    {
        m_iPosAxis = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iszFacing"))
    {
        m_iszFacing = ALLOC_STRING(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iFaceMode"))
    {
        m_iFaceMode = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iFaceAxis"))//AJH
    {
        m_iFaceAxis = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else
        CPointEntity::KeyValue(pkvd);
}

void CMotionManager::PostSpawn()
{
    pThread = GetClassPtr((CMotionThread*)NULL);
    if (FStringNull(pev->targetname))
        Use(this, this, USE_ON, 0);

}

void CMotionManager::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{

    if (useType == USE_OFF) {        //AJH to allow motion_managers to turn off
        if (pThread != NULL)        //
            pThread->m_hLocus = NULL;    //
    }
    else {

        if (pThread == NULL) //AJH we shouldn't need this but for some reason the pointer is ALWAYS NULL after save/restore.
        {
            pThread = GetClassPtr((CMotionThread*)NULL);
            //ALERT(at_debug,"Motion_manager motion thread pointer is NULL, Creating new motionthread.\n");
        }
        CBaseEntity* pTarget = pActivator;
        if (pev->target) {
            pTarget = UTIL_FindEntityByTargetname(NULL, STRING(pev->target), pActivator);
            if (pTarget == NULL)
                ALERT(at_error, "motion_manager \"%s\" can't find entity \"%s\" to affect\n", STRING(pev->targetname), STRING(pev->target));
            else {
                do {
                    Affect(pTarget, pActivator);
                    pTarget = UTIL_FindEntityByTargetname(pTarget, STRING(pev->target), pActivator);
                } while (pTarget);
            }
        }
    }
}

void CMotionManager::Affect(CBaseEntity* pTarget, CBaseEntity* pActivator)
{
    if (pev->spawnflags & SF_MOTION_DEBUG)
        ALERT(at_debug, "DEBUG: Creating MotionThread for %s \"%s\"\n", STRING(pTarget->pev->classname), STRING(pTarget->pev->targetname));

    if (pThread == NULL)
    {
        ALERT(at_debug, "Motion_manager motion thread pointer is NULL!!\n");
        return; //error?
    }
    pThread->m_hLocus = pActivator;
    pThread->m_hTarget = pTarget;
    pThread->m_iszPosition = m_iszPosition;
    pThread->m_iPosMode = m_iPosMode;
    pThread->m_iPosAxis = m_iPosAxis;    //AJH
    pThread->m_iszFacing = m_iszFacing;
    pThread->m_iFaceMode = m_iFaceMode;
    pThread->m_iFaceAxis = m_iFaceAxis;    //AJH
    pThread->pev->spawnflags = pev->spawnflags;
    pThread->SetNextThink(0);
}
