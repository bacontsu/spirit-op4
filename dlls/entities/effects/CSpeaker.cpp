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

#include "CSpeaker.h"
#include "entities/base/CTalkMonster.h"

LINK_ENTITY_TO_CLASS(speaker, CSpeaker);

TYPEDESCRIPTION CSpeaker::m_SaveData[] =
{
    DEFINE_FIELD(CSpeaker, m_preset, FIELD_INTEGER),
};

IMPLEMENT_SAVERESTORE(CSpeaker, CBaseEntity);

//
// ambient_generic - general-purpose user-defined static sound
//
void CSpeaker::Spawn()
{
    char* szSoundFile = (char*)STRING(pev->message);

    if (!m_preset && (FStringNull(pev->message) || strlen(szSoundFile) < 1))
    {
        ALERT(at_error, "SPEAKER with no Level/Sentence! at: %f, %f, %f\n", pev->origin.x, pev->origin.y, pev->origin.z);
        SetNextThink(0.1);
        SetThink(&CSpeaker::SUB_Remove);
        return;
    }
    pev->solid = SOLID_NOT;
    pev->movetype = MOVETYPE_NONE;


    SetThink(&CSpeaker::SpeakerThink);
    DontThink();

    // allow on/off switching via 'use' function.

    SetUse(&CSpeaker::ToggleUse);

    Precache();
}

void CSpeaker::Precache()
{
    if (!FBitSet(pev->spawnflags, SPEAKER_START_SILENT))
        // set first announcement time for random n second
        SetNextThink(RANDOM_FLOAT(5.0, 15.0));
}

void CSpeaker::SpeakerThink()
{
    const char* szSoundFile;
    float flvolume = pev->health * 0.1;
    float flattenuation = 0.3;
    int flags = 0;
    int pitch = 100;


    // Wait for the talkmonster to finish first.
    if (gpGlobals->time <= CTalkMonster::g_talkWaitTime)
    {
        AbsoluteNextThink(CTalkMonster::g_talkWaitTime + RANDOM_FLOAT(5, 10));
        return;
    }

    if (m_preset)
    {
        // go lookup preset text, assign szSoundFile
        switch (m_preset)
        {
        case 1: szSoundFile = "C1A0_";
            break;
        case 2: szSoundFile = "C1A1_";
            break;
        case 3: szSoundFile = "C1A2_";
            break;
        case 4: szSoundFile = "C1A3_";
            break;
        case 5: szSoundFile = "C1A4_";
            break;
        case 6: szSoundFile = "C2A1_";
            break;
        case 7: szSoundFile = "C2A2_";
            break;
        case 8: szSoundFile = "C2A3_";
            break;
        case 9: szSoundFile = "C2A4_";
            break;
        case 10: szSoundFile = "C2A5_";
            break;
        case 11: szSoundFile = "C3A1_";
            break;
        case 12: szSoundFile = "C3A2_";
            break;
        }
    }
    else
        szSoundFile = STRING(pev->message);

    if (szSoundFile[0] == '!')
    {
        // play single sentence, one shot
        UTIL_EmitAmbientSound(ENT(pev), pev->origin, szSoundFile,
                              flvolume, flattenuation, flags, pitch);

        // shut off and reset
        DontThink();
    }
    else
    {
        // make random announcement from sentence group

        if (SENTENCEG_PlayRndSz(ENT(pev), szSoundFile, flvolume, flattenuation, flags, pitch) < 0)
            ALERT(at_debug, "Level Design Error!\nSPEAKER has bad sentence group name: %s\n", szSoundFile);

        // set next announcement time for random 5 to 10 minute delay
        SetNextThink(RANDOM_FLOAT(ANNOUNCE_MINUTES_MIN * 60.0, ANNOUNCE_MINUTES_MAX * 60.0));

        CTalkMonster::g_talkWaitTime = gpGlobals->time + 5; // time delay until it's ok to speak: used so that two NPCs don't talk at once
    }

    return;
}


//
// ToggleUse - if an announcement is pending, cancel it.  If no announcement is pending, start one.
//
void CSpeaker::ToggleUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
    int fActive = (m_fNextThink > 0.0);

    // fActive is TRUE only if an announcement is pending

    if (useType != USE_TOGGLE)
    {
        // ignore if we're just turning something on that's already on, or
        // turning something off that's already off.
        if ((fActive && useType == USE_ON) || (!fActive && useType == USE_OFF))
            return;
    }

    if (useType == USE_ON)
    {
        // turn on announcements
        SetNextThink(0.1);
        return;
    }

    if (useType == USE_OFF)
    {
        // turn off announcements
        DontThink();
        return;
    }

    // Toggle announcements


    if (fActive)
    {
        // turn off announcements
        DontThink();
    }
    else
    {
        // turn on announcements
        SetNextThink(0.1);
    }
}

// KeyValue - load keyvalue pairs into member data
// NOTE: called BEFORE spawn!

void CSpeaker::KeyValue(KeyValueData* pkvd)
{
    // preset
    if (FStrEq(pkvd->szKeyName, "preset"))
    {
        m_preset = atoi(pkvd->szValue);
        pkvd->fHandled = TRUE;
    }
    else
        CBaseEntity::KeyValue(pkvd);
}
