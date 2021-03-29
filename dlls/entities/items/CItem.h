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

// constant items
#define ITEM_HEALTHKIT		1
#define ITEM_ANTIDOTE		2
#define ITEM_SECURITY		3
#define ITEM_BATTERY		4

class CItem : public CBaseEntity
{
public:
    void Spawn(void) override;
    CBaseEntity* Respawn(void) override;
    void DLLEXPORT ItemTouch(CBaseEntity* pOther);
    void DLLEXPORT Materialize(void);
    virtual BOOL MyTouch(CBasePlayer* pPlayer) { return FALSE; }
};
