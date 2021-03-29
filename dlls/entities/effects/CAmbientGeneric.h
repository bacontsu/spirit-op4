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
* Valve LLC.All other use, distribution, or modification is prohibited
* without written permission from Valve LLC.
*
****/
#pragma once

#include "entities/CBaseEntity.h"

// runtime pitch shift and volume fadein/out structure

// NOTE: IF YOU CHANGE THIS STRUCT YOU MUST CHANGE THE SAVE/RESTORE VERSION NUMBER
// SEE BELOW (in the typedescription for the class)
typedef struct dynpitchvol
{
    // NOTE: do not change the order of these parameters 
    // NOTE: unless you also change order of rgdpvpreset array elements!
    int preset;

    int pitchrun; // pitch shift % when sound is running 0 - 255
    int pitchstart; // pitch shift % when sound stops or starts 0 - 255
    int spinup; // spinup time 0 - 100
    int spindown; // spindown time 0 - 100

    int volrun; // volume change % when sound is running 0 - 10
    int volstart; // volume change % when sound stops or starts 0 - 10
    int fadein; // volume fade in time 0 - 100
    int fadeout; // volume fade out time 0 - 100

    // Low Frequency Oscillator
    int lfotype; // 0) off 1) square 2) triangle 3) random
    int lforate; // 0 - 1000, how fast lfo osciallates

    int lfomodpitch; // 0-100 mod of current pitch. 0 is off.
    int lfomodvol; // 0-100 mod of current volume. 0 is off.

    int cspinup; // each trigger hit increments counter and spinup pitch


    int cspincount;

    int pitch;
    int spinupsav;
    int spindownsav;
    int pitchfrac;

    int vol;
    int fadeinsav;
    int fadeoutsav;
    int volfrac;

    int lfofrac;
    int lfomult;
} dynpitchvol_t;

#define CDPVPRESETMAX 27

extern dynpitchvol_t rgdpvpreset[CDPVPRESETMAX];

class CAmbientGeneric : public CBaseEntity
{
public:
    void KeyValue(KeyValueData* pkvd) override;
    void Spawn() override;
    //    void PostSpawn();
    void Precache() override;
    void DLLEXPORT ToggleUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
    void DLLEXPORT StartPlayFrom();
    void DLLEXPORT RampThink();
    void InitModulationParms();

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];
    int ObjectCaps() override { return (CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION); }

    float m_flAttenuation; // attenuation value
    dynpitchvol_t m_dpv;

    BOOL m_fActive; // only TRUE when the entity is playing a looping sound
    BOOL m_fLooping; // TRUE when the sound played will loop
    edict_t* m_pPlayFrom; //LRC - the entity to play from
    int m_iChannel; //LRC - the channel to play from, for "play from X" sounds
};
