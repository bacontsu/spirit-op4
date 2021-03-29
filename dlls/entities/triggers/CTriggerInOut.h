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

#include "CBaseTrigger.h"
#include "CInOutRegister.h"

class CTriggerInOut : public CBaseTrigger
{
public:
    void Spawn() override;
    void DLLEXPORT Touch(CBaseEntity* pOther) override;
    void DLLEXPORT Think() override;
    void FireOnEntry(CBaseEntity* pOther);
    void FireOnLeaving(CBaseEntity* pOther);

    void KeyValue(KeyValueData* pkvd) override;
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    STATE GetState() override { return m_pRegister->IsEmpty() ? STATE_OFF : STATE_ON; }

    //LRC 1.8 - let it act as an alias that refers to the entities within it
    CBaseEntity* FollowAlias(CBaseEntity* pFrom) override;

    string_t m_iszAltTarget;
    string_t m_iszBothTarget;
    CInOutRegister* m_pRegister;
};
