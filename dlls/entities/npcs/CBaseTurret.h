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
#include "entities/effects/CSprite.h"

#define TURRET_SHOTS             2
#define TURRET_RANGE            (100 * 12)
#define TURRET_SPREAD           Vector( 0, 0, 0 )
#define TURRET_TURNRATE         30                 // angles per 0.1 second
#define TURRET_MAXWAIT          15                 // seconds turret will stay active w/o a target
#define TURRET_MAXSPIN          5                  // seconds turret barrel will spin w/o a target
#define TURRET_MACHINE_VOLUME   0.5
#define TURRET_GLOW_SPRITE      "sprites/flare3.spr"

typedef enum
{
    TURRET_ANIM_NONE = 0,
    TURRET_ANIM_FIRE,
    TURRET_ANIM_SPIN,
    TURRET_ANIM_DEPLOY,
    TURRET_ANIM_RETIRE,
    TURRET_ANIM_DIE,
} TURRET_ANIM;

//
// TODO: 
//        Take advantage of new monster fields like m_hEnemy and get rid of that OFFSET() stuff
//        Revisit enemy validation stuff, maybe it's not necessary with the newest monster code
//

class CBaseTurret : public CBaseMonster
{
public:
    void Spawn(void) override;
    void Precache(void) override;
    void KeyValue(KeyValueData* pkvd) override;
    void DLLEXPORT TurretUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);

    void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr, int bitsDamageType) override;
    int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
    int Classify(void) override;

    int BloodColor(void) override { return DONT_BLEED; }

    void GibMonster(void) override
    {
    } // UNDONE: Throw turret gibs?

    // Think functions

    void DLLEXPORT ActiveThink(void);
    void DLLEXPORT SearchThink(void);
    void DLLEXPORT AutoSearchThink(void);
    void DLLEXPORT TurretDeath(void);

    virtual void DLLEXPORT SpinDownCall(void) { m_iSpin = 0; }
    virtual void DLLEXPORT SpinUpCall(void) { m_iSpin = 1; }

    // void SpinDown(void);
    // float DLLEXPORT SpinDownCall( void ) { return SpinDown(); }

    // virtual float SpinDown(void) { return 0;}
    // virtual float Retire(void) { return 0;}

    void DLLEXPORT Deploy(void);
    void DLLEXPORT Retire(void);

    void DLLEXPORT Initialize(void);

    virtual void Ping(void);
    virtual void EyeOn(void);
    virtual void EyeOff(void);

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;

    static TYPEDESCRIPTION m_SaveData[];

    // other functions
    void SetTurretAnim(TURRET_ANIM anim);
    int MoveTurret(void);

    virtual void Shoot(Vector& vecSrc, Vector& vecDirToEnemy) {}

    float m_flMaxSpin; // Max time to spin the barrel w/o a target
    int m_iSpin;

    CSprite* m_pEyeGlow;
    int m_eyeBrightness;

    int m_iDeployHeight;
    int m_iRetractHeight;
    int m_iMinPitch;

    int m_iBaseTurnRate; // angles per second
    float m_fTurnRate; // actual turn rate
    int m_iOrientation; // 0 = floor, 1 = Ceiling
    int m_iOn;

    virtual STATE getState()
    {
        if (m_iOn) { return STATE_ON; }
        else { return STATE_OFF; }
    }

    int m_fBeserk; // Sometimes this bitch will just freak out
    int m_iAutoStart; // true if the turret auto deploys when a target
    // enters its range

    Vector m_vecLastSight;
    float m_flLastSight; // Last time we saw a target
    float m_flMaxWait; // Max time to seach w/o a target
    int m_iSearchSpeed; // Not Used!

    // movement
    float m_flStartYaw;
    Vector m_vecCurAngles;
    Vector m_vecGoalAngles;


    float m_flPingTime; // Time until the next ping, used when searching
    float m_flSpinUpTime; // Amount of time until the barrel should spin down when searching
};
