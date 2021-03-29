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

//=========================================================
// barnacle - stationary ceiling mounted 'fishing' monster
//=========================================================
class CBarnacle : public CBaseMonster
{
public:
    void Spawn() override;
    void Precache() override;
    CBaseEntity* TongueTouchEnt(float* pflLength);
    int Classify() override;
    void HandleAnimEvent(MonsterEvent_t* pEvent) override;
    void DLLEXPORT BarnacleThink();
    void DLLEXPORT WaitTillDead();
    void Killed(entvars_t* pevAttacker, int iGib) override;
    int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    float m_flAltitude;
    float m_flKillVictimTime;
    int m_cGibs; // barnacle loads up on gibs each time it kills something.
    BOOL m_fTongueExtended;
    BOOL m_fLiftingPrey;
    float m_flTongueAdj;
};
