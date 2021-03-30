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
void EV_EgonFire(struct event_args_s* args);
void EV_EgonStop(struct event_args_s* args);
}

int g_fireAnims1[] = {EGON_FIRE1, EGON_FIRE2, EGON_FIRE3, EGON_FIRE4};
int g_fireAnims2[] = {EGON_ALTFIRECYCLE};

#define ARRAYSIZE(p)		(sizeof(p)/sizeof(p[0]))

BEAM* pBeam;
BEAM* pBeam2;
TEMPENTITY* pFlare; // Vit_amiN: egon's beam flare

void EV_EgonFlareCallback(struct tempent_s* ent, float frametime, float currenttime)
{
    float delta = currenttime - ent->tentOffset.z; // time past since the last scale
    if (delta >= ent->tentOffset.y)
    {
        ent->entity.curstate.scale += ent->tentOffset.x * delta;
        ent->tentOffset.z = currenttime;
    }
}

void EV_EgonFire(event_args_t* args)
{
    int idx, iFireState, iFireMode;
    Vector origin;

    idx = args->entindex;
    VectorCopy(args->origin, origin);
    iFireState = args->iparam1;
    iFireMode = args->iparam2;
    int iStartup = args->bparam1;


    if (iStartup)
    {
        if (iFireMode == FIRE_WIDE)
            gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, EGON_SOUND_STARTUP, 0.98, ATTN_NORM, 0, 125);
        else
            gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, EGON_SOUND_STARTUP, 0.9, ATTN_NORM, 0, 100);
    }
    else
    {
        //If there is any sound playing already, kill it.
        //This is necessary because multiple sounds can play on the same channel at the same time.
        //In some cases, more than 1 run sound plays when the egon stops firing, in which case only the earliest entry in the list is stopped.
        //This ensures no more than 1 of those is ever active at the same time.
        gEngfuncs.pEventAPI->EV_StopSound(idx, CHAN_STATIC, EGON_SOUND_RUN);

        if (iFireMode == FIRE_WIDE)
            gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_STATIC, EGON_SOUND_RUN, 0.98, ATTN_NORM, 0, 125);
        else
            gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_STATIC, EGON_SOUND_RUN, 0.9, ATTN_NORM, 0, 100);
    }

    //Only play the weapon anims if I shot it.
    if (EV_IsLocal(idx))
        gEngfuncs.pEventAPI->EV_WeaponAnimation(g_fireAnims1[gEngfuncs.pfnRandomLong(0, 3)], 1);

    if (iStartup == 1 && EV_IsLocal(idx) && !pBeam && !pBeam2 && !pFlare && cl_lw->value)
        //Adrian: Added the cl_lw check for those lital people that hate weapon prediction.
    {
        Vector vecSrc, vecEnd, origin, angles, forward, right, up;
        pmtrace_t tr;

        cl_entity_t* pl = gEngfuncs.GetEntityByIndex(idx);

        if (pl)
        {
            VectorCopy(gHUD.m_vecAngles, angles);

            AngleVectors(angles, forward, right, up);

            EV_GetGunPosition(args, vecSrc, pl->origin);

            VectorMA(vecSrc, 2048, forward, vecEnd);

            gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction(false, true);

            // Store off the old count
            gEngfuncs.pEventAPI->EV_PushPMStates();

            // Now add in all of the players.
            gEngfuncs.pEventAPI->EV_SetSolidPlayers(idx - 1);

            gEngfuncs.pEventAPI->EV_SetTraceHull(2);
            gEngfuncs.pEventAPI->EV_PlayerTrace(vecSrc, vecEnd, PM_STUDIO_BOX, -1, &tr);

            gEngfuncs.pEventAPI->EV_PopPMStates();

            int iBeamModelIndex = gEngfuncs.pEventAPI->EV_FindModelIndex(EGON_BEAM_SPRITE);

            float r = 50.0f;
            float g = 50.0f;
            float b = 125.0f;

            //if ( IEngineStudio.IsHardware() )
            {
                r /= 255.0f;
                g /= 255.0f;
                b /= 255.0f;
            }


            pBeam = gEngfuncs.pEfxAPI->R_BeamEntPoint(idx | 0x1000, tr.endpos, iBeamModelIndex, 99999, 3.5, 0.2, 0.7, 55, 0, 0, r, g, b);

            if (pBeam)
                pBeam->flags |= (FBEAM_SINENOISE);

            pBeam2 = gEngfuncs.pEfxAPI->R_BeamEntPoint(idx | 0x1000, tr.endpos, iBeamModelIndex, 99999, 5.0, 0.08, 0.7, 25, 0, 0, r, g, b);

            // Vit_amiN: egon beam flare
            pFlare = gEngfuncs.pEfxAPI->R_TempSprite(tr.endpos, vec3_origin, 1.0,
                                                     gEngfuncs.pEventAPI->EV_FindModelIndex(EGON_FLARE_SPRITE),
                                                     kRenderGlow, kRenderFxNoDissipation, 1.0, 99999, FTENT_SPRCYCLE | FTENT_PERSIST);
        }
    }

    if (pFlare) // Vit_amiN: store the last mode for EV_EgonStop()
    {
        pFlare->tentOffset.x = (iFireMode == FIRE_WIDE) ? 1.0f : 0.0f;
    }
}

void EV_EgonStop(event_args_t* args)
{
    int idx;
    Vector origin;

    idx = args->entindex;
    VectorCopy(args->origin, origin);

    gEngfuncs.pEventAPI->EV_StopSound(idx, CHAN_STATIC, EGON_SOUND_RUN);

    if (args->iparam1)
        gEngfuncs.pEventAPI->EV_PlaySound(idx, origin, CHAN_WEAPON, EGON_SOUND_OFF, 0.98, ATTN_NORM, 0, 100);

    if (EV_IsLocal(idx))
    {
        if (pBeam)
        {
            pBeam->die = 0.0;
            pBeam = NULL;
        }


        if (pBeam2)
        {
            pBeam2->die = 0.0;
            pBeam2 = NULL;
        }

        if (pFlare) // Vit_amiN: egon beam flare
        {
            pFlare->die = gEngfuncs.GetClientTime();

            if (gEngfuncs.GetMaxClients() == 1 || !(pFlare->flags & FTENT_NOMODEL))
            {
                if (pFlare->tentOffset.x != 0.0f) // true for iFireMode == FIRE_WIDE
                {
                    pFlare->callback = &EV_EgonFlareCallback;
                    pFlare->fadeSpeed = 2.0; // fade out will take 0.5 sec
                    pFlare->tentOffset.x = 10.0; // scaling speed per second
                    pFlare->tentOffset.y = 0.1; // min time between two scales
                    pFlare->tentOffset.z = pFlare->die; // the last callback run time
                    pFlare->flags = FTENT_FADEOUT | FTENT_CLIENTCUSTOM;
                }
            }

            pFlare = NULL;
        }
    }
}
