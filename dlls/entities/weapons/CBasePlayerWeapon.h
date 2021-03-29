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

#include "entities/player/CBasePlayerItem.h"

// inventory items that 
class CBasePlayerWeapon : public CBasePlayerItem
{
public:
    static TYPEDESCRIPTION m_SaveData[];
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;

    // generic weapon versions of CBasePlayerItem calls
    int AddToPlayer(CBasePlayer* pPlayer) override;
    int AddDuplicate(CBasePlayerItem* pItem) override;


    virtual int ExtractAmmo(CBasePlayerWeapon* pWeapon); //{ return TRUE; };            // Return TRUE if you can add ammo to yourself when picked up
    virtual int ExtractClipAmmo(CBasePlayerWeapon* pWeapon); // { return TRUE; };            // Return TRUE if you can add ammo to yourself when picked up

    virtual int AddWeapon(void)
    {
        ExtractAmmo(this);
        return TRUE;
    } // Return TRUE if you want to add yourself to the player

    // generic "shared" ammo handlers
    BOOL AddPrimaryAmmo(int iCount, char* szName, int iMaxClip, int iMaxCarry);
    BOOL AddSecondaryAmmo(int iCount, char* szName, int iMaxCarry);

    void UpdateItemInfo(void) override
    {
    } // updates HUD state

    int m_iPlayEmptySound;
    int m_fFireOnEmpty; // True when the gun is empty and the player is still holding down the
    // attack key(s)
    virtual BOOL PlayEmptySound(void);
    virtual void ResetEmptySound(void);

    virtual void SendWeaponAnim(int iAnim, int skiplocal = 1, int body = 0); // skiplocal is 1 if client is predicting weapon animations

    BOOL CanDeploy(void) override;
    virtual BOOL IsUseable(void);
    BOOL DefaultDeploy(const char* szViewModel, const char* szWeaponModel, int iAnim, const char* szAnimExt, int skiplocal = 0, int body = 0);
    int DefaultReload(int iClipSize, int iAnim, float fDelay, int body = 0);

    void ItemPostFrame(void) override; // called each frame by the player PostThink
    // called by CBasePlayerWeapons ItemPostFrame()
    virtual void PrimaryAttack(void) {} // do "+ATTACK"
    virtual void SecondaryAttack(void) {} // do "+ATTACK2"
    virtual void Reload(void) {} // do "+RELOAD"
    virtual void WeaponIdle(void) {} // called when no buttons pressed
    int UpdateClientData(CBasePlayer* pPlayer) override; // sends hud info to client dll, if things have changed
    virtual void RetireWeapon(void);
    virtual BOOL ShouldWeaponIdle(void) { return FALSE; }
    void Holster(int skiplocal = 0) override;
    virtual BOOL UseDecrement(void) { return FALSE; }
    virtual void OnAmmoOrClipChanged() {} // Called when ammo is forcably changed (e.g. by player_weaponstrip)

    int PrimaryAmmoIndex() override;
    int SecondaryAmmoIndex() override;

    void PrintState(void);

    CBasePlayerItem* GetWeaponPtr(void) override { return (CBasePlayerItem*)this; }
    float GetNextAttackDelay(float delay);

    float m_flPumpTime;
    int m_fInSpecialReload; // Are we in the middle of a reload for the shotguns
    float m_flNextPrimaryAttack; // soonest time ItemPostFrame will call PrimaryAttack
    float m_flNextSecondaryAttack; // soonest time ItemPostFrame will call SecondaryAttack
    float m_flTimeWeaponIdle; // soonest time ItemPostFrame will call WeaponIdle
    int m_iPrimaryAmmoType; // "primary" ammo index into players m_rgAmmo[]
    int m_iSecondaryAmmoType; // "secondary" ammo index into players m_rgAmmo[]
    int m_iClip; // number of shots left in the primary weapon clip, -1 it not used
    int m_iClientClip; // the last version of m_iClip sent to hud dll
    int m_iClientWeaponState; // the last version of the weapon state sent to hud dll (is current weapon, is on target)
    int m_fInReload; // Are we in the middle of a reload;
    int m_iClipSize; //This required weapon_generic, defintion in same class will crash'es compile

    int m_iDefaultAmmo; // how much ammo you get when you pick up this weapon as placed by a level designer.

    // hle time creep vars
    float m_flPrevPrimaryAttack;
    float m_flLastFireTime;
};
