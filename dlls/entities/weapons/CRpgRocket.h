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
#include "CRpg.h"

class CRpgRocket : public CGrenade
{
public:
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];
    void Spawn() override;
    void Precache() override;
    void DLLEXPORT FollowThink();
    void DLLEXPORT IgniteThink();
    void DLLEXPORT RocketTouch(CBaseEntity* pOther);
    static CRpgRocket* CreateRpgRocket(Vector vecOrigin, Vector vecAngles, CBaseEntity* pOwner, CRpg* pLauncher);

    int m_iTrail;
    float m_flIgniteTime;
    EHANDLE m_pLauncher; // pointer back to the launcher that fired me. 
};
