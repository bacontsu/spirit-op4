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

#include "CActAnimating.h"
#include "CXenTreeTrigger.h"
#include "CXenPLight.h"

#define TREE_AE_ATTACK 1

class CXenTree : public CActAnimating
{
public:
    void Spawn() override;
    void Precache() override;
    void Touch(CBaseEntity* pOther) override;
    void Think() override;

    int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override
    {
        Attack();
        return 0;
    }

    void HandleAnimEvent(MonsterEvent_t* pEvent) override;
    void Attack();
    int Classify() override { return CLASS_BARNACLE; }

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    static const char* pAttackHitSounds[];
    static const char* pAttackMissSounds[];

private:
    CXenTreeTrigger* m_pTrigger;
};
