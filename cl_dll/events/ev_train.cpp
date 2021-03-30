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
void EV_TrainPitchAdjust(struct event_args_s* args);
}

void EV_TrainPitchAdjust(event_args_t* args)
{
    int idx;
    Vector origin;

    unsigned short us_params;
    int noise;
    float m_flVolume;
    int pitch;
    int stop;

    char sz[256];

    idx = args->entindex;

    VectorCopy(args->origin, origin);

    us_params = (unsigned short)args->iparam1;
    stop = args->bparam1;

    m_flVolume = (float)(us_params & 0x003f) / 40.0;
    noise = (int)(((us_params) >> 12) & 0x0007);
    pitch = (int)(10.0 * (float)((us_params >> 6) & 0x003f));

    switch (noise)
    {
    case 1:
        strcpy(sz, "plats/ttrain1.wav");
        break;
    case 2:
        strcpy(sz, "plats/ttrain2.wav");
        break;
    case 3:
        strcpy(sz, "plats/ttrain3.wav");
        break;
    case 4:
        strcpy(sz, "plats/ttrain4.wav");
        break;
    case 5:
        strcpy(sz, "plats/ttrain6.wav");
        break;
    case 6:
        strcpy(sz, "plats/ttrain7.wav");
        break;
    default:
        // no sound
        strcpy(sz, "");
        return;
    }

    if (stop)
    {
        gEngfuncs.pEventAPI->EV_StopSound(idx, CHAN_STATIC, sz);
    }
    else
    {
        gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_STATIC, sz, m_flVolume, ATTN_NORM, SND_CHANGE_PITCH, pitch);
    }
}
