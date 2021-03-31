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
* Valve LLC.  All other use, distribution, or modification is prohibited
* without written permission from Valve LLC.
*
****/

#include "CBasePlayerWeapon.h"
#include "gamerules/CGameRules.h"
#include "entities/player/CBasePlayer.h"

extern int gmsgCurWeapon;

TYPEDESCRIPTION CBasePlayerWeapon::m_SaveData[] =
{
#if defined( CLIENT_WEAPONS )
    DEFINE_FIELD(CBasePlayerWeapon, m_flNextPrimaryAttack, FIELD_FLOAT),
    DEFINE_FIELD(CBasePlayerWeapon, m_flNextSecondaryAttack, FIELD_FLOAT),
    DEFINE_FIELD(CBasePlayerWeapon, m_flTimeWeaponIdle, FIELD_FLOAT),
#else    // CLIENT_WEAPONS
    DEFINE_FIELD(CBasePlayerWeapon, m_flNextPrimaryAttack, FIELD_TIME),
    DEFINE_FIELD(CBasePlayerWeapon, m_flNextSecondaryAttack, FIELD_TIME),
    DEFINE_FIELD(CBasePlayerWeapon, m_flTimeWeaponIdle, FIELD_TIME),
#endif    // CLIENT_WEAPONS
    DEFINE_FIELD(CBasePlayerWeapon, m_iPrimaryAmmoType, FIELD_INTEGER),
    DEFINE_FIELD(CBasePlayerWeapon, m_iSecondaryAmmoType, FIELD_INTEGER),
    DEFINE_FIELD(CBasePlayerWeapon, m_iClip, FIELD_INTEGER),
    DEFINE_FIELD(CBasePlayerWeapon, m_iDefaultAmmo, FIELD_INTEGER),
    //    DEFINE_FIELD( CBasePlayerWeapon, m_iClientClip, FIELD_INTEGER )     , reset to zero on load so hud gets updated correctly
    //  DEFINE_FIELD( CBasePlayerWeapon, m_iClientWeaponState, FIELD_INTEGER ), reset to zero on load so hud gets updated correctly
};

IMPLEMENT_SAVERESTORE(CBasePlayerWeapon, CBasePlayerItem);

int CBasePlayerWeapon::AddToPlayer(CBasePlayer* pPlayer)
{
    if (!UTIL_IsMasterTriggered(m_sMaster, pPlayer)) //
        return FALSE; // AJH allows for locked weapons

    int bResult = CBasePlayerItem::AddToPlayer(pPlayer);

    pPlayer->pev->weapons |= (1 << m_iId);

    if (!m_iPrimaryAmmoType)
    {
        m_iPrimaryAmmoType = pPlayer->GetAmmoIndex(pszAmmo1());
        m_iSecondaryAmmoType = pPlayer->GetAmmoIndex(pszAmmo2());
    }


    if (bResult)
    {
        return AddWeapon();
    }
    return FALSE;
}

// CALLED THROUGH the newly-touched weapon's instance. The existing player weapon is pOriginal
int CBasePlayerWeapon::AddDuplicate(CBasePlayerItem* pOriginal)
{
    if (!UTIL_IsMasterTriggered(m_sMaster, m_pPlayer)) //
        return FALSE; // AJH allows for locked weapons 

    if (m_iDefaultAmmo)
    {
        return ExtractAmmo((CBasePlayerWeapon*)pOriginal);
    }
    else
    {
        // a dead player dropped this.
        return ExtractClipAmmo((CBasePlayerWeapon*)pOriginal);
    }
}

//=========================================================
// called by the new item with the existing item as parameter
//
// if we call ExtractAmmo(), it's because the player is picking up this type of weapon for 
// the first time. If it is spawned by the world, m_iDefaultAmmo will have a default ammo amount in it.
// if  this is a weapon dropped by a dying player, has 0 m_iDefaultAmmo, which means only the ammo in 
// the weapon clip comes along. 
//=========================================================
int CBasePlayerWeapon::ExtractAmmo(CBasePlayerWeapon* pWeapon)
{
    int iReturn = 0;

    if (pszAmmo1() != NULL)
    {
        // blindly call with m_iDefaultAmmo. It's either going to be a value or zero. If it is zero,
        // we only get the ammo in the weapon's clip, which is what we want. 
        iReturn = pWeapon->AddPrimaryAmmo(m_iDefaultAmmo, (char*)pszAmmo1(), iMaxClip(), iMaxAmmo1());
        m_iDefaultAmmo = 0;
    }

    if (pszAmmo2() != NULL)
    {
        iReturn |= pWeapon->AddSecondaryAmmo(0, (char*)pszAmmo2(), iMaxAmmo2());
    }

    return iReturn;
}

