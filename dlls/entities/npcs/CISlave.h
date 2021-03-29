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

#include "entities/base/CSquadMonster.h"
#include "entities/effects/CBeam.h"
#include "monsters.h"

#define ISLAVE_MAX_BEAMS    8

//=========================================================
// Alien slave monster
//=========================================================
class CISlave : public CSquadMonster
{
public:
    void Spawn() override;
    void Precache() override;
    void SetYawSpeed() override;
    int ISoundMask() override;
    int Classify() override;
    int IRelationship(CBaseEntity* pTarget) override;
    void HandleAnimEvent(MonsterEvent_t* pEvent) override;
    BOOL CheckRangeAttack1(float flDot, float flDist) override;
    BOOL CheckRangeAttack2(float flDot, float flDist) override;
    void CallForHelp(const char* szClassname, float flDist, EHANDLE hEnemy, Vector& vecLocation);
    void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr, int bitsDamageType) override;
    int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;

    void DeathSound() override;
    void PainSound() override;
    void AlertSound() override;
    void IdleSound() override;

    void Killed(entvars_t* pevAttacker, int iGib) override;

    void StartTask(Task_t* pTask) override;
    Schedule_t* GetSchedule() override;
    Schedule_t* GetScheduleOfType(int Type) override;
    CUSTOM_SCHEDULES;

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    void ClearBeams();
    void ArmBeam(int side);
    void WackBeam(int side, CBaseEntity* pEntity);
    void ZapBeam(int side);
    void BeamGlow();

    int m_iBravery;

    CBeam* m_pBeam[ISLAVE_MAX_BEAMS];

    int m_iBeams;
    float m_flNextAttack;

    int m_voicePitch;

    EHANDLE m_hDead;

    static const char* pAttackHitSounds[];
    static const char* pAttackMissSounds[];
    static const char* pPainSounds[];
    static const char* pDeathSounds[];
};
