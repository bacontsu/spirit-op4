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

#include "entities/CBaseEntity.h"

#define SF_PENDULUM_INSTANT        1
#define SF_PENDULUM_SWING          2  // spawnflag that makes a pendulum a rope swing.
#define SF_PENDULUM_AUTO_RETURN   16

class CPendulum : public CBaseEntity
{
public:
    void Spawn() override;
    void KeyValue(KeyValueData* pkvd) override;
    void DLLEXPORT SwingThink();
    void DLLEXPORT PendulumUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
    void DLLEXPORT StopThink();
    void Touch(CBaseEntity* pOther) override;
    void DLLEXPORT RopeTouch(CBaseEntity* pOther); // this touch func makes the pendulum a rope
    int ObjectCaps() override;
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    void Blocked(CBaseEntity* pOther) override;
    STATE GetState() override;

    static TYPEDESCRIPTION m_SaveData[];

    float m_accel; // Acceleration
    float m_distance; // 
    float m_time;
    float m_damp;
    float m_maxSpeed;
    float m_dampSpeed;
    Vector m_center;
    Vector m_start;

    EHANDLE m_hActivator; //AJH (give frags to this entity)
};
