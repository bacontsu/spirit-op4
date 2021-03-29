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

#include "entities/CBaseEntity.h"

//LRC-  RenderFxFader, a subsidiary entity for RenderFxManager
class CRenderFxFader : public CBaseEntity
{
public:
    void Spawn() override;
    void DLLEXPORT FadeThink();
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    int ObjectCaps() override { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

    static TYPEDESCRIPTION m_SaveData[];

    float m_flStartTime;
    float m_flDuration;
    float m_flCoarseness;
    int m_iStartAmt;
    int m_iOffsetAmt;
    Vector m_vecStartColor;
    Vector m_vecOffsetColor;
    float m_fStartScale;
    float m_fOffsetScale;
    EHANDLE m_hTarget;

    int m_iszAmtFactor;
};
