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

void EV_SnarkFire(event_args_t* args)
{
    int idx;
    Vector vecSrc, angles, view_ofs, forward;
    pmtrace_t tr;

    idx = args->entindex;
    VectorCopy(args->origin, vecSrc);
    VectorCopy(args->angles, angles);

    AngleVectors(angles, forward, NULL, NULL);

    if (!EV_IsLocal(idx))
        return;

    if (args->ducking)
        vecSrc = vecSrc - (VEC_HULL_MIN - VEC_DUCK_HULL_MIN);

    // Store off the old count
    gEngfuncs.pEventAPI->EV_PushPMStates();

    // Now add in all of the players.
    gEngfuncs.pEventAPI->EV_SetSolidPlayers(idx - 1);
    gEngfuncs.pEventAPI->EV_SetTraceHull(2);
    gEngfuncs.pEventAPI->EV_PlayerTrace(vecSrc + forward * 20, vecSrc + forward * 64, PM_NORMAL, -1, &tr);

    //Find space to drop the thing.
    if (tr.allsolid == 0 && tr.startsolid == 0 && tr.fraction > 0.25)
        gEngfuncs.pEventAPI->EV_WeaponAnimation(SQUEAK_THROW, 0);

    gEngfuncs.pEventAPI->EV_PopPMStates();
}