//=========================================================
// called by the new item's class with the existing item as parameter
//=========================================================
int CBasePlayerWeapon::ExtractClipAmmo(CBasePlayerWeapon* pWeapon)
{
    int iAmmo;

    if (m_iClip == WEAPON_NOCLIP)
    {
        iAmmo = 0; // guns with no clips always come empty if they are second-hand
    }
    else
    {
        iAmmo = m_iClip;
    }

    return pWeapon->m_pPlayer->GiveAmmo(iAmmo, pszAmmo1(), iMaxAmmo1()); // , &m_iPrimaryAmmoType
}

BOOL CBasePlayerWeapon::AddPrimaryAmmo(int iCount, char* szName, int iMaxClip, int iMaxCarry)
{
    int iIdAmmo;

    if (iMaxClip < 1)
    {
        m_iClip = -1;
        iIdAmmo = m_pPlayer->GiveAmmo(iCount, szName, iMaxCarry);
    }
    else if (m_iClip == 0)
    {
        int i;
        i = V_min(m_iClip + iCount, iMaxClip) - m_iClip;
        m_iClip += i;
        iIdAmmo = m_pPlayer->GiveAmmo(iCount - i, szName, iMaxCarry);
    }
    else
    {
        iIdAmmo = m_pPlayer->GiveAmmo(iCount, szName, iMaxCarry);
    }

    // m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] = iMaxCarry; // hack for testing

    if (iIdAmmo > 0)
    {
        m_iPrimaryAmmoType = iIdAmmo;
        if (m_pPlayer->HasPlayerItem(this))
        {
            // play the "got ammo" sound only if we gave some ammo to a player that already had this gun.
            // if the player is just getting this gun for the first time, DefaultTouch will play the "picked up gun" sound for us.
            EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
        }
    }

    return iIdAmmo > 0 ? TRUE : FALSE;
}

BOOL CBasePlayerWeapon::AddSecondaryAmmo(int iCount, char* szName, int iMax)
{
    int iIdAmmo;

    iIdAmmo = m_pPlayer->GiveAmmo(iCount, szName, iMax);

    //m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] = iMax; // hack for testing

    if (iIdAmmo > 0)
    {
        m_iSecondaryAmmoType = iIdAmmo;
        EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
    }
    return iIdAmmo > 0 ? TRUE : FALSE;
}

BOOL CBasePlayerWeapon::PlayEmptySound()
{
    if (m_iPlayEmptySound)
    {
        EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/357_cock1.wav", 0.8, ATTN_NORM);
        m_iPlayEmptySound = 0;
        return 0;
    }
    return 0;
}

void CBasePlayerWeapon::SendWeaponAnim(int iAnim, int skiplocal, int body)
{
    if (UseDecrement())
        skiplocal = 1;
    else
        skiplocal = 0;

    m_pPlayer->pev->weaponanim = iAnim;

#if defined( CLIENT_WEAPONS )
    if (skiplocal && ENGINE_CANSKIP(m_pPlayer->edict()))
        return;
#endif

    MESSAGE_BEGIN(MSG_ONE, SVC_WEAPONANIM, NULL, m_pPlayer->pev);
    WRITE_BYTE(iAnim); // sequence number
    WRITE_BYTE(pev->body); // weaponmodel bodygroup.
    MESSAGE_END();
}

//=========================================================
// IsUseable - this function determines whether or not a 
// weapon is useable by the player in its current state. 
// (does it have ammo loaded? do I have any ammo for the 
// weapon?, etc)
//=========================================================
BOOL CBasePlayerWeapon::IsUseable()
{
    return CanDeploy();
}

BOOL CBasePlayerWeapon::DefaultDeploy(const char* szViewModel, const char* szWeaponModel, int iAnim, const char* szAnimExt, int skiplocal /* = 0 */, int body)
{
    if (!CanDeploy())
        return FALSE;

    m_pPlayer->TabulateAmmo();
    m_pPlayer->pev->viewmodel = MAKE_STRING(szViewModel);
    m_pPlayer->pev->weaponmodel = MAKE_STRING(szWeaponModel);
    strcpy(m_pPlayer->m_szAnimExtention, szAnimExt);
    SendWeaponAnim(iAnim, skiplocal, body);

    m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
    m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0;
    m_flLastFireTime = 0.0;

    return TRUE;
}

