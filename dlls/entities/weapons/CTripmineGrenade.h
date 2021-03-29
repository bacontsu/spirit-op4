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

#include "CGrenade.h"
#include "entities/effects/CBeam.h"

class CTripmineGrenade : public CGrenade
{
    void Spawn() override;
    void Precache() override;

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;

    static TYPEDESCRIPTION m_SaveData[];

    int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;

    void DLLEXPORT WarningThink();
    void DLLEXPORT PowerupThink();
    void DLLEXPORT BeamBreakThink();
    void DLLEXPORT DelayDeathThink();
    void Killed(entvars_t* pevAttacker, int iGib) override;

    void MakeBeam();
    void KillBeam();

    float m_flPowerUp;
    Vector m_vecDir;
    Vector m_vecEnd;
    float m_flBeamLength;

    EHANDLE m_hOwner;
    CBeam* m_pBeam;
    Vector m_posOwner;
    Vector m_angleOwner;
    edict_t* m_pRealOwner; // tracelines don't hit PEV->OWNER, which means a player couldn't detonate his own trip mine, so we store the owner here.
};
