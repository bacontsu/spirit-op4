/***
*
*    Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
*    This product contains software technology licensed from Id
*    Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*    All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#pragma once

#include "entities/CBaseDelay.h"
#include "entities/base/CPointEntity.h"
#include "util/sound.h"

#define SF_BREAK_TRIGGER_ONLY      1  // may only be broken by trigger
#define SF_BREAK_TOUCH             2  // can be 'crashed through' by running player (plate glass)
#define SF_BREAK_PRESSURE          4  // can be broken by a player standing on it
#define SF_BREAK_FADE_RESPAWN      8  // LRC- fades in gradually when respawned
#define SF_BREAK_CROWBAR         256  // instant break if hit with crowbar

typedef enum { expRandom, expDirected } Explosions;

typedef enum
{
    matGlass = 0,
    matWood,
    matMetal,
    matFlesh,
    matCinderBlock,
    matCeilingTile,
    matComputer,
    matUnbreakableGlass,
    matRocks,
    matNone,
    matLastMaterial
} Materials;

#define    NUM_SHARDS 6 // this many shards spawned when breakable objects break;
#define SF_BREAKABLE_INVERT 16

class CBreakable : public CBaseDelay
{
public:
    // basic functions
    void Spawn() override;
    void Precache() override;
    void KeyValue(KeyValueData* pkvd) override;
    bool CalcNumber(CBaseEntity* pLocus, float* OUTresult) override;
    void DLLEXPORT BreakTouch(CBaseEntity* pOther);
    void DLLEXPORT BreakUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
    void DLLEXPORT RespawnUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
    void DLLEXPORT RespawnThink();
    void DLLEXPORT RespawnFadeThink();
    void DamageSound();
    virtual void DoRespawn(); //AJH Fix for respawnable breakable pushables
    int Classify() override;

    // breakables use an overridden takedamage
    int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
    // To spark when hit
    void TraceAttack(entvars_t* pevAttacker, float flDamage, Vector vecDir, TraceResult* ptr, int bitsDamageType) override;

    BOOL IsBreakable();
    BOOL SparkWhenHit();

    STATE GetState() override;

    int DamageDecal(int bitsDamageType) override;

    void DLLEXPORT Die();

    int ObjectCaps() override;
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;

    inline BOOL Explodable();
    inline int ExplosionMagnitude();
    inline void ExplosionSetMagnitude(int magnitude);

    static void MaterialSoundPrecache(Materials precacheMaterial);
    static void MaterialSoundRandom(edict_t* pEdict, Materials soundMaterial, float volume);
    static const char** MaterialSoundList(Materials precacheMaterial, int& soundCount);

    static const char* pSoundsWood[];
    static const char* pSoundsFlesh[];
    static const char* pSoundsGlass[];
    static const char* pSoundsMetal[];
    static const char* pSoundsConcrete[];
    static const char* pSpawnObjects[];

    static TYPEDESCRIPTION m_SaveData[];

    Materials m_Material;
    Explosions m_Explosion;
    int m_idShard;
    float m_angle;
    int m_iszGibModel;
    int m_iszSpawnObject;
    //LRC
    int m_iRespawnTime;
    int m_iInitialHealth;
    int m_iInitialRenderAmt;
    int m_iInitialRenderMode;
    int m_iClass; //so that monsters will attack it
    int m_iszWhenHit; // locus trigger
    CPointEntity* m_pHitProxy;
};
