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

// pev->dmg_take is duration
// pev->dmg_save is hold duration
#define SF_FADE_IN                0x0001        // Fade in, not out
#define SF_FADE_MODULATE        0x0002        // Modulate, don't blend
#define SF_FADE_ONLYONE            0x0004
#define SF_FADE_PERMANENT        0x0008        //LRC - hold permanently
#define SF_FADE_CAMERA            0x0010    //fading only for camera

class CFade : public CPointEntity
{
public:
    void Spawn(void) override;
    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
    void KeyValue(KeyValueData* pkvd) override;

    STATE GetState(void) override { return m_iState; } // LRC
    void Think(void) override; //LRC

    inline float Duration(void) { return pev->dmg_take; }
    inline float HoldTime(void) { return pev->dmg_save; }

    inline void SetDuration(float duration) { pev->dmg_take = duration; }
    inline void SetHoldTime(float hold) { pev->dmg_save = hold; }

    STATE m_iState; // LRC. Don't saverestore this value, it's not worth it.
private:
};
