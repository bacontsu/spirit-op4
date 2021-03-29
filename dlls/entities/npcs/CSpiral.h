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

// Spiral Effect
class CSpiral : public CBaseEntity
{
public:
    void Spawn() override;
    void Think() override;
    int ObjectCaps() override { return FCAP_DONT_SAVE; }
    static CSpiral* Create(const Vector& origin, float height, float radius, float duration);
};
