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

#include "entities/CBaseMonster.h"

class CCycler : public CBaseMonster
{
public:
    void GenericCyclerSpawn(const char* szModel, Vector vecMin, Vector vecMax);
    int ObjectCaps() override { return (CBaseEntity::ObjectCaps() | FCAP_IMPULSE_USE); }
    int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
    void Spawn() override;
    void Think() override;
    //void Pain( float flDamage );
    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;

    // Don't treat as a live target
    BOOL IsAlive() override { return FALSE; }

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    int m_animate;
};
