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

#include "entities/CBaseToggle.h"

// Make this button behave like a door (HACKHACK)
// This will disable use and make the button solid
// rotating buttons were made SOLID_NOT by default since their were some
// collision problems with them...
#define SF_MOMENTARY_DOOR                    0x0001
#define SF_MOMENTARY_ROTBUTTON_AUTO_RETURN   16

class CMomentaryRotButton : public CBaseToggle
{
public:
    void Spawn() override;
    void KeyValue(KeyValueData* pkvd) override;
    int ObjectCaps() override;
    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
    void DLLEXPORT Off();
    void DLLEXPORT Return();
    void UpdateSelf(float value);
    void UpdateSelfReturn(float value);
    void UpdateAllButtons(float value, int start);

    void PlaySound();
    void UpdateTarget(float value);

    static CMomentaryRotButton* Instance(edict_t* pent);

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;

    static TYPEDESCRIPTION m_SaveData[];

    int m_lastUsed;
    int m_direction;
    float m_returnSpeed;
    vec3_t m_start;
    vec3_t m_end;
    int m_sounds;
};
