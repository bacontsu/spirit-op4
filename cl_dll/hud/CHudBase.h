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

#ifndef _WIN32
#define _cdecl 
#endif

#include "parsemsg.h"
#include "../cl_util.h"

class CHudBase
{
public:
    point_t m_pos;
    int m_type;
    int m_iFlags; // active, moving, 
    virtual ~CHudBase() = default;

    virtual int Init() { return 0; }
    virtual int VidInit() { return 0; }
    virtual int Draw(float flTime) { return 0; }
    virtual void Think() {}
    virtual void Reset() {}

    // called every time a server is connected to
    virtual void InitHUDData() {}
};
