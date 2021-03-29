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

// shared event functions
#include "ev_common.h"
#include "hud/CHud.h"
#include "event_api.h"
#include "pm_shared.h"
#include "pm_materials.h"

int tracerCount[32];

#define IS_FIRSTPERSON_SPEC ( g_iUser1 == OBS_IN_EYE || (g_iUser1 && (gHUD.m_Spectator.m_pip->value == INSET_IN_EYE)) )
/*
=================
GetEntity

Return's the requested cl_entity_t
=================
*/
struct cl_entity_s* GetEntity(int idx)
{
    return gEngfuncs.GetEntityByIndex(idx);
}

/*
=================
GetViewEntity

Return's the current weapon/view model
=================
*/
struct cl_entity_s* GetViewEntity()
{
    return gEngfuncs.GetViewModel();
}

/*
=================
EV_CreateTracer

Creates a tracer effect
=================
*/
void EV_CreateTracer(float* start, float* end)
{
    gEngfuncs.pEfxAPI->R_TracerEffect(start, end);
}

/*
=================
EV_IsPlayer

Is the entity's index in the player range?
=================
*/
qboolean EV_IsPlayer(int idx)
{
    if (idx >= 1 && idx <= gEngfuncs.GetMaxClients())
        return true;

    return false;
}

/*
=================
EV_IsLocal

Is the entity == the local player
=================
*/
qboolean EV_IsLocal(int idx)
{
    // check if we are in some way in first person spec mode
    if (IS_FIRSTPERSON_SPEC)
        return (g_iUser2 == idx);
    else
        return gEngfuncs.pEventAPI->EV_IsLocal(idx - 1) ? true : false;
}

/*
=================
EV_GetGunPosition

Figure out the height of the gun
=================
*/
void EV_GetGunPosition(event_args_t* args, float* pos, float* origin)
{
    int idx;
    vec3_t view_ofs;

    idx = args->entindex;

    view_ofs.Clear();
    view_ofs[2] = DEFAULT_VIEWHEIGHT;

    if (EV_IsPlayer(idx))
    {
        // in spec mode use entity viewheigh, not own
        if (EV_IsLocal(idx) && !IS_FIRSTPERSON_SPEC)
        {
            // Grab predicted result for local player
            gEngfuncs.pEventAPI->EV_LocalPlayerViewheight(view_ofs);
        }
        else if (args->ducking == 1)
        {
            view_ofs[2] = VEC_DUCK_VIEW;
        }
    }

    VectorAdd(origin, view_ofs, pos);
}

/*
=================
EV_EjectBrass

Bullet shell casings
=================
*/
void EV_EjectBrass(float* origin, float* velocity, float rotation, int model, int soundtype)
{
    vec3_t endpos;
    endpos.Clear();
    endpos[1] = rotation;
    gEngfuncs.pEfxAPI->R_TempModel(origin, velocity, endpos, 2.5, model, soundtype);
}

/*
=================
EV_GetDefaultShellInfo

Determine where to eject shells from
=================
*/
void EV_GetDefaultShellInfo(event_args_t* args, float* origin, float* velocity, float* ShellVelocity, float* ShellOrigin, float* forward, float* right,
                            float* up, float forwardScale, float upScale, float rightScale)
{
    int i;
    vec3_t view_ofs;
    float fR, fU;

    int idx;

    idx = args->entindex;

    view_ofs.Clear();
    view_ofs[2] = DEFAULT_VIEWHEIGHT;

    if (EV_IsPlayer(idx))
    {
        if (EV_IsLocal(idx))
        {
            gEngfuncs.pEventAPI->EV_LocalPlayerViewheight(view_ofs);
        }
        else if (args->ducking == 1)
        {
            view_ofs[2] = VEC_DUCK_VIEW;
        }
    }

    fR = gEngfuncs.pfnRandomFloat(50, 70);
    fU = gEngfuncs.pfnRandomFloat(100, 150);

    for (i = 0; i < 3; i++)
    {
        ShellVelocity[i] = velocity[i] + right[i] * fR + up[i] * fU + forward[i] * 25;
        ShellOrigin[i] = origin[i] + view_ofs[i] + up[i] * upScale + forward[i] * forwardScale + right[i] * rightScale;
    }
}

/*
=================
EV_MuzzleFlash

Flag weapon/view model for muzzle flash
=================
*/
void EV_MuzzleFlash()
{
    // Add muzzle flash to current weapon model
    cl_entity_t* ent = GetViewEntity();
    if (!ent)
    {
        return;
    }

    // Or in the muzzle flash
    ent->curstate.effects |= EF_MUZZLEFLASH;
}


