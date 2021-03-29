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
* Valve LLC.  All other use, distribution, or modification is prohibited
* without written permission from Valve LLC.
*
****/

#include "CHudParticle.h"
#include "CHud.h"
#include "effects/aurora/ParticleSystemManager.h"
#include "effects/aurora/ParticleSystem.h"

DECLARE_MESSAGE(m_Particle, Particle)

int CHudParticle::Init()
{
    //ConsolePrint("Hooking Particle message\n"); // 30/08/02 November235: Just a debug
    HOOK_MESSAGE(Particle);
    gHUD.AddHudElem(this);

    if (g_pParticleSystems)
    {
        delete g_pParticleSystems;
        g_pParticleSystems = NULL;
    }

    g_pParticleSystems = new ParticleSystemManager();

    return 1;
};

int CHudParticle::VidInit()
{
    g_pParticleSystems->ClearSystems();
    return 1;
};

int CHudParticle::MsgFunc_Particle(const char* pszName, int iSize, void* pbuf)
{
    BEGIN_READ(pbuf, iSize);
    //	int type = READ_BYTE();
    int entindex = READ_SHORT();
    //	float x = READ_COORD();
    //	float y = READ_COORD();
    //	float z = READ_COORD();
    //	float ax = 0;//READ_COORD();
    //	float ay = 0;//READ_COORD();
    //	float az = 0;//READ_COORD();
    char* sz = READ_STRING();
    //	gEngfuncs.Con_Printf("Message received\n");
    //char sz[255];

    ParticleSystem* pSystem = new ParticleSystem(entindex, sz); //"aurora/smoke.aur");

    g_pParticleSystems->AddSystem(pSystem);

    return 1;
}

int CHudParticle::Draw(float flTime)
{
    return 1;
}
