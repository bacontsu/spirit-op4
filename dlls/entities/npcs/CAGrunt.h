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
#include "monsters.h"

//=========================================================
// Agrunt - Dominant, warlike alien grunt monster
//=========================================================
class CAGrunt : public CSquadMonster
{
public:
    void Spawn() override;
    void Precache() override;
    void SetYawSpeed() override;
    int Classify() override;
    int ISoundMask() override;
    void HandleAnimEvent(MonsterEvent_t* pEvent) override;

    void SetObjectCollisionBox() override
    {
        pev->absmin = pev->origin + Vector(-32, -32, 0);
        pev->absmax = pev->origin + Vector(32, 32, 85);
    }

    Schedule_t* GetSchedule() override;
    Schedule_t* GetScheduleOfType(int Type) override;
    BOOL FCanCheckAttacks() override;
    BOOL CheckMeleeAttack1(float flDot, float flDist) override;
    BOOL CheckRangeAttack1(float flDot, float flDist) override;
    void StartTask(Task_t* pTask) override;
    void AlertSound() override;
    void DeathSound() override;
    void PainSound() override;
    void AttackSound();
    void PrescheduleThink() override;
    void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr, int bitsDamageType) override;
    int IRelationship(CBaseEntity* pTarget) override;
    void StopTalking();
    BOOL ShouldSpeak();
    void Killed(entvars_t* pevAttacker, int iGib) override;

    CUSTOM_SCHEDULES;

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    static const char* pAttackHitSounds[];
    static const char* pAttackMissSounds[];
    static const char* pAttackSounds[];
    static const char* pDieSounds[];
    static const char* pPainSounds[];
    static const char* pIdleSounds[];
    static const char* pAlertSounds[];

    BOOL m_fCanHornetAttack;
    float m_flNextHornetAttackCheck;

    float m_flNextPainTime;

    // three hacky fields for speech stuff. These don't really need to be saved.
    float m_flNextSpeakTime;
    float m_flNextWordTime;
    int m_iLastWord;
};
