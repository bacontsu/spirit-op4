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

#include "CTripBeam.h"

// UNDONE: Jay -- CTripBeam is only a test
#if _DEBUG

LINK_ENTITY_TO_CLASS(trip_beam, CTripBeam);

void CTripBeam::Spawn()
{
    CLightning::Spawn();
    SetTouch(&CTripBeam::TriggerTouch);
    pev->solid = SOLID_TRIGGER;
    RelinkBeam();
}

#endif
