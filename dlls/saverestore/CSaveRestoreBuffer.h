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

#include "extdll.h"
#include "util.h"

#define MAX_ENTITYARRAY 64

class CSaveRestoreBuffer
{
public:
    CSaveRestoreBuffer();
    CSaveRestoreBuffer(SAVERESTOREDATA* pdata);
    ~CSaveRestoreBuffer();

    int EntityIndex(entvars_t* pevLookup);
    int EntityIndex(edict_t* pentLookup);
    int EntityIndex(EOFFSET eoLookup);
    int EntityIndex(CBaseEntity* pEntity);

    int EntityFlags(int entityIndex, int flags) { return EntityFlagsSet(entityIndex, 0); }
    int EntityFlagsSet(int entityIndex, int flags);

    edict_t* EntityFromIndex(int entityIndex);

    unsigned short TokenHash(const char* pszToken);

protected:
    SAVERESTOREDATA* m_pdata;
    void BufferRewind(int size);
    unsigned int HashString(const char* pszToken);
};
