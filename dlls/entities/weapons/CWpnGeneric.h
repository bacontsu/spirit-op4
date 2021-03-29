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

class CWpnGeneric : public CBasePlayerWeapon
{
public:
    void Spawn() override;
    int GetItemInfo(ItemInfo* p) override;
    int AddToPlayer(CBasePlayer* pPlayer) override;

    void PrimaryAttack() override;
    void SecondaryAttack() override;
    BOOL Deploy() override;
    void Holster(int skiplocal = 0) override;
    void WeaponIdle() override;
    void Precache() override;
    void Reload() override;
private:
    int m_iShell;


    unsigned short m_usFireGen1; //Different type of shots
    unsigned short m_usFireGen2; //In next release may be extended. G-Cont.
    unsigned short m_usFireGen3;
public:
    /*    int m_fInZoom; // don't save this
        BOOL m_fNVG;//NightVision Status
        CLaserSpot *m_pSpot;
        int m_fSpotActive;
        void UpdateSpot();
    */
    BOOL UseDecrement() override
    {
#if defined( CLIENT_WEAPONS )
        return TRUE;
#else
    return FALSE;
#endif
    }
};
