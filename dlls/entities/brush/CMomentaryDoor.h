/***
*
*    Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*    This product contains software technology licensed from Id
*    Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*    All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#pragma once

#include "entities/CBaseToggle.h"
#include "CBaseDoor.h"

class CMomentaryDoor : public CBaseToggle
{
public:
    void Spawn() override;
    void Precache() override;

    void KeyValue(KeyValueData* pkvd) override;
    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
    int ObjectCaps() override;

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    void DLLEXPORT DoorMoveDone();

    BYTE m_bMoveSnd; // sound a door makes while moving    
    BYTE m_bStopSnd; // sound a door makes while moving

    STATE m_iState;
    float m_fLastPos;

    STATE GetState() override;

    bool CalcNumber(CBaseEntity* pLocus, float* OUTresult) override;
};
