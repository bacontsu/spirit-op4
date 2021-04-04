/***
*
*    Copyright (c) 1996-2001, Valve LLC. All rights reserved.
*    
*    This product contains software technology licensed from Id 
*    Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*    All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#pragma once

#include "extdll.h"
#include "saverestore/CSave.h"
#include "saverestore/CRestore.h"
#include <vector>

extern int gSizes[FIELD_TYPECOUNT];
extern TYPEDESCRIPTION gEntvarsDescription[];
extern int gEntvarsCount;

#define ENTVARS_COUNT gEntvarsCount

#define IMPLEMENT_SAVERESTORE(derivedClass,baseClass) \
    int derivedClass::Save( CSave &save )\
    {\
        if ( !baseClass::Save(save) )\
            return 0;\
        if (pev->targetname)\
            return save.WriteFields( STRING(pev->targetname), #derivedClass, this, m_SaveData, ARRAYSIZE(m_SaveData) );\
        else\
            return save.WriteFields( STRING(pev->classname), #derivedClass, this, m_SaveData, ARRAYSIZE(m_SaveData) );\
    }\
    int derivedClass::Restore( CRestore &restore )\
    {\
        if ( !baseClass::Restore(restore) )\
            return 0;\
        return restore.ReadFields( #derivedClass, this, m_SaveData, ARRAYSIZE(m_SaveData) );\
    }

inline short GetVectorFieldSize(TYPEDESCRIPTION type, void* pOutputData)
{
    if (pOutputData == nullptr) return -1;
    switch (type.fieldType)
    {
        case FIELD_VECTOR:
        case FIELD_POSITION_VECTOR:
            return static_cast<std::vector<Vector>*>(pOutputData)->size();
        case FIELD_SHORT:
            return static_cast<std::vector<short>*>(pOutputData)->size();
        case FIELD_CHARACTER: 
            return static_cast<std::vector<char>*>(pOutputData)->size();
        default:
            return static_cast<std::vector<size_t>*>(pOutputData)->size();
    }
}

inline void* GetVectorData(TYPEDESCRIPTION type, void* pOutputData)
{
    if (pOutputData == nullptr) return nullptr;
    switch (type.fieldType)
    {
        case FIELD_VECTOR:
        case FIELD_POSITION_VECTOR:
            return static_cast<std::vector<Vector>*>(pOutputData)->data();
        case FIELD_SHORT:
            return static_cast<std::vector<short>*>(pOutputData)->data();
        case FIELD_CHARACTER: 
            return static_cast<std::vector<char>*>(pOutputData)->data();
        default:
            return static_cast<std::vector<size_t>*>(pOutputData)->data();
    }
}

inline void AssignVector(TYPEDESCRIPTION type, short count, void* source, void* destination)
{
    switch (type.fieldType)
    {
    case FIELD_VECTOR:
    case FIELD_POSITION_VECTOR:
        {
            auto* const cast = static_cast<Vector*>(source);
            const auto vec = std::vector<Vector>(cast, cast + count);
            *static_cast<std::vector<Vector>*>(destination) = vec;
            break;
        }
    case FIELD_SHORT:
        {
            auto* const cast = static_cast<short*>(source);
            const auto vec = std::vector<short>(cast, cast + count);
            *static_cast<std::vector<short>*>(destination) = vec;
            break;
        }
    case FIELD_CHARACTER:
        {
            auto* const cast = static_cast<char*>(source);
            const auto vec = std::vector<char>(cast, cast + count);
            *static_cast<std::vector<char>*>(destination) = vec;
            break;
        }
    default:
        {
            auto* const cast = static_cast<size_t*>(source);
            const auto vec = std::vector<size_t>(cast, cast + count);
            *static_cast<std::vector<size_t>*>(destination) = vec;
            break;
        }
    }
}
