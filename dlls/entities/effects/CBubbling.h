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

#define SF_BUBBLES_STARTOFF  0x0001

class CBubbling : public CBaseEntity
{
public:
    void Spawn(void) override;
    void Precache(void) override;
    void KeyValue(KeyValueData* pkvd) override;

    void EXPORT FizzThink(void);
    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;

    int ObjectCaps(void) override { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    int m_density;
    int m_frequency;
    int m_bubbleModel;
    int m_state;

    STATE GetState(void) override { return m_state ? STATE_ON : STATE_OFF; };
};
