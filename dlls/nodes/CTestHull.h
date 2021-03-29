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
// TestHull is a modelless clip hull that verifies reachable
// nodes by walking from every node to each of it's connections
//=========================================================
class CTestHull : public CBaseMonster
{
public:
    void Spawn(entvars_t* pevMasterNode);
    int ObjectCaps() override { return CBaseMonster::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }
    void DLLEXPORT CallBuildNodeGraph();
    void BuildNodeGraph();
    void DLLEXPORT ShowBadNode();
    void DLLEXPORT DropDelay();
    void DLLEXPORT PathFind();

    Vector vecBadNodeOrigin;
};
