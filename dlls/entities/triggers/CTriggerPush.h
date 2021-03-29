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

#define SF_TRIGGER_PUSH_ONCE       1
#define SF_TRIGGER_PUSH_START_OFF  2  //spawnflag that makes trigger_push spawn turned OFF

class CTriggerPush : public CBaseTrigger
{
public:
    void Spawn() override;
    void KeyValue(KeyValueData* pkvd) override;
    void Touch(CBaseEntity* pOther) override;

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    int m_iszPushVel;
    int m_iszPushSpeed;
};
