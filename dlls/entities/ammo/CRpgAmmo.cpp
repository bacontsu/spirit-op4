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

#include "entities/ammo/CRpgAmmo.h"

LINK_ENTITY_TO_CLASS(ammo_rpgclip, CRpgAmmo);

void CRpgAmmo::Spawn()
{
    Precache();
    SET_MODEL(ENT(pev), "models/w_rpgammo.mdl");
    CBasePlayerAmmo::Spawn();
}

void CRpgAmmo::Precache()
{
    PRECACHE_MODEL("models/w_rpgammo.mdl");
    PRECACHE_SOUND("items/9mmclip1.wav");
}

BOOL CRpgAmmo::AddAmmo(CBaseEntity* pOther)
{
    int iGive;

#ifdef CLIENT_DLL
        if (bIsMultiplayer())
#else
    if (g_pGameRules->IsMultiplayer())
#endif
    {
        // hand out more ammo per rocket in multiplayer.
        iGive = AMMO_RPGCLIP_GIVE * 2;
    }
    else
    {
        iGive = AMMO_RPGCLIP_GIVE;
    }

    if (pOther->GiveAmmo(iGive, "rockets", ROCKET_MAX_CARRY) != -1)
    {
        EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
        return TRUE;
    }
    return FALSE;
}
