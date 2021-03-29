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

#include "entities/CBaseToggle.h"

#define SF_SENTENCE_ONCE         0x0001
#define SF_SENTENCE_FOLLOWERS    0x0002    // only say if following player
#define SF_SENTENCE_INTERRUPT    0x0004    // force talking except when dead
#define SF_SENTENCE_CONCURRENT   0x0008    // allow other people to keep talking

class CScriptedSentence : public CBaseToggle
{
public:
    void Spawn() override;
    void KeyValue(KeyValueData* pkvd) override;
    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
    void DLLEXPORT FindThink();
    void DLLEXPORT DelayThink();
    void DLLEXPORT DurationThink();
    int ObjectCaps() override { return (CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION); }

    STATE GetState() override { return m_playing ? STATE_ON : STATE_OFF; }

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;

    static TYPEDESCRIPTION m_SaveData[];

    CBaseMonster* FindEntity(CBaseEntity* pActivator);
    BOOL AcceptableSpeaker(CBaseMonster* pMonster);
    BOOL StartSentence(CBaseMonster* pTarget);


private:
    int m_iszSentence; // string index for idle animation
    int m_iszEntity; // entity that is wanted for this sentence
    float m_flRadius; // range to search
    float m_flDuration; // How long the sentence lasts
    float m_flRepeat; // maximum repeat rate
    float m_flAttenuation;
    float m_flVolume;
    BOOL m_active; // is the sentence enabled? (for m_flRepeat)
    BOOL m_playing; //LRC- is the sentence playing? (for GetState)
    int m_iszListener; // name of entity to look at while talking
};
