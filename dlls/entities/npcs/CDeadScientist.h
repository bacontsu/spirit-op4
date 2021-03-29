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

#include "entities/CBaseMonster.h"

//=========================================================
// Dead Scientist PROP
//=========================================================
class CDeadScientist : public CBaseMonster
{
public:
    void Spawn() override;
    int Classify() override { return CLASS_HUMAN_PASSIVE; }

    void KeyValue(KeyValueData* pkvd) override;
    int m_iPose; // which sequence to display
    static const char* m_szPoses[7];
};