int CBasePlayerWeapon::UpdateClientData(CBasePlayer* pPlayer)
{
    BOOL bSend = FALSE;
    int state = 0;
    if (pPlayer->m_pActiveItem == this)
    {
        if (pPlayer->m_fOnTarget)
            state = WEAPON_IS_ONTARGET;
        else
            state = 1;
    }

    // Forcing send of all data!
    if (!pPlayer->m_fWeapon)
    {
        bSend = TRUE;
    }

    // This is the current or last weapon, so the state will need to be updated
    if (this == pPlayer->m_pActiveItem ||
        this == pPlayer->m_pClientActiveItem)
    {
        if (pPlayer->m_pActiveItem != pPlayer->m_pClientActiveItem)
        {
            bSend = TRUE;
        }
    }

    // If the ammo, state, or fov has changed, update the weapon
    if (m_iClip != m_iClientClip ||
        state != m_iClientWeaponState ||
        pPlayer->m_iFOV != pPlayer->m_iClientFOV)
    {
        bSend = TRUE;
    }

    if (bSend)
    {
        MESSAGE_BEGIN(MSG_ONE, gmsgCurWeapon, NULL, pPlayer->pev);
        WRITE_BYTE(state);
        WRITE_BYTE(m_iId);
        WRITE_BYTE(m_iClip);
        MESSAGE_END();

        m_iClientClip = m_iClip;
        m_iClientWeaponState = state;
        pPlayer->m_fWeapon = TRUE;
    }

    if (m_pNext)
        m_pNext->UpdateClientData(pPlayer);

    return 1;
}

//=========================================================
// RetireWeapon - no more ammo for this gun, put it away.
//=========================================================
void CBasePlayerWeapon::RetireWeapon()
{
    Holster();
    // first, no viewmodel at all.
    m_pPlayer->pev->viewmodel = iStringNull;
    m_pPlayer->pev->weaponmodel = iStringNull;
    //m_pPlayer->pev->viewmodelindex = NULL;

    g_pGameRules->GetNextBestWeapon(m_pPlayer, this);

	//If we're still equipped and we couldn't switch to another weapon, dequip this one
	if (CanHolster() && m_pPlayer->m_pActiveItem == this)
	{
		m_pPlayer->SwitchWeapon(nullptr);
	}
}

void CBasePlayerWeapon::Holster(int skiplocal /* = 0 */)
{
    m_fInReload = FALSE; // cancel any reload in progress.
    m_pPlayer->pev->viewmodel = 0;
    m_pPlayer->pev->weaponmodel = 0;
}

//=========================================================
//=========================================================
int CBasePlayerWeapon::PrimaryAmmoIndex()
{
    return m_iPrimaryAmmoType;
}

//=========================================================
//=========================================================
int CBasePlayerWeapon::SecondaryAmmoIndex()
{
    return m_iSecondaryAmmoType;
}

void CBasePlayerWeapon::PrintState()
{
    ALERT(at_debug, "primary:  %f\n", m_flNextPrimaryAttack);
    ALERT(at_debug, "idle   :  %f\n", m_flTimeWeaponIdle);

    //    ALERT( at_debug, "nextrl :  %f\n", m_flNextReload );
    //    ALERT( at_debug, "nextpum:  %f\n", m_flPumpTime );

    //    ALERT( at_debug, "m_frt  :  %f\n", m_fReloadTime );
    ALERT(at_debug, "m_finre:  %i\n", m_fInReload);
    //    ALERT( at_debug, "m_finsr:  %i\n", m_fInSpecialReload );

    ALERT(at_debug, "m_iclip:  %i\n", m_iClip);
}

//=========================================================================
// GetNextAttackDelay - An accurate way of calcualting the next attack time.
//=========================================================================
float CBasePlayerWeapon::GetNextAttackDelay(float delay)
{
    if (m_flLastFireTime == 0 || m_flNextPrimaryAttack == -1)
    {
        // At this point, we are assuming that the client has stopped firing
        // and we are going to reset our book keeping variables.
        m_flLastFireTime = gpGlobals->time;
        m_flPrevPrimaryAttack = delay;
    }
    // calculate the time between this shot and the previous
    float flTimeBetweenFires = gpGlobals->time - m_flLastFireTime;
    float flCreep = 0.0f;
    if (flTimeBetweenFires > 0)
        flCreep = flTimeBetweenFires - m_flPrevPrimaryAttack; // postive or negative

    // save the last fire time
    m_flLastFireTime = gpGlobals->time;

    float flNextAttack = UTIL_WeaponTimeBase() + delay - flCreep;
    // we need to remember what the m_flNextPrimaryAttack time is set to for each shot, 
    // store it as m_flPrevPrimaryAttack.
    m_flPrevPrimaryAttack = flNextAttack - UTIL_WeaponTimeBase();
    //     char szMsg[256];
    //     _snprintf( szMsg, sizeof(szMsg), "next attack time: %0.4f\n", gpGlobals->time + flNextAttack );
    //     OutputDebugString( szMsg );
    return flNextAttack;
}
