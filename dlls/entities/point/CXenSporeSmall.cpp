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
* Valve LLC.  All other use, distribution, or modification is prohibited
* without written permission from Valve LLC.
*
****/

#include "CXenSporeSmall.h"

LINK_ENTITY_TO_CLASS(xen_spore_small, CXenSporeSmall);

void CXenSporeSmall::Spawn()
{
    pev->skin = 0;
    CXenSpore::Spawn();
    UTIL_SetSize(pev, Vector(-16, -16, 0), Vector(16, 16, 64));
}
