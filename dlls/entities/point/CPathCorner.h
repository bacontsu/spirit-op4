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
#include "util/saverestore.h"

class CPathCorner : public CPointEntity
{
public:
    void Spawn() override;
    void KeyValue(KeyValueData* pkvd) override;
    float GetDelay() override { return m_flWait; }
    //    void Touch( CBaseEntity *pOther );
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;

    static TYPEDESCRIPTION m_SaveData[];

private:
    float m_flWait;
};
