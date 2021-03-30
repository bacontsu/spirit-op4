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
void EV_FirePython(struct event_args_s* args);
}

void EV_FirePython(event_args_t* args)
{
    int idx;
    Vector origin;
    Vector angles;
    Vector velocity;

    Vector vecSrc, vecAiming;
    Vector up, right, forward;
    float flSpread = 0.01;

    idx = args->entindex;
    VectorCopy(args->origin, origin);
    VectorCopy(args->angles, angles);
    VectorCopy(args->velocity, velocity);

    AngleVectors(angles, forward, right, up);

    if (EV_IsLocal(idx))
    {
        // Python uses different body in multiplayer versus single player
        int multiplayer = gEngfuncs.GetMaxClients() == 1 ? 0 : 1;

        // Add muzzle flash to current weapon model
        EV_MuzzleFlash();
        gEngfuncs.pEventAPI->EV_WeaponAnimation(PYTHON_FIRE1, multiplayer ? 1 : 0);

        V_PunchAxis(0, -10.0);
    }

    switch (gEngfuncs.pfnRandomLong(0, 1))
    {
    case 0:
        gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "weapons/357_shot1.wav", gEngfuncs.pfnRandomFloat(0.8, 0.9), ATTN_NORM, 0, PITCH_NORM);
        break;
    case 1:
        gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "weapons/357_shot2.wav", gEngfuncs.pfnRandomFloat(0.8, 0.9), ATTN_NORM, 0, PITCH_NORM);
        break;
    }

    EV_GetGunPosition(args, vecSrc, origin);

    VectorCopy(forward, vecAiming);

    EV_HLDM_FireBullets(idx, forward, right, up, 1, vecSrc, vecAiming, 8192, BULLET_PLAYER_357, 0, 0, args->fparam1, args->fparam2);
}
