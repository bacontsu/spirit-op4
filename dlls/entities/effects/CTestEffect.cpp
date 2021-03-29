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

#include "CTestEffect.h"
#include "CBeam.h"

LINK_ENTITY_TO_CLASS(test_effect, CTestEffect);

void CTestEffect::Spawn()
{
    Precache();
}

void CTestEffect::Precache()
{
    PRECACHE_MODEL("sprites/lgtning.spr");
}

void CTestEffect::TestThink()
{
    int i;
    float t = (gpGlobals->time - m_flStartTime);

    if (m_iBeam < 24)
    {
        CBeam* pbeam = CBeam::BeamCreate("sprites/lgtning.spr", 100);

        TraceResult tr;

        Vector vecSrc = pev->origin;
        Vector vecDir = Vector(RANDOM_FLOAT(-1.0, 1.0), RANDOM_FLOAT(-1.0, 1.0), RANDOM_FLOAT(-1.0, 1.0));
        vecDir = vecDir.Normalize();
        UTIL_TraceLine(vecSrc, vecSrc + vecDir * 128, ignore_monsters, ENT(pev), &tr);

        pbeam->PointsInit(vecSrc, tr.vecEndPos);
        // pbeam->SetColor( 80, 100, 255 );
        pbeam->SetColor(255, 180, 100);
        pbeam->SetWidth(100);
        pbeam->SetScrollRate(12);

        m_flBeamTime[m_iBeam] = gpGlobals->time;
        m_pBeam[m_iBeam] = pbeam;
        m_iBeam++;

#if 0
        Vector vecMid = (vecSrc + tr.vecEndPos) * 0.5;
        MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY);
        WRITE_BYTE(TE_DLIGHT);
        WRITE_COORD(vecMid.x);    // X
        WRITE_COORD(vecMid.y);    // Y
        WRITE_COORD(vecMid.z);    // Z
        WRITE_BYTE(20);        // radius * 0.1
        WRITE_BYTE(255);        // r
        WRITE_BYTE(180);        // g
        WRITE_BYTE(100);        // b
        WRITE_BYTE(20);        // time * 10
        WRITE_BYTE(0);        // decay * 0.1
        MESSAGE_END();
#endif
    }

    if (t < 3.0)
    {
        for (i = 0; i < m_iBeam; i++)
        {
            t = (gpGlobals->time - m_flBeamTime[i]) / (3 + m_flStartTime - m_flBeamTime[i]);
            m_pBeam[i]->SetBrightness(255 * t);
            // m_pBeam[i]->SetScrollRate( 20 * t );
        }
        SetNextThink(0.1);
    }
    else
    {
        for (i = 0; i < m_iBeam; i++)
        {
            UTIL_Remove(m_pBeam[i]);
        }
        m_flStartTime = gpGlobals->time;
        m_iBeam = 0;
        SetThink(NULL);
    }
}


void CTestEffect::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
    SetThink(&CTestEffect::TestThink);
    SetNextThink(0.1);
    m_flStartTime = gpGlobals->time;
}
