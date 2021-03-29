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

#include "CWorld.h"
#include "CCorpse.h"
#include "client.h"
#include "util/decals.h"
#include "entities/effects/CMessage.h"
#include "entities/effects/CSoundEnt.h"
#include "gamerules/CGameRules.h"
#include "nodes/CGraph.h"
#include "util/lights.h"

extern DLL_GLOBAL int gDisplayTitle;
extern DLL_GLOBAL BOOL g_fGameOver;
extern CBaseEntity* g_pLastSpawn;
extern void W_Precache(); // weapon precache - weapons.cpp
void I_Precache(); // item precache

BOOL g_allowGJump;
BOOL g_startSuit; //LRC
CWorld* g_pWorld = NULL; //LRC

LINK_ENTITY_TO_CLASS(worldspawn, CWorld);

void CWorld::Spawn()
{
    g_fGameOver = FALSE;
    Precache();
}

void CWorld::Precache()
{
    //LRC - set up the world lists
    g_pWorld = this;
    m_pAssistLink = NULL;
    m_pFirstAlias = NULL;
    //    ALERT(at_console, "Clearing AssistList\n");

    g_pLastSpawn = NULL;

#if 1
    CVAR_SET_STRING("sv_gravity", "800"); // 67ft/sec
    CVAR_SET_STRING("sv_stepsize", "18");
#else
    CVAR_SET_STRING("sv_gravity", "384"); // 32ft/sec
    CVAR_SET_STRING("sv_stepsize", "24");
#endif

    CVAR_SET_STRING("room_type", "0"); // clear DSP

    // Set up game rules
    if (g_pGameRules)
    {
        delete g_pGameRules;
    }

    g_pGameRules = InstallGameRules();

    //!!!UNDONE why is there so much Spawn code in the Precache function? I'll just keep it here 

    /*    if ( WorldGraph.m_fGraphPresent && !WorldGraph.m_fGraphPointersSet )
        {
            if ( !WorldGraph.FSetGraphPointers() )
            {
                ALERT ( at_debug, "**Graph pointers were not set!\n");
            }
            else
            {
                ALERT ( at_debug, "**Graph Pointers Set!\n" );
            }
        }*/

    ///!!!LATER - do we want a sound ent in deathmatch? (sjb)
    //pSoundEnt = CBaseEntity::Create( "soundent", g_vecZero, g_vecZero, edict() );
    pSoundEnt = GetClassPtr((CSoundEnt*)NULL);
    pSoundEnt->Spawn();

    if (!pSoundEnt)
    {
        ALERT(at_debug, "**COULD NOT CREATE SOUNDENT**\n");
    }

    InitBodyQue();

    // init sentence group playback stuff from sentences.txt.
    // ok to call this multiple times, calls after first are ignored.

    SENTENCEG_Init();

    // init texture type array from materials.txt

    TEXTURETYPE_Init();


    // the area based ambient sounds MUST be the first precache_sounds

    // player precaches     
    W_Precache(); // get weapon precaches
    I_Precache(); // get Inventory Item precaches

    ClientPrecache();

    // sounds used from C physics code
    PRECACHE_SOUND("common/null.wav"); // clears sound channels

    PRECACHE_SOUND("items/suitchargeok1.wav"); //!!! temporary sound for respawning weapons.
    PRECACHE_SOUND("items/gunpickup2.wav"); // player picks up a gun.

    PRECACHE_SOUND("common/bodydrop3.wav"); // dead bodies hitting the ground (animation events)
    PRECACHE_SOUND("common/bodydrop4.wav");

    g_Language = (int)CVAR_GET_FLOAT("sv_language");
    if (g_Language == LANGUAGE_GERMAN)
    {
        PRECACHE_MODEL("models/germangibs.mdl");
    }
    else
    {
        PRECACHE_MODEL("models/hgibs.mdl");
        PRECACHE_MODEL("models/agibs.mdl");
    }

    PRECACHE_SOUND("weapons/ric1.wav");
    PRECACHE_SOUND("weapons/ric2.wav");
    PRECACHE_SOUND("weapons/ric3.wav");
    PRECACHE_SOUND("weapons/ric4.wav");
    PRECACHE_SOUND("weapons/ric5.wav");

    PRECACHE_MODEL("sprites/null.spr"); //LRC

    //
    // Setup light animation tables. 'a' is total darkness, 'z' is maxbright.
    //
    int i;

    // 0 normal
    for (i = 0; i <= 13; i++)
    {
        LIGHT_STYLE(i, (char*)STRING(GetStdLightStyle(i)));
    }

    // styles 32-62 are assigned by the light program for switchable lights

    // 63 testing
    LIGHT_STYLE(63, "a");

    for (i = 0; i < (int) gDecalsSize; i++)
        gDecals[i].index = DECAL_INDEX(gDecals[i].name);

    // init the WorldGraph.
    WorldGraph.InitGraph();

    // make sure the .NOD file is newer than the .BSP file.
    if (!WorldGraph.CheckNODFile((char*)STRING(gpGlobals->mapname)))
    {
        // NOD file is not present, or is older than the BSP file.
        WorldGraph.AllocNodes();
    }
    else
    {
        // Load the node graph for this level
        if (!WorldGraph.FLoadGraph((char*)STRING(gpGlobals->mapname)))
        {
            // couldn't load, so alloc and prepare to build a graph.
            ALERT(at_debug, "*Error opening .NOD file\n");
            WorldGraph.AllocNodes();
        }
        else
        {
            ALERT(at_debug, "\n*Graph Loaded!\n");
        }
    }

    if (pev->speed > 0)
        CVAR_SET_FLOAT("sv_zmax", pev->speed);
    else
        CVAR_SET_FLOAT("sv_zmax", 4096);

    if (pev->netname)
    {
        ALERT(at_aiconsole, "Chapter title: %s\n", STRING(pev->netname));
        CBaseEntity* pEntity = CBaseEntity::Create("env_message", g_vecZero, g_vecZero, NULL);
        if (pEntity)
        {
            pEntity->SetThink(&CBaseEntity::SUB_CallUseToggle);
            pEntity->pev->message = pev->netname;
            pev->netname = 0;
            pEntity->SetNextThink(0.3);
            pEntity->pev->spawnflags = SF_MESSAGE_ONCE;
        }
    }

    if (pev->spawnflags & SF_WORLD_DARK)
        CVAR_SET_FLOAT("v_dark", 1.0);
    else
        CVAR_SET_FLOAT("v_dark", 0.0);

    if (pev->spawnflags & SF_WORLD_TITLE)
        gDisplayTitle = TRUE; // display the game title if this key is set
    else
        gDisplayTitle = FALSE;

    if (pev->spawnflags & SF_WORLD_FORCETEAM)
    {
        CVAR_SET_FLOAT("mp_defaultteam", 1);
    }
    else
    {
        CVAR_SET_FLOAT("mp_defaultteam", 0);
    }
}


