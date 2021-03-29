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

//=========================================================
// CWeaponBox - a single entity that can store weapons
// and ammo. 
//=========================================================
class CWeaponBox : public CBaseEntity
{
    void Precache() override;
    void Spawn() override;
    void Touch(CBaseEntity* pOther) override;
    void KeyValue(KeyValueData* pkvd) override;
    BOOL IsEmpty();
    int GiveAmmo(int iCount, const char* szName, int iMax, int* pIndex = NULL);
    void SetObjectCollisionBox() override;

public:
    void DLLEXPORT Kill();
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    BOOL HasWeapon(CBasePlayerItem* pCheckItem);
    BOOL PackWeapon(CBasePlayerItem* pWeapon);
    BOOL PackAmmo(int iszName, int iCount);

    CBasePlayerItem* m_rgpPlayerItems[MAX_ITEM_TYPES]; // one slot for each 

    int m_rgiszAmmo[MAX_AMMO_SLOTS]; // ammo names
    int m_rgAmmo[MAX_AMMO_SLOTS]; // ammo quantities

    int m_cAmmoTypes; // how many ammo types packed into this box (if packed by a level designer)
};
