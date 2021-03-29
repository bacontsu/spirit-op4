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

#include "entities/CBaseMonster.h"

#define AFLOCK_MAX_RECRUIT_RADIUS  1024
#define AFLOCK_FLY_SPEED           125
#define AFLOCK_TURN_RATE           75
#define AFLOCK_ACCELERATE          10
#define AFLOCK_CHECK_DIST          192
#define AFLOCK_TOO_CLOSE           100
#define AFLOCK_TOO_FAR             256

class CFlockingFlyer : public CBaseMonster
{
public:
    void Spawn() override;
    void Precache() override;
    void SpawnCommonCode();
    void DLLEXPORT IdleThink();
    void BoidAdvanceFrame();
    void DLLEXPORT FormFlock();
    void DLLEXPORT Start();
    void DLLEXPORT FlockLeaderThink();
    void DLLEXPORT FlockFollowerThink();
    void DLLEXPORT FallHack();
    void MakeSound();
    void AlertFlock();
    void SpreadFlock();
    void SpreadFlock2();
    void Killed(entvars_t* pevAttacker, int iGib) override;
    void Poop();
    BOOL FPathBlocked();
    //void KeyValue( KeyValueData *pkvd );

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    int IsLeader() { return m_pSquadLeader == this; }
    int InSquad() { return m_pSquadLeader != NULL; }
    int SquadCount();
    void SquadRemove(CFlockingFlyer* pRemove);
    void SquadUnlink();
    void SquadAdd(CFlockingFlyer* pAdd);
    void SquadDisband();

    CFlockingFlyer* m_pSquadLeader;
    CFlockingFlyer* m_pSquadNext;
    BOOL m_fTurning; // is this boid turning?
    BOOL m_fCourseAdjust; // followers set this flag TRUE to override flocking while they avoid something
    BOOL m_fPathBlocked; // TRUE if there is an obstacle ahead
    Vector m_vecReferencePoint; // last place we saw leader
    Vector m_vecAdjustedVelocity; // adjusted velocity (used when fCourseAdjust is TRUE)
    float m_flGoalSpeed;
    float m_flLastBlockedTime;
    float m_flFakeBlockedTime;
    float m_flAlertTime;
    float m_flFlockNextSoundTime;
};