//
// Just to ignore the "wad" field.
//
void CWorld::KeyValue(KeyValueData* pkvd)
{
    if (FStrEq(pkvd->szKeyName, "skyname"))
    {
        // Sent over net now.
        CVAR_SET_STRING("sv_skyname", pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "sounds"))
    {
        gpGlobals->cdAudioTrack = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "WaveHeight"))
    {
        // Sent over net now.
        pev->scale = atof(pkvd->szValue) * (1.0 / 8.0);
        pkvd->fHandled = TRUE;
        CVAR_SET_FLOAT("sv_wateramp", pev->scale);
    }
    else if (FStrEq(pkvd->szKeyName, "MaxRange"))
    {
        pev->speed = atof(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "chaptertitle"))
    {
        pev->netname = ALLOC_STRING(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "startdark"))
    {
        // UNDONE: This is a gross hack!!! The CVAR is NOT sent over the client/sever link
        // but it will work for single player
        int flag = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
        if (flag)
            pev->spawnflags |= SF_WORLD_DARK;
    }
    else if (FStrEq(pkvd->szKeyName, "newunit"))
    {
        // Single player only.  Clear save directory if set
        if (atoi(pkvd->szValue))
            CVAR_SET_FLOAT("sv_newunit", 1);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "gametitle"))
    {
        if (atoi(pkvd->szValue))
            pev->spawnflags |= SF_WORLD_TITLE;

        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "mapteams"))
    {
        pev->team = ALLOC_STRING(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "defaultteam"))
    {
        if (atoi(pkvd->szValue))
        {
            pev->spawnflags |= SF_WORLD_FORCETEAM;
        }
        pkvd->fHandled = TRUE;
    }
        //LRC- let map designers start the player with his suit already on
    else if (FStrEq(pkvd->szKeyName, "startsuit"))
    {
        g_startSuit = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "allowmonsters"))
    {
        CVAR_SET_FLOAT("mp_allowmonsters", atof(pkvd->szValue));
        pkvd->fHandled = TRUE;
    }
        //LRC- ends

        //AJH- Gauss Jump in single play
    else if (FStrEq(pkvd->szKeyName, "allow_sp_gjump"))
    {
        g_allowGJump = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else if (FStrEq(pkvd->szKeyName, "timed_damage"))
    {
        CVAR_SET_FLOAT("timed_damage", atof(pkvd->szValue));
        pkvd->fHandled = TRUE;
    }
    else
        CBaseEntity::KeyValue(pkvd);
}

void I_Precache()
{
    // common world objects (moved from W_Precache - weapons.cpp)
    UTIL_PrecacheOther("item_suit");
    UTIL_PrecacheOther("item_battery");
    UTIL_PrecacheOther("item_antidote");
    UTIL_PrecacheOther("item_security");
    UTIL_PrecacheOther("item_longjump");
    UTIL_PrecacheOther("item_healthkit");
}
