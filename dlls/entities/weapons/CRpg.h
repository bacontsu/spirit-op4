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
#include "CLaserSpot.h"

class CRpg : public CBasePlayerWeapon
{
public:

#ifndef CLIENT_DLL
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];
#endif

    void Spawn(void) override;
    void Precache(void) override;
    void Reload(void) override;
    int GetItemInfo(ItemInfo* p) override;
    int AddToPlayer(CBasePlayer* pPlayer) override;

    BOOL Deploy(void) override;
    BOOL CanHolster(void) override;
    void Holster(int skiplocal = 0) override;

    void PrimaryAttack(void) override;
    void SecondaryAttack(void) override;
    void WeaponIdle(void) override;
    void UpdateSpot(void);
    BOOL ShouldWeaponIdle(void) override { return TRUE; }

    CLaserSpot* m_pSpot;
    int m_fSpotActive;
    int m_cActiveRockets; // how many missiles in flight from this launcher right now?

    BOOL UseDecrement(void) override
    {
#if defined( CLIENT_WEAPONS )
        return TRUE;
#else
        return FALSE;
#endif
    }

private:
    unsigned short m_usRpg;
};
