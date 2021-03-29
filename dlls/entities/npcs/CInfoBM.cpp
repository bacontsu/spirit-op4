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

#include "CInfoBM.h"

LINK_ENTITY_TO_CLASS(info_bigmomma, CInfoBM);

TYPEDESCRIPTION CInfoBM::m_SaveData[] =
{
    DEFINE_FIELD(CInfoBM, m_preSequence, FIELD_STRING),
};

IMPLEMENT_SAVERESTORE(CInfoBM, CPointEntity);

void CInfoBM::Spawn()
{
}

void CInfoBM::KeyValue(KeyValueData* pkvd)
{
    if (FStrEq(pkvd->szKeyName, "radius"))
    {
        pev->scale = atof(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "reachdelay"))
    {
        pev->speed = atof(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "reachtarget"))
    {
        pev->message = ALLOC_STRING(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "reachsequence"))
    {
        pev->netname = ALLOC_STRING(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "presequence"))
    {
        m_preSequence = ALLOC_STRING(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else
        CPointEntity::KeyValue(pkvd);
}
