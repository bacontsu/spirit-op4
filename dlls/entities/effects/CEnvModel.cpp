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

#include "CEnvModel.h"

LINK_ENTITY_TO_CLASS(env_model, CEnvModel);

TYPEDESCRIPTION CEnvModel::m_SaveData[] =
{
    DEFINE_FIELD(CEnvModel, m_iszSequence_On, FIELD_STRING),
    DEFINE_FIELD(CEnvModel, m_iszSequence_Off, FIELD_STRING),
    DEFINE_FIELD(CEnvModel, m_iAction_On, FIELD_INTEGER),
    DEFINE_FIELD(CEnvModel, m_iAction_Off, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CEnvModel, CBaseAnimating);

void CEnvModel::KeyValue(KeyValueData* pkvd)
{
    if (FStrEq(pkvd->szKeyName, "m_iszSequence_On"))
    {
        m_iszSequence_On = ALLOC_STRING(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iszSequence_Off"))
    {
        m_iszSequence_Off = ALLOC_STRING(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iAction_On"))
    {
        m_iAction_On = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "m_iAction_Off"))
    {
        m_iAction_Off = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else
    {
        CBaseAnimating::KeyValue(pkvd);
    }
}

void CEnvModel::Spawn()
{
    Precache();
    SET_MODEL(ENT(pev), STRING(pev->model));
    UTIL_SetOrigin(this, pev->origin);

    //    UTIL_AssignOrigin(this, pev->oldorigin); //AJH - WTF is this here for?

    if (pev->spawnflags & SF_ENVMODEL_SOLID)
    {
        pev->solid = SOLID_SLIDEBOX;
        UTIL_SetSize(pev, Vector(-10, -10, -10), Vector(10, 10, 10)); //LRCT
    }

    if (pev->spawnflags & SF_ENVMODEL_DROPTOFLOOR)
    {
        pev->origin.z += 1;
        DROP_TO_FLOOR(ENT(pev));
    }
    SetBoneController(0, 0);
    SetBoneController(1, 0);

    SetSequence();

    SetNextThink(0.1);
}

void CEnvModel::Precache()
{
    PRECACHE_MODEL((char*)STRING(pev->model));
}

STATE CEnvModel::GetState()
{
    if (pev->spawnflags & SF_ENVMODEL_OFF)
        return STATE_OFF;
    else
        return STATE_ON;
}

void CEnvModel::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
    if (ShouldToggle(useType, !(pev->spawnflags & SF_ENVMODEL_OFF)))
    {
        if (pev->spawnflags & SF_ENVMODEL_OFF)
            pev->spawnflags &= ~SF_ENVMODEL_OFF;
        else
            pev->spawnflags |= SF_ENVMODEL_OFF;

        SetSequence();
        SetNextThink(0.1);
    }
}

void CEnvModel::Think()
{
    int iTemp;

    //    ALERT(at_console, "env_model Think fr=%f\n", pev->framerate);

    StudioFrameAdvance(); // set m_fSequenceFinished if necessary

    //    if (m_fSequenceLoops)
    //    {
    //        SetNextThink( 1E6 );
    //        return; // our work here is done.
    //    }
    if (m_fSequenceFinished && !m_fSequenceLoops)
    {
        if (pev->spawnflags & SF_ENVMODEL_OFF)
            iTemp = m_iAction_Off;
        else
            iTemp = m_iAction_On;

        switch (iTemp)
        {
            //        case 1: // loop
            //            pev->animtime = gpGlobals->time;
            //            m_fSequenceFinished = FALSE;
            //            m_flLastEventCheck = gpGlobals->time;
            //            pev->frame = 0;
            //            break;
        case 2: // change state
            if (pev->spawnflags & SF_ENVMODEL_OFF)
                pev->spawnflags &= ~SF_ENVMODEL_OFF;
            else
                pev->spawnflags |= SF_ENVMODEL_OFF;
            SetSequence();
            break;
        default: //remain frozen
            return;
        }
    }
    SetNextThink(0.1);
}

void CEnvModel::SetSequence()
{
    int iszSeq;

    if (pev->spawnflags & SF_ENVMODEL_OFF)
        iszSeq = m_iszSequence_Off;
    else
        iszSeq = m_iszSequence_On;

    if (!iszSeq)
        return;
    pev->sequence = LookupSequence(STRING(iszSeq));

    if (pev->sequence == -1)
    {
        if (pev->targetname)
            ALERT(at_error, "env_model %s: unknown sequence \"%s\"\n", STRING(pev->targetname), STRING(iszSeq));
        else
            ALERT(at_error, "env_model: unknown sequence \"%s\"\n", STRING(pev->targetname), STRING(iszSeq));
        pev->sequence = 0;
    }

    pev->frame = 0;
    ResetSequenceInfo();

    if (pev->spawnflags & SF_ENVMODEL_OFF)
    {
        if (m_iAction_Off == 1)
            m_fSequenceLoops = 1;
        else
            m_fSequenceLoops = 0;
    }
    else
    {
        if (m_iAction_On == 1)
            m_fSequenceLoops = 1;
        else
            m_fSequenceLoops = 0;
    }
}
