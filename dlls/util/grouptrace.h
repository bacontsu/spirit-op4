/***
*
* Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*
* This product contains software technology licensed from Id
* Software, Inc. (Id Technology").  Id Technology (c) 1996 Id Software, Inc.
* All Rights Reserved.
*
* Use, distribution, and modification of this source code and/or resulting
* object code is restricted to non-commercial enhancements to products from
* Valve LLC.All other use, distribution, or modification is prohibited
* without written permission from Valve LLC.
*
****/
#pragma once

#define GROUP_OP_AND   0
#define GROUP_OP_NAND  1

extern int g_groupmask;
extern int g_groupop;

class UTIL_GroupTrace
{
public:
    UTIL_GroupTrace(int groupmask, int op);
    ~UTIL_GroupTrace();

private:
    int m_oldgroupmask, m_oldgroupop;
};

void UTIL_SetGroupTrace(int groupmask, int op);
void UTIL_UnsetGroupTrace();
