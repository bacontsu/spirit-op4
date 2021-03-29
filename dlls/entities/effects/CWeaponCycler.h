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

#include "entities/weapons/CBasePlayerWeapon.h"

//Weapon Cycler
class CWeaponCycler : public CBasePlayerWeapon
{
public:
    void Spawn() override;
    int iItemSlot() override { return 1; }
    int GetItemInfo(ItemInfo* p) override { return 0; }

    void PrimaryAttack() override;
    void SecondaryAttack() override;
    BOOL Deploy() override;
    void Holster(int skiplocal = 0) override;
    int m_iszModel;
    int m_iModel;
};