void EV_HLDM_GunshotDecalTrace(pmtrace_t* pTrace, char* decalName)
{
    int iRand;
    physent_t* pe;

    gEngfuncs.pEfxAPI->R_BulletImpactParticles(pTrace->endpos);

    iRand = gEngfuncs.pfnRandomLong(0, 0x7FFF);
    if (iRand < (0x7fff / 2))// not every bullet makes a sound.
    {
        switch (iRand % 5)
        {
        case 0:	gEngfuncs.pEventAPI->EV_PlaySound(-1, pTrace->endpos, 0, "weapons/ric1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM); break;
        case 1:	gEngfuncs.pEventAPI->EV_PlaySound(-1, pTrace->endpos, 0, "weapons/ric2.wav", 1.0, ATTN_NORM, 0, PITCH_NORM); break;
        case 2:	gEngfuncs.pEventAPI->EV_PlaySound(-1, pTrace->endpos, 0, "weapons/ric3.wav", 1.0, ATTN_NORM, 0, PITCH_NORM); break;
        case 3:	gEngfuncs.pEventAPI->EV_PlaySound(-1, pTrace->endpos, 0, "weapons/ric4.wav", 1.0, ATTN_NORM, 0, PITCH_NORM); break;
        case 4:	gEngfuncs.pEventAPI->EV_PlaySound(-1, pTrace->endpos, 0, "weapons/ric5.wav", 1.0, ATTN_NORM, 0, PITCH_NORM); break;
        }
    }

    pe = gEngfuncs.pEventAPI->EV_GetPhysent(pTrace->ent);

    // Only decal brush models such as the world etc.
    if (decalName && decalName[0] && pe && (pe->solid == SOLID_BSP || pe->movetype == MOVETYPE_PUSHSTEP))
    {
        if (CVAR_GET_FLOAT("r_decals"))
        {
            gEngfuncs.pEfxAPI->R_DecalShoot(
                gEngfuncs.pEfxAPI->Draw_DecalIndex(gEngfuncs.pEfxAPI->Draw_DecalIndexFromName(decalName)),
                gEngfuncs.pEventAPI->EV_IndexFromTrace(pTrace), 0, pTrace->endpos, 0);
        }
    }
}

char* EV_HLDM_DamageDecal(physent_t* pe)
{
    static char decalname[32];
    int idx;

    if (pe->classnumber == 1)
    {
        idx = gEngfuncs.pfnRandomLong(0, 2);
        sprintf(decalname, "{break%i", idx + 1);
    }
    else if (pe->rendermode != kRenderNormal)
    {
        sprintf(decalname, "{bproof1");
    }
    else
    {
        idx = gEngfuncs.pfnRandomLong(0, 4);
        sprintf(decalname, "{shot%i", idx + 1);
    }
    return decalname;
}

void EV_HLDM_DecalGunshot(pmtrace_t* pTrace, int iBulletType)
{
    physent_t* pe;

    pe = gEngfuncs.pEventAPI->EV_GetPhysent(pTrace->ent);

    if (pe && pe->solid == SOLID_BSP)
    {
        switch (iBulletType)
        {
        case BULLET_PLAYER_9MM:
        case BULLET_MONSTER_9MM:
        case BULLET_PLAYER_MP5:
        case BULLET_MONSTER_MP5:
        case BULLET_PLAYER_BUCKSHOT:
        case BULLET_PLAYER_357:
        default:
            // smoke and decal
            EV_HLDM_GunshotDecalTrace(pTrace, EV_HLDM_DamageDecal(pe));
            break;
        }
    }
}

int EV_HLDM_CheckTracer(int idx, float* vecSrc, float* end, float* forward, float* right, int iBulletType, int iTracerFreq, int* tracerCount)
{
    int tracer = 0;
    int i;
    qboolean player = idx >= 1 && idx <= gEngfuncs.GetMaxClients() ? true : false;

    if (iTracerFreq != 0 && ((*tracerCount)++ % iTracerFreq) == 0)
    {
        vec3_t vecTracerSrc;

        if (player)
        {
            vec3_t offset(0, 0, -4);

            // adjust tracer position for player
            for (i = 0; i < 3; i++)
            {
                vecTracerSrc[i] = vecSrc[i] + offset[i] + right[i] * 2 + forward[i] * 16;
            }
        }
        else
        {
            VectorCopy(vecSrc, vecTracerSrc);
        }

        if (iTracerFreq != 1)		// guns that always trace also always decal
            tracer = 1;

        switch (iBulletType)
        {
        case BULLET_PLAYER_MP5:
        case BULLET_MONSTER_MP5:
        case BULLET_MONSTER_9MM:
        case BULLET_MONSTER_12MM:
        default:
            EV_CreateTracer(vecTracerSrc, end);
            break;
        }
    }

    return tracer;
}

// play a strike sound based on the texture that was hit by the attack traceline.  VecSrc/VecEnd are the
// original traceline endpoints used by the attacker, iBulletType is the type of bullet that hit the texture.
// returns volume of strike instrument (crowbar) to play
float EV_HLDM_PlayTextureSound(int idx, pmtrace_t* ptr, float* vecSrc, float* vecEnd, int iBulletType)
{
    // hit the world, try to play sound based on texture material type
    char chTextureType = CHAR_TEX_CONCRETE;
    float fvol;
    float fvolbar;
    const char* rgsz[4];
    int cnt;
    float fattn = ATTN_NORM;
    int entity;
    char* pTextureName;
    char texname[64];
    char szbuffer[64];

    entity = gEngfuncs.pEventAPI->EV_IndexFromTrace(ptr);

    // FIXME check if playtexture sounds movevar is set
    //

    chTextureType = 0;

    // Player
    if (entity >= 1 && entity <= gEngfuncs.GetMaxClients())
    {
        // hit body
        chTextureType = CHAR_TEX_FLESH;
    }
    else if (entity == 0)
    {
        // get texture from entity or world (world is ent(0))
        pTextureName = (char*)gEngfuncs.pEventAPI->EV_TraceTexture(ptr->ent, vecSrc, vecEnd);

        if (pTextureName)
        {
            strcpy(texname, pTextureName);
            pTextureName = texname;

            // strip leading '-0' or '+0~' or '{' or '!'
            if (*pTextureName == '-' || *pTextureName == '+')
            {
                pTextureName += 2;
            }

            if (*pTextureName == '{' || *pTextureName == '!' || *pTextureName == '~' || *pTextureName == ' ')
            {
                pTextureName++;
            }

            // '}}'
            strcpy(szbuffer, pTextureName);
            szbuffer[CBTEXTURENAMEMAX - 1] = 0;

            // get texture type
            chTextureType = PM_FindTextureType(szbuffer);
        }
    }

    switch (chTextureType)
    {
    default:
    case CHAR_TEX_CONCRETE: fvol = 0.9;	fvolbar = 0.6;
        rgsz[0] = "player/pl_step1.wav";
        rgsz[1] = "player/pl_step2.wav";
        cnt = 2;
        break;
    case CHAR_TEX_METAL: fvol = 0.9; fvolbar = 0.3;
        rgsz[0] = "player/pl_metal1.wav";
        rgsz[1] = "player/pl_metal2.wav";
        cnt = 2;
        break;
    case CHAR_TEX_DIRT:	fvol = 0.9; fvolbar = 0.1;
        rgsz[0] = "player/pl_dirt1.wav";
        rgsz[1] = "player/pl_dirt2.wav";
        rgsz[2] = "player/pl_dirt3.wav";
        cnt = 3;
        break;
    case CHAR_TEX_VENT:	fvol = 0.5; fvolbar = 0.3;
        rgsz[0] = "player/pl_duct1.wav";
        rgsz[1] = "player/pl_duct1.wav";
        cnt = 2;
        break;
    case CHAR_TEX_GRATE: fvol = 0.9; fvolbar = 0.5;
        rgsz[0] = "player/pl_grate1.wav";
        rgsz[1] = "player/pl_grate4.wav";
        cnt = 2;
        break;
    case CHAR_TEX_TILE:	fvol = 0.8; fvolbar = 0.2;
        rgsz[0] = "player/pl_tile1.wav";
        rgsz[1] = "player/pl_tile3.wav";
        rgsz[2] = "player/pl_tile2.wav";
        rgsz[3] = "player/pl_tile4.wav";
        cnt = 4;
        break;
    case CHAR_TEX_SLOSH: fvol = 0.9; fvolbar = 0.0;
        rgsz[0] = "player/pl_slosh1.wav";
        rgsz[1] = "player/pl_slosh3.wav";
        rgsz[2] = "player/pl_slosh2.wav";
        rgsz[3] = "player/pl_slosh4.wav";
        cnt = 4;
        break;
    case CHAR_TEX_WOOD: fvol = 0.9; fvolbar = 0.2;
        rgsz[0] = "debris/wood1.wav";
        rgsz[1] = "debris/wood2.wav";
        rgsz[2] = "debris/wood3.wav";
        cnt = 3;
        break;
    case CHAR_TEX_GLASS:
    case CHAR_TEX_COMPUTER:
        fvol = 0.8; fvolbar = 0.2;
        rgsz[0] = "debris/glass1.wav";
        rgsz[1] = "debris/glass2.wav";
        rgsz[2] = "debris/glass3.wav";
        cnt = 3;
        break;
    case CHAR_TEX_FLESH:
        if (iBulletType == BULLET_PLAYER_CROWBAR)
            return 0.0; // crowbar already makes this sound
        fvol = 1.0;	fvolbar = 0.2;
        rgsz[0] = "weapons/bullet_hit1.wav";
        rgsz[1] = "weapons/bullet_hit2.wav";
        fattn = 1.0;
        cnt = 2;
        break;
    }

    // play material hit sound
    gEngfuncs.pEventAPI->EV_PlaySound(0, ptr->endpos, CHAN_STATIC, rgsz[gEngfuncs.pfnRandomLong(0, cnt - 1)], fvol, fattn, 0, 96 + gEngfuncs.pfnRandomLong(0, 0xf));
    return fvolbar;
}

/*
================
FireBullets

Go to the trouble of combining multiple pellets into a single damage call.
================
*/
void EV_HLDM_FireBullets(int idx, float* forward, float* right, float* up, int cShots, float* vecSrc, float* vecDirShooting, float flDistance, int iBulletType, int iTracerFreq, int* tracerCount, float flSpreadX, float flSpreadY)
{
    int i;
    pmtrace_t tr;
    int iShot;
    int tracer;

    for (iShot = 1; iShot <= cShots; iShot++)
    {
        vec3_t vecDir, vecEnd;

        float x, y, z;
        //We randomize for the Shotgun.
        if (iBulletType == BULLET_PLAYER_BUCKSHOT)
        {
            do {
                x = gEngfuncs.pfnRandomFloat(-0.5, 0.5) + gEngfuncs.pfnRandomFloat(-0.5, 0.5);
                y = gEngfuncs.pfnRandomFloat(-0.5, 0.5) + gEngfuncs.pfnRandomFloat(-0.5, 0.5);
                z = x * x + y * y;
            } while (z > 1);

            for (i = 0; i < 3; i++)
            {
                vecDir[i] = vecDirShooting[i] + x * flSpreadX * right[i] + y * flSpreadY * up[i];
                vecEnd[i] = vecSrc[i] + flDistance * vecDir[i];
            }
        }//But other guns already have their spread randomized in the synched spread.
        else
        {

            for (i = 0; i < 3; i++)
            {
                vecDir[i] = vecDirShooting[i] + flSpreadX * right[i] + flSpreadY * up[i];
                vecEnd[i] = vecSrc[i] + flDistance * vecDir[i];
            }
        }

        gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction(false, true);

        // Store off the old count
        gEngfuncs.pEventAPI->EV_PushPMStates();

        // Now add in all of the players.
        gEngfuncs.pEventAPI->EV_SetSolidPlayers(idx - 1);

        gEngfuncs.pEventAPI->EV_SetTraceHull(2);
        gEngfuncs.pEventAPI->EV_PlayerTrace(vecSrc, vecEnd, PM_STUDIO_BOX, -1, &tr);

        tracer = EV_HLDM_CheckTracer(idx, vecSrc, tr.endpos, forward, right, iBulletType, iTracerFreq, tracerCount);

        // do damage, paint decals
        if (tr.fraction != 1.0)
        {
            switch (iBulletType)
            {
            default:
            case BULLET_PLAYER_9MM:

                EV_HLDM_PlayTextureSound(idx, &tr, vecSrc, vecEnd, iBulletType);
                EV_HLDM_DecalGunshot(&tr, iBulletType);

                break;
            case BULLET_PLAYER_MP5:

                if (!tracer)
                {
                    EV_HLDM_PlayTextureSound(idx, &tr, vecSrc, vecEnd, iBulletType);
                    EV_HLDM_DecalGunshot(&tr, iBulletType);
                }
                break;
            case BULLET_PLAYER_BUCKSHOT:

                EV_HLDM_DecalGunshot(&tr, iBulletType);

                break;
            case BULLET_PLAYER_357:

                EV_HLDM_PlayTextureSound(idx, &tr, vecSrc, vecEnd, iBulletType);
                EV_HLDM_DecalGunshot(&tr, iBulletType);

                break;

            }
        }

        gEngfuncs.pEventAPI->EV_PopPMStates();
    }
}
