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

#include "CBasePlayerWeapon.h"

enum tripmine_e {
    TRIPMINE_IDLE1 = 0,
    TRIPMINE_IDLE2,
    TRIPMINE_ARM1,
    TRIPMINE_ARM2,
    TRIPMINE_FIDGET,
    TRIPMINE_HOLSTER,
    TRIPMINE_DRAW,
    TRIPMINE_WORLD,
    TRIPMINE_GROUND,
};

#define TRIPMINE_PRIMARY_VOLUME 450

class CTripmine : public CBasePlayerWeapon
{
public:
    void Spawn() override;
    void Precache() override;
    int GetItemInfo(ItemInfo* p) override;

    void SetObjectCollisionBox() override
    {
        //!!!BUGBUG - fix the model!
        pev->absmin = pev->origin + Vector(-16, -16, -5);
        pev->absmax = pev->origin + Vector(16, 16, 28);
    }

    void PrimaryAttack() override;
    BOOL Deploy() override;
    void Holster(int skiplocal = 0) override;
    void WeaponIdle() override;

    BOOL UseDecrement() override
    {
#if defined( CLIENT_WEAPONS )
        return TRUE;
#else
        return FALSE;
#endif
    }

private:
    unsigned short m_usTripFire;
};
