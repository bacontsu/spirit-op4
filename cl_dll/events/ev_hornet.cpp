/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

#include "ev_common.h"

extern "C" {
void EV_HornetGunFire(struct event_args_s* args);
}

void EV_HornetGunFire(event_args_t* args)
{
    int idx, iFireMode;
    Vector origin, angles, vecSrc, forward, right, up;

    idx = args->entindex;
    VectorCopy(args->origin, origin);
    VectorCopy(args->angles, angles);
    iFireMode = args->iparam1;

    //Only play the weapon anims if I shot it.
    if (EV_IsLocal(idx))
    {
        V_PunchAxis(0, gEngfuncs.pfnRandomLong(0, 2));
        gEngfuncs.pEventAPI->EV_WeaponAnimation(HGUN_SHOOT, 1);
    }

    switch (gEngfuncs.pfnRandomLong(0, 2))
    {
    case 0:
        gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "agrunt/ag_fire1.wav", 1, ATTN_NORM, 0, 100);
        break;
    case 1:
        gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "agrunt/ag_fire2.wav", 1, ATTN_NORM, 0, 100);
        break;
    case 2:
        gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "agrunt/ag_fire3.wav", 1, ATTN_NORM, 0, 100);
        break;
    }
}
