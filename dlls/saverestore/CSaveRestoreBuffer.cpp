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

#include "CSaveRestoreBuffer.h"
#include "entities/CBaseEntity.h"


// Base class includes common SAVERESTOREDATA pointer, and manages the entity table
CSaveRestoreBuffer::CSaveRestoreBuffer()
{
    m_pdata = NULL;
}


CSaveRestoreBuffer::CSaveRestoreBuffer(SAVERESTOREDATA* pdata)
{
    m_pdata = pdata;
}


CSaveRestoreBuffer::~CSaveRestoreBuffer()
{
}

int CSaveRestoreBuffer::EntityIndex(CBaseEntity* pEntity)
{
    if (pEntity == NULL)
        return -1;
    return EntityIndex(pEntity->pev);
}


int CSaveRestoreBuffer::EntityIndex(entvars_t* pevLookup)
{
    if (pevLookup == NULL)
        return -1;
    return EntityIndex(ENT(pevLookup));
}

int CSaveRestoreBuffer::EntityIndex(EOFFSET eoLookup)
{
    return EntityIndex(ENT(eoLookup));
}


int CSaveRestoreBuffer::EntityIndex(edict_t* pentLookup)
{
    if (!m_pdata || pentLookup == NULL)
        return -1;

    int i;
    ENTITYTABLE* pTable;

    for (i = 0; i < m_pdata->tableCount; i++)
    {
        pTable = m_pdata->pTable + i;
        if (pTable->pent == pentLookup)
            return i;
    }
    return -1;
}


edict_t* CSaveRestoreBuffer::EntityFromIndex(int entityIndex)
{
    if (!m_pdata || entityIndex < 0)
        return NULL;

    int i;
    ENTITYTABLE* pTable;

    for (i = 0; i < m_pdata->tableCount; i++)
    {
        pTable = m_pdata->pTable + i;
        if (pTable->id == entityIndex)
            return pTable->pent;
    }
    return NULL;
}


int CSaveRestoreBuffer::EntityFlagsSet(int entityIndex, int flags)
{
    if (!m_pdata || entityIndex < 0)
        return 0;
    if (entityIndex > m_pdata->tableCount)
        return 0;

    m_pdata->pTable[entityIndex].flags |= flags;

    return m_pdata->pTable[entityIndex].flags;
}


void CSaveRestoreBuffer::BufferRewind(int size)
{
    if (!m_pdata)
        return;

    if (m_pdata->size < size)
        size = m_pdata->size;

    m_pdata->pCurrentData -= size;
    m_pdata->size -= size;
}

#ifndef _WIN32
extern "C" {
    unsigned _rotr(unsigned val, int shift)
    {
        register unsigned lobit;        /* non-zero means lo bit set */
        register unsigned num = val;    /* number to rotate */

        shift &= 0x1f;                  /* modulo 32 -- this will also make
                                           negative shifts work */

        while (shift--) {
            lobit = num & 1;        /* get high bit */
            num >>= 1;              /* shift right one bit */
            if (lobit)
                num |= 0x80000000;  /* set hi bit if lo bit was set */
        }

        return num;
    }
}
#endif

unsigned int CSaveRestoreBuffer::HashString(const char* pszToken)
{
    unsigned int hash = 0;

    while (*pszToken)
        hash = _rotr(hash, 4) ^ *pszToken++;

    return hash;
}

unsigned short CSaveRestoreBuffer::TokenHash(const char* pszToken)
{
    unsigned short hash = (unsigned short)(HashString(pszToken) % (unsigned)m_pdata->tokenCount);

#if _DEBUG
    static int tokensparsed = 0;
    tokensparsed++;
    if (!m_pdata->tokenCount || !m_pdata->pTokens)
        ALERT(at_error, "No token table array in TokenHash()!");
#endif

    for (int i = 0; i < m_pdata->tokenCount; i++)
    {
#if _DEBUG
        static qboolean beentheredonethat = FALSE;
        if (i > 50 && !beentheredonethat)
        {
            beentheredonethat = TRUE;
            ALERT(at_error, "CSaveRestoreBuffer :: TokenHash() is getting too full!");
        }
#endif

        int index = hash + i;
        if (index >= m_pdata->tokenCount)
            index -= m_pdata->tokenCount;

        if (!m_pdata->pTokens[index] || strcmp(pszToken, m_pdata->pTokens[index]) == 0)
        {
            m_pdata->pTokens[index] = (char*)pszToken;
            return index;
        }
    }

    // Token hash table full!!! 
    // [Consider doing overflow table(s) after the main table & limiting linear hash table search]
    ALERT(at_error, "CSaveRestoreBuffer :: TokenHash() is COMPLETELY FULL!");
    return 0;
}
