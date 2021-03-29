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

// special deathmatch shotgun spreads
#define VECTOR_CONE_DM_SHOTGUN       Vector( 0.08716, 0.04362, 0.00 ) // 10 degrees by 5 degrees
#define VECTOR_CONE_DM_DOUBLESHOTGUN Vector( 0.17365, 0.04362, 0.00 ) // 20 degrees by 5 degrees

class CShotgun : public CBasePlayerWeapon
{
public:

#ifndef CLIENT_DLL
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];
#endif

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
    void ItemPostFrame() override;
    int m_fInReload;
    float m_flNextReload;
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
    unsigned short m_usDoubleFire;
    unsigned short m_usSingleFire;
};
