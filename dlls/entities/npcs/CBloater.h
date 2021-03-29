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

class CBloater : public CBaseMonster
{
public:
    void Spawn() override;
    void Precache() override;
    void SetYawSpeed() override;
    int Classify() override;
    void HandleAnimEvent(MonsterEvent_t* pEvent) override;

    void PainSound() override;
    void AlertSound() override;
    void IdleSound() override;
    void AttackSnd();

    // No range attacks
    BOOL CheckRangeAttack1(float flDot, float flDist) override { return FALSE; }
    BOOL CheckRangeAttack2(float flDot, float flDist) override { return FALSE; }
    int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
};
