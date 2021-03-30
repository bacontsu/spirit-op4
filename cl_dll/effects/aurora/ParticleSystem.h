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

#include "particle.h"

#define COLLISION_NONE 0
#define COLLISION_DIE 1
#define COLLISION_BOUNCE 2

class ParticleSystem
{
public:
    ParticleSystem(int entindex, char* szFilename); //int iParticles );
    //	ParticleSystem( int iParticles );
    ~ParticleSystem();
    void AllocateParticles(int iParticles);
    void CalculateDistance();

    //	ParticleType *GetMainType() { return GetType(m_szMainType); }
    ParticleType* GetType(const char* szName);
    ParticleType* AddPlaceholderType(const char* szName);
    ParticleType* ParseType(char*& szFile);

    cl_entity_t* GetEntity() { return gEngfuncs.GetEntityByIndex(m_iEntIndex); }

    static float c_fCosTable[360 + 90];
    static bool c_bCosTableInit;

    // General functions
    bool UpdateSystem(float frametime, /*Vector &right, Vector &up,*/ int messagenum); // If this function returns false, the manager deletes the system
    void DrawSystem(); //Vector &right, Vector &up);
    particle* ActivateParticle(); // adds one of the free particles to the active list, and returns it for initialisation.
    //MUST CHECK WHETHER THIS RESULT IS NULL!

    static float CosLookup(int angle) { return angle < 0 ? c_fCosTable[angle + 360] : c_fCosTable[angle]; }
    static float SinLookup(int angle) { return angle < -90 ? c_fCosTable[angle + 450] : c_fCosTable[angle + 90]; }

    // returns false if the particle has died
    bool UpdateParticle(particle* part, float frametime);
    void DrawParticle(particle* part, Vector& right, Vector& up);

    // Utility functions that have to be public
    //	bool ParticleIsVisible( particle* part );

    // Pointer to next system for linked list structure	
    ParticleSystem* m_pNextSystem;

    particle* m_pActiveParticle;
    float m_fViewerDist;
    int m_iEntIndex;

private:
    // the block of allocated particles
    particle* m_pAllParticles;
    // First particles in the linked list for the active particles and the dead particles
    //	particle*		m_pActiveParticle;
    particle* m_pFreeParticle;
    particle* m_pMainParticle; // the "source" particle.

    ParticleType* m_pFirstType;

    ParticleType* m_pMainType;
    //char m_szMainType[MAX_TYPENAME]; // name of the main particle type
};
