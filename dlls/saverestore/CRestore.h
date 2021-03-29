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

#include "CSaveRestoreBuffer.h"

typedef struct
{
    unsigned short size;
    unsigned short token;
    char* pData;
} HEADER;

class CRestore : public CSaveRestoreBuffer
{
public:
    CRestore(SAVERESTOREDATA* pdata) : CSaveRestoreBuffer(pdata)
    {
        m_global = 0;
        m_precache = TRUE;
    }

    int ReadEntVars(const char* pname, entvars_t* pev); // entvars_t
    int ReadFields(const char* pname, void* pBaseData, TYPEDESCRIPTION* pFields, int fieldCount);
    int ReadField(void* pBaseData, TYPEDESCRIPTION* pFields, int fieldCount, int startField, int size, char* pName, void* pData);
    int ReadInt();
    short ReadShort();
    int ReadNamedInt(const char* pName);
    char* ReadNamedString(const char* pName);
    int Empty() { return (m_pdata == NULL) || ((m_pdata->pCurrentData - m_pdata->pBaseData) >= m_pdata->bufferSize); }
    inline void SetGlobalMode(int global) { m_global = global; }
    void PrecacheMode(BOOL mode) { m_precache = mode; }

private:
    char* BufferPointer();
    void BufferReadBytes(char* pOutput, int size);
    void BufferSkipBytes(int bytes);
    int BufferSkipZString();
    int BufferCheckZString(const char* string);

    void BufferReadHeader(HEADER* pheader);

    int m_global; // Restoring a global entity?
    BOOL m_precache;
};
