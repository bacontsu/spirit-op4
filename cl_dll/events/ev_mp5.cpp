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
void EV_FireMP5(struct event_args_s* args);
void EV_FireMP52(struct event_args_s* args);
}

void EV_FireMP5(event_args_t* args)
{
    int idx;
    Vector origin;
    Vector angles;
    Vector velocity;

    Vector ShellVelocity;
    Vector ShellOrigin;
    int shell;
    Vector vecSrc, vecAiming;
    Vector up, right, forward;
    float flSpread = 0.01;

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
        gEngfuncs.pEventAPI->EV_WeaponAnimation(MP5_FIRE1 + gEngfuncs.pfnRandomLong(0, 2), 2);

        V_PunchAxis(0, gEngfuncs.pfnRandomFloat(-2, 2));
    }

    EV_GetDefaultShellInfo(args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 20, -12, 4);

    EV_EjectBrass(ShellOrigin, ShellVelocity, angles[YAW], shell, TE_BOUNCE_SHELL);

    switch (gEngfuncs.pfnRandomLong(0, 1))
    {
    case 0:
        gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "weapons/hks1.wav", 1, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong(0, 0xf));
        break;
    case 1:
        gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "weapons/hks2.wav", 1, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong(0, 0xf));
        break;
    }

    EV_GetGunPosition(args, vecSrc, origin);
    VectorCopy(forward, vecAiming);

    if (gEngfuncs.GetMaxClients() > 1)
    {
        EV_HLDM_FireBullets(idx, forward, right, up, 1, vecSrc, vecAiming, 8192, BULLET_PLAYER_MP5, 2, &tracerCount[idx - 1], args->fparam1, args->fparam2);
    }
    else
    {
        EV_HLDM_FireBullets(idx, forward, right, up, 1, vecSrc, vecAiming, 8192, BULLET_PLAYER_MP5, 2, &tracerCount[idx - 1], args->fparam1, args->fparam2);
    }
}

// We only predict the animation and sound
// The grenade is still launched from the server.
void EV_FireMP52(event_args_t* args)
{
    int idx;
    Vector origin;

    idx = args->entindex;
    VectorCopy(args->origin, origin);

    if (EV_IsLocal(idx))
    {
        gEngfuncs.pEventAPI->EV_WeaponAnimation(MP5_LAUNCH, 2);
        V_PunchAxis(0, -10);
    }

    switch (gEngfuncs.pfnRandomLong(0, 1))
    {
    case 0:
        gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "weapons/glauncher.wav", 1, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong(0, 0xf));
        break;
    case 1:
        gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, "weapons/glauncher2.wav", 1, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong(0, 0xf));
        break;
    }
}
