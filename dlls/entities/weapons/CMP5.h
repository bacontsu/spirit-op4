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

class CMP5 : public CBasePlayerWeapon
{
public:
    void Spawn() override;
    void Precache() override;
    int GetItemInfo(ItemInfo* p) override;
    int AddToPlayer(CBasePlayer* pPlayer) override;

    void PrimaryAttack() override;
    void SecondaryAttack() override;
    BOOL Deploy() override;
    void Holster(int skiplocal = 0) override;
    void Reload() override;
    void WeaponIdle() override;
    float m_flNextAnimTime;
    int m_iShell;

    BOOL UseDecrement() override
    {
#if defined( CLIENT_WEAPONS )
        return TRUE;
#else
        return FALSE;
#endif
    }

private:
    unsigned short m_usMP5;
    unsigned short m_usMP52;
};
