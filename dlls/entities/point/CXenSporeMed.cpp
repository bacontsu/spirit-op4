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

#include "CXenSporeMed.h"

LINK_ENTITY_TO_CLASS(xen_spore_medium, CXenSporeMed);

void CXenSporeMed::Spawn()
{
    pev->skin = 1;
    CXenSpore::Spawn();
    UTIL_SetSize(pev, Vector(-40, -40, 0), Vector(40, 40, 120));
}
