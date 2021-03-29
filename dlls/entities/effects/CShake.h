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

#include "entities/base/CPointEntity.h"

// Screen shake
class CShake : public CPointEntity
{
public:
    void Spawn() override;
    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
    void KeyValue(KeyValueData* pkvd) override;

    inline float Amplitude() { return pev->scale; }
    inline float Frequency() { return pev->dmg_save; }
    inline float Duration() { return pev->dmg_take; }
    inline float Radius() { return pev->dmg; }

    inline void SetAmplitude(float amplitude) { pev->scale = amplitude; }
    inline void SetFrequency(float frequency) { pev->dmg_save = frequency; }
    inline void SetDuration(float duration) { pev->dmg_take = duration; }
    inline void SetRadius(float radius) { pev->dmg = radius; }

    STATE m_iState; //LRC
    STATE GetState() override { return m_iState; } //LRC
    void Think() override { m_iState = STATE_OFF; } //LRC
private:
};
