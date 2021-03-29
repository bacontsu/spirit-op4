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

class CMotionThread;

#define SF_MOTION_DEBUG 1
#define SF_MOTION_SWAPXY 2 // MJB Swap X and Y values
#define SF_MOTION_SWAPYZ 4 // MJB Swap Y and Z values
#define SF_MOTION_SWAPZX 8 // MJB Swap Z and X values

//===========================================================
//LRC- motion_manager
//===========================================================
class CMotionManager : public CPointEntity
{
public:
    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
    void KeyValue(KeyValueData* pkvd) override;
    void Affect(CBaseEntity* pTarget, CBaseEntity* pActivator);
    void PostSpawn() override;

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    int m_iszPosition;
    int m_iPosMode;
    int m_iPosAxis; //AJH
    int m_iszFacing;
    int m_iFaceMode;
    int m_iFaceAxis; //AJH
    CMotionThread* pThread;
};
