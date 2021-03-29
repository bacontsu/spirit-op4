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

#define STOMP_INTERVAL 0.025

class CStomp : public CBaseEntity
{
public:
    void Spawn() override;
    void Think() override;
    static CStomp* StompCreate(const Vector& origin, const Vector& end, float speed);

private:
    // UNDONE: re-use this sprite list instead of creating new ones all the time
    //    CSprite        *m_pSprites[ STOMP_SPRITE_COUNT ];
};
