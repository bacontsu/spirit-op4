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

#include "entities/base/CTalkMonster.h"
#include "entities/effects/CBeam.h"

//=========================================================
// Generic Monster - purely for scripted sequence work.
//=========================================================
class CGenericMonster : public CTalkMonster
{
public:
    void Spawn() override;
    void Precache() override;
    void SetYawSpeed() override;
    int Classify() override;
    void HandleAnimEvent(MonsterEvent_t* pEvent) override;
    int ISoundMask() override;
    void KeyValue(KeyValueData* pkvd) override;
    void Torch();
    void MakeGas();
    void UpdateGas();
    void KillGas();

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    int HasCustomGibs() override { return m_iszGibModel; }

    CBeam* m_pBeam;
    int m_iszGibModel;
};
