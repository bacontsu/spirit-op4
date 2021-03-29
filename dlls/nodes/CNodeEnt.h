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

//=========================================================
// hints - these MUST coincide with the HINTS listed under
// info_node in the FGD file!
//=========================================================
enum
{
    HINT_NONE = 0,
    HINT_WORLD_DOOR,
    HINT_WORLD_WINDOW,
    HINT_WORLD_BUTTON,
    HINT_WORLD_MACHINERY,
    HINT_WORLD_LEDGE,
    HINT_WORLD_LIGHT_SOURCE,
    HINT_WORLD_HEAT_SOURCE,
    HINT_WORLD_BLINKING_LIGHT,
    HINT_WORLD_BRIGHT_COLORS,
    HINT_WORLD_HUMAN_BLOOD,
    HINT_WORLD_ALIEN_BLOOD,

    HINT_TACTICAL_EXIT = 100,
    HINT_TACTICAL_VANTAGE,
    HINT_TACTICAL_AMBUSH,

    HINT_STUKA_PERCH = 300,
    HINT_STUKA_LANDING,
};

//=========================================================
// Nodes start out as ents in the level. The node graph 
// is built, then these ents are discarded. 
//=========================================================
class CNodeEnt : public CBaseEntity
{
    void Spawn() override;
    void KeyValue(KeyValueData* pkvd) override;
    int ObjectCaps() override { return CBaseEntity::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

    short m_sHintType;
    short m_sHintActivity;
};
