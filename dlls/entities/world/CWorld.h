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

#define SF_WORLD_DARK         0x0001        // Fade from black at startup
#define SF_WORLD_TITLE        0x0002        // Display game title at startup
#define SF_WORLD_FORCETEAM    0x0004        // Force teams
//#define SF_WORLD_STARTSUIT    0x0008        // LRC- Start this level with an HEV suit!

//=======================
// CWorld
//
// This spawns first when each level begins.
//=======================
class CWorld : public CBaseEntity
{
public:
    void Spawn() override;
    void Precache() override;
    void KeyValue(KeyValueData* pkvd) override;

    CBaseMutableAlias* m_pFirstAlias;
};

extern CWorld* g_pWorld;
