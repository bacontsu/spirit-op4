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

#include "entities/ammo/CShotgunAmmo.h"

LINK_ENTITY_TO_CLASS(ammo_buckshot, CShotgunAmmo);

void CShotgunAmmo::Spawn()
{
    Precache();
    SET_MODEL(ENT(pev), "models/w_shotbox.mdl");
    CBasePlayerAmmo::Spawn();
}

void CShotgunAmmo::Precache()
{
    PRECACHE_MODEL("models/w_shotbox.mdl");
    PRECACHE_SOUND("items/9mmclip1.wav");
}

BOOL CShotgunAmmo::AddAmmo(CBaseEntity* pOther)
{
    if (pOther->GiveAmmo(AMMO_BUCKSHOTBOX_GIVE, "buckshot", BUCKSHOT_MAX_CARRY) != -1)
    {
        EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
        return TRUE;
    }
    return FALSE;
}
