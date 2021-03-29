/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*
*	This product contains software technology licensed from Id
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#pragma once

#include <cl_entity.h>

#include "ParticleSystem.h"

class ParticleSystemManager
{
public:
    ParticleSystemManager();
    ~ParticleSystemManager();
    void AddSystem(ParticleSystem*);
    ParticleSystem* FindSystem(cl_entity_t* pEntity);
    void UpdateSystems(float frametime);
    void ClearSystems();
    void SortSystems();
    //	void		DeleteSystem( ParticleSystem* );

    //private:
    ParticleSystem* m_pFirstSystem;
    //ParticleSystem* systemio;
};

extern ParticleSystemManager* g_pParticleSystems;
