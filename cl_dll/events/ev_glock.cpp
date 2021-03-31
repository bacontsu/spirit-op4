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

void EV_FireGlock1(event_args_t* args)
{
    int idx;
    Vector origin;
    Vector angles;
    Vector velocity;
    int empty;

    Vector ShellVelocity;
    Vector ShellOrigin;
    int shell;
    Vector vecSrc, vecAiming;
    Vector up, right, forward;

    idx = args->entindex;
    VectorCopy(args->origin, origin);
    VectorCopy(args->angles, angles);
    VectorCopy(args->velocity, velocity);

    empty = args->bparam1;
    AngleVectors(angles, forward, right, up);

    shell = gEngfuncs.pEventAPI->EV_FindModelIndex("models/shell.mdl"); // brass shell

    if (EV_IsLocal(idx))
    {
        EV_MuzzleFlash();
        gEngfuncs.pEventAPI->EV_WeaponAnimation(empty ? GLOCK_SHOOT_EMPTY : GLOCK_SHOOT, 2);

        V_PunchAxis(0, -2.0);
    }

    EV_GetDefaultShellInfo(args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 20, -12, 4);

    EV_EjectBrass(ShellOrigin, ShellVelocity, angles[YAW], shell, TE_BOUNCE_SHELL);

    gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "weapons/pl_gun3.wav", gEngfuncs.pfnRandomFloat(0.92, 1.0), ATTN_NORM, 0,
                                      98 + gEngfuncs.pfnRandomLong(0, 3));

    EV_GetGunPosition(args, vecSrc, origin);

    VectorCopy(forward, vecAiming);

    EV_HLDM_FireBullets(idx, forward, right, up, 1, vecSrc, vecAiming, 8192, BULLET_PLAYER_9MM, 0, 0, args->fparam1, args->fparam2);
}

void EV_FireGlock2(event_args_t* args)
{
    int idx;
    Vector origin;
    Vector angles;
    Vector velocity;

    Vector ShellVelocity;
    Vector ShellOrigin;
    int shell;
    Vector vecSrc, vecAiming;
    Vector vecSpread;
    Vector up, right, forward;

    idx = args->entindex;
    VectorCopy(args->origin, origin);
    VectorCopy(args->angles, angles);
    VectorCopy(args->velocity, velocity);

    AngleVectors(angles, forward, right, up);

    shell = gEngfuncs.pEventAPI->EV_FindModelIndex("models/shell.mdl"); // brass shell

    if (EV_IsLocal(idx))
    {
        // Add muzzle flash to current weapon model
        EV_MuzzleFlash();
        gEngfuncs.pEventAPI->EV_WeaponAnimation(GLOCK_SHOOT, 2);

        V_PunchAxis(0, -2.0);
    }

    EV_GetDefaultShellInfo(args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 20, -12, 4);

    EV_EjectBrass(ShellOrigin, ShellVelocity, angles[YAW], shell, TE_BOUNCE_SHELL);

    gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "weapons/pl_gun3.wav", gEngfuncs.pfnRandomFloat(0.92, 1.0), ATTN_NORM, 0,
                                      98 + gEngfuncs.pfnRandomLong(0, 3));

    EV_GetGunPosition(args, vecSrc, origin);

    VectorCopy(forward, vecAiming);

    EV_HLDM_FireBullets(idx, forward, right, up, 1, vecSrc, vecAiming, 8192, BULLET_PLAYER_9MM, 0, &tracerCount[idx - 1], args->fparam1, args->fparam2);
}
