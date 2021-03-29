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

#include "entities/base/CPointEntity.h"

//
// MultiSouce
//

#define MAX_MULTI_TARGETS 16 // maximum number of targets a single multi_manager entity may be assigned.
#define MS_MAX_TARGETS    32

#define SF_MULTI_FIREONCLOSE    1
#define SF_MULTI_INIT            2

class CMultiSource : public CPointEntity
{
public:
    void Spawn() override;
    void KeyValue(KeyValueData* pkvd) override;
    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
    STATE GetState() override;
    void DLLEXPORT Register();

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;

    static TYPEDESCRIPTION m_SaveData[];

    EHANDLE m_rgEntities[MS_MAX_TARGETS];
    int m_rgTriggered[MS_MAX_TARGETS];

    int m_iTotal;
    string_t m_globalstate;
};
