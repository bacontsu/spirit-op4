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

#include "entities/CBaseEntity.h"

#define SF_MONSTERTARGET_OFF 1

//LRC - an entity for monsters to shoot at.
class CMonsterTarget : public CBaseEntity
{
public:
    void Spawn(void) override;
    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
    int Classify(void) override { return pev->frags; }

    STATE GetState(void) override
    {
        return pev->health ? STATE_ON : STATE_OFF;
    }
};
