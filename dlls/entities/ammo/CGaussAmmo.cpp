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

#include "entities/ammo/CGaussAmmo.h"

LINK_ENTITY_TO_CLASS(ammo_gaussclip, CGaussAmmo);

void CGaussAmmo::Spawn()
{
    Precache();
    SET_MODEL(ENT(pev), "models/w_gaussammo.mdl");
    CBasePlayerAmmo::Spawn();
}

void CGaussAmmo::Precache()
{
    PRECACHE_MODEL("models/w_gaussammo.mdl");
    PRECACHE_SOUND("items/9mmclip1.wav");
}

BOOL CGaussAmmo::AddAmmo(CBaseEntity* pOther)
{
    if (pOther->GiveAmmo(AMMO_URANIUMBOX_GIVE, "uranium", URANIUM_MAX_CARRY) != -1)
    {
        EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);
        return TRUE;
    }
    return FALSE;
}
