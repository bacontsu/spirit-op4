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

class CFuncMortarField : public CBaseToggle
{
public:
    void Spawn() override;
    void Precache() override;
    void KeyValue(KeyValueData* pkvd) override;

    // Bmodels don't go across transitions
    int ObjectCaps() override;

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;

    static TYPEDESCRIPTION m_SaveData[];

    void DLLEXPORT FieldUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

    int m_iszXController;
    int m_iszYController;
    float m_flSpread;
    float m_flDelay;
    int m_iCount;
    int m_fControl;
};
