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

#include "entities/CBaseAnimating.h"
#include "weapons.h"
#include "util/sound.h"

// Items that the player has in their inventory that they can use
class CBasePlayerItem : public CBaseAnimating
{
public:

    void SetObjectCollisionBox(void) override;
#ifndef CLIENT_DLL                              //AJH for lockable weapons
    void KeyValue(KeyValueData* pkvd) override; //
#endif                                          //

    static TYPEDESCRIPTION m_SaveData[];
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;

    virtual int AddToPlayer(CBasePlayer* pPlayer); // return TRUE if the item you want the item added to the player inventory
    virtual int AddDuplicate(CBasePlayerItem* pItem) { return FALSE; } // return TRUE if you want your duplicate removed from world
    void DLLEXPORT DestroyItem(void);
    void DLLEXPORT DefaultTouch(CBaseEntity* pOther); // default weapon touch
    void DLLEXPORT FallThink(void); // when an item is first spawned, this think is run to determine when the object has hit the ground.
    void DLLEXPORT Materialize(void); // make a weapon visible and tangible
    void DLLEXPORT AttemptToMaterialize(void); // the weapon desires to become visible and tangible, if the game rules allow for it
    CBaseEntity* Respawn(void) override; // copy a weapon
    void FallInit(void);
    void CheckRespawn(void);
    virtual int GetItemInfo(ItemInfo* p) { return 0; } // returns 0 if struct not filled out
    virtual BOOL CanDeploy(void) { return TRUE; }

    virtual BOOL Deploy() // returns is deploy was successful
    {
        return TRUE;
    }

    virtual BOOL CanHolster(void) { return TRUE; } // can this weapon be put away right now?
    virtual void Holster(int skiplocal = 0);
    virtual void UpdateItemInfo(void) {}

    virtual void ItemPreFrame(void) {} // called each frame by the player PreThink
    virtual void ItemPostFrame(void) {} // called each frame by the player PostThink

    virtual void Drop(void);
    virtual void Kill(void);
    virtual void AttachToPlayer(CBasePlayer* pPlayer);

    virtual int PrimaryAmmoIndex() { return -1; }
    virtual int SecondaryAmmoIndex() { return -1; }

    virtual int UpdateClientData(CBasePlayer* pPlayer) { return 0; }

    virtual CBasePlayerItem* GetWeaponPtr(void) { return NULL; }

    static ItemInfo ItemInfoArray[MAX_WEAPONS];
    static AmmoInfo AmmoInfoArray[MAX_AMMO_SLOTS];

    string_t m_sMaster; //AJH for lockable weapons

    CBasePlayer* m_pPlayer;
    CBasePlayerItem* m_pNext;
    int m_iId; // WEAPON_???
#ifndef CLIENT_DLL//AJH Test Debug
    void Spawn(void) override;
#endif//AJH
    virtual int iItemSlot(void)
    {
        ItemInfo II;
        if (GetItemInfo(&II))
            return II.iSlot + 1;
        else
            return 0; // return 0 to MAX_ITEMS_SLOTS, used in hud
    }

    int iItemPosition(void) { return ItemInfoArray[m_iId].iPosition; }
    const char* pszAmmo1(void) { return ItemInfoArray[m_iId].pszAmmo1; }
    int iMaxAmmo1(void) { return ItemInfoArray[m_iId].iMaxAmmo1; }
    const char* pszAmmo2(void) { return ItemInfoArray[m_iId].pszAmmo2; }
    int iMaxAmmo2(void) { return ItemInfoArray[m_iId].iMaxAmmo2; }
    const char* pszName(void) { return ItemInfoArray[m_iId].pszName; }
    int iMaxClip(void) { return ItemInfoArray[m_iId].iMaxClip; }
    int iWeight(void) { return ItemInfoArray[m_iId].iWeight; }
    int iFlags(void) { return ItemInfoArray[m_iId].iFlags; }

    // int        m_iIdPrimary;                                        // Unique Id for primary ammo
    // int        m_iIdSecondary;                                        // Unique Id for secondary ammo
};
