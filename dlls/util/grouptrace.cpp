/***
*
* Copyright (c) 1996-2001, Valve LLC. All rights reserved.
* 
* This product contains software technology licensed from Id
* Software, Inc. ("Id Technology).  Id Technology (c) 1996 Id Software, Inc.
* All Rights Reserved.
*
* Use, distribution, and modification of this source code and/or resulting
* object code is restricted to non-commercial enhancements to products from
* Valve LLC.  All other use, distribution, or modification is prohibited
* without written permission from Valve LLC.
*
****/

#include "extdll.h"
#include "util.h"
#include "grouptrace.h"

int g_groupmask = 0;
int g_groupop = 0;

// Normal overrides
void UTIL_SetGroupTrace(int groupmask, int op)
{
    g_groupmask = groupmask;
    g_groupop = op;

    ENGINE_SETGROUPMASK(g_groupmask, g_groupop);
}

void UTIL_UnsetGroupTrace()
{
    g_groupmask = 0;
    g_groupop = 0;

    ENGINE_SETGROUPMASK(0, 0);
}

// Smart version, it'll clean itself up when it pops off stack
UTIL_GroupTrace::UTIL_GroupTrace(int groupmask, int op)
{
    m_oldgroupmask = g_groupmask;
    m_oldgroupop = g_groupop;

    g_groupmask = groupmask;
    g_groupop = op;

    ENGINE_SETGROUPMASK(g_groupmask, g_groupop);
}

UTIL_GroupTrace::~UTIL_GroupTrace()
{
    g_groupmask = m_oldgroupmask;
    g_groupop = m_oldgroupop;

    ENGINE_SETGROUPMASK(g_groupmask, g_groupop);
}
