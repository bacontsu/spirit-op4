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

#define MT_AFFECT_X 1
#define MT_AFFECT_Y 2
#define MT_AFFECT_Z 4

#define SF_MTHREAD_STEP 16 // AJH / MJB - 'step' next frame only.

class CMotionThread : public CPointEntity
{
public:
    void Spawn() override; //AJH
    void Think() override;

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    int m_iszPosition;
    int m_iPosMode;
    int m_iPosAxis; //AJH axis to affect
    int m_iszFacing;
    int m_iFaceMode;
    int m_iFaceAxis; //AJH axis to affect
    EHANDLE m_hLocus;
    EHANDLE m_hTarget;
};
