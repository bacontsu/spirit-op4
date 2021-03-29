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
* Valve LLC.All other use, distribution, or modification is prohibited
* without written permission from Valve LLC.
*
****/
#pragma once

#include "CStateWatcher.h"

#define SF_WRCOUNT_FIRESTART  0x0001
#define SF_WRCOUNT_STARTED    0x8000

//AJH Heavily rewritten as it didn't work
class CWatcherCount : public CBaseToggle
{
public:
    int m_iMode;

    void KeyValue(KeyValueData* pkvd) override;

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    void Spawn(void) override;
    void DLLEXPORT Think(void) override;
    STATE GetState(void) override { return (pev->spawnflags & SF_SWATCHER_VALID) ? STATE_ON : STATE_OFF; }
    int ObjectCaps(void) override { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

    bool CalcNumber(CBaseEntity* pLocus, float* OUTresult) override
    {
        *OUTresult = pev->iuser1;
        return true;
    }
};
