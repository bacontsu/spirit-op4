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

#include "entities/base/CTalkMonster.h"

#define NUM_SCIENTIST_HEADS  4 // four heads available for scientist model

enum { HEAD_GLASSES = 0, HEAD_EINSTEIN = 1, HEAD_LUTHER = 2, HEAD_SLICK = 3 };

//=======================================================
// Scientist
//=======================================================
class CScientist : public CTalkMonster
{
public:
    void Spawn(void) override;
    void Precache(void) override;

    void SetYawSpeed(void) override;
    int Classify(void) override;
    void HandleAnimEvent(MonsterEvent_t* pEvent) override;
    void RunTask(Task_t* pTask) override;
    void StartTask(Task_t* pTask) override;
    int ObjectCaps(void) override { return CTalkMonster::ObjectCaps() | FCAP_IMPULSE_USE; }
    int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
    int FriendNumber(int arrayNumber) override;
    void SetActivity(Activity newActivity) override;
    Activity GetStoppedActivity(void) override;
    int ISoundMask(void) override;
    void DeclineFollowing(void) override;

    float CoverRadius(void) override { return 1200; } // Need more room for cover because scientists want to get far away!
    BOOL DisregardEnemy(CBaseEntity* pEnemy) { return !pEnemy->IsAlive() || (gpGlobals->time - m_fearTime) > 15; }

    BOOL CanHeal(void);
    void Heal(void);
    void Scream(void);

    // Override these to set behavior
    Schedule_t* GetScheduleOfType(int Type) override;
    Schedule_t* GetSchedule(void) override;
    MONSTERSTATE GetIdealState(void) override;

    void DeathSound(void) override;
    void PainSound(void) override;

    void TalkInit(void);

    void Killed(entvars_t* pevAttacker, int iGib) override;

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    CUSTOM_SCHEDULES;

private:
    float m_painTime;
    float m_healTime;
    float m_fearTime;
};
