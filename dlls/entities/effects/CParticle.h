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

//=========================================================
// LRC - env_particle, uses the aurora particle system
//=========================================================
//extern int gmsgParticle = 0;
#define SF_PARTICLE_ON 1
#define SF_PARTICLE_SPAWNUSE 2    //AJH for spawnable env_particles

class CParticle : public CPointEntity
{
public:
    void Spawn() override;
    void Activate() override;
    void Precache() override;
    void DesiredAction() override;
    void DLLEXPORT Think() override;

    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
};
