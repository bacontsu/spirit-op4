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

#define BOLT_AIR_VELOCITY    2000
#define BOLT_WATER_VELOCITY  1000

class CCrossbow : public CBasePlayerWeapon
{
public:
    void Spawn() override;
    void Precache() override;
    int GetItemInfo(ItemInfo* p) override;

    void FireBolt();
    void FireSniperBolt();
    void PrimaryAttack() override;
    void SecondaryAttack() override;
    int AddToPlayer(CBasePlayer* pPlayer) override;
    BOOL Deploy() override;
    void Holster(int skiplocal = 0) override;
    void Reload() override;
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
    unsigned short m_usCrossbow;
    unsigned short m_usCrossbow2;
};
