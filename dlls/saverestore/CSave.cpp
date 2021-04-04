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

#include "CSave.h"
#include "util/ehandle.h"
#include "util/saverestore.h"


void CSave::WriteData(const char* pname, int size, const char* pdata, int prefixSize, const char* prefixData)
{
    BufferField(pname, size, pdata, prefixSize, prefixData);
}


void CSave::WriteShort(const char* pname, const short* data, int count, int prefixSize, const char* prefixData)
{
    BufferField(pname, sizeof(short) * count, (const char*)data, prefixSize, prefixData);
}


void CSave::WriteInt(const char* pname, const int* data, int count, int prefixSize, const char* prefixData)
{
    BufferField(pname, sizeof(int) * count, (const char*)data, prefixSize, prefixData);
}


void CSave::WriteFloat(const char* pname, const float* data, int count, int prefixSize, const char* prefixData)
{
    BufferField(pname, sizeof(float) * count, (const char*)data, prefixSize, prefixData);
}


void CSave::WriteTime(const char* pname, const float* data, int count, int prefixSize, const char* prefixData)
{
    Vector tmp, input;

    BufferHeader(pname, sizeof(float) * count + prefixSize);
    if (prefixSize > 0) BufferData(prefixData, prefixSize);

    for (auto i = 0; i < count; i++)
    {
        auto tmp = data[0];

        // Always encode time as a delta from the current time so it can be re-based if loaded in a new level
        // Times of 0 are never written to the file, so they will be restored as 0, not a relative time
        if (m_pdata)
            tmp -= m_pdata->time;

        BufferData((const char*)&tmp, sizeof(float));
        data++;
    }
}


void CSave::WriteString(const char* pname, const char* pdata, int prefixSize, const char* prefixData)
{
#ifdef TOKENIZE
    short    token = (short)TokenHash(pdata);
    WriteShort(pname, &token, 1);
#else
    BufferField(pname, strlen(pdata) + 1, pdata, prefixSize, prefixData);
#endif
}


void CSave::WriteString(const char* pname, const int* stringId, int count, int prefixSize, const char* prefixData)
{
    int i, size;

#ifdef TOKENIZE
    short    token = (short)TokenHash(STRING(*stringId));
    WriteShort(pname, &token, 1);
#else
#if 0
    if (count != 1)
        ALERT(at_error, "No string arrays!\n");
    WriteString(pname, (char*)STRING(*stringId));
#endif

    size = 0;
    for (i = 0; i < count; i++)
        size += strlen(STRING(stringId[i])) + 1;

    BufferHeader(pname, size + prefixSize);
    if (prefixSize > 0) BufferData(prefixData, prefixSize);
    for (i = 0; i < count; i++)
    {
        const auto* pString = STRING(stringId[i]);
        BufferData(pString, strlen(pString) + 1);
    }
#endif
}


void CSave::WriteVector(const char* pname, const Vector& value, int prefixSize, const char* prefixData)
{
    WriteVector(pname, &value.x, 1, prefixSize, prefixData);
}


void CSave::WriteVector(const char* pname, const float* value, int count, int prefixSize, const char* prefixData)
{
    BufferHeader(pname, sizeof(float) * 3 * count + prefixSize);
    if (prefixSize > 0) BufferData(prefixData, prefixSize);
    BufferData((const char*)value, sizeof(float) * 3 * count);
}


void CSave::WritePositionVector(const char* pname, const Vector& value, int prefixSize, const char* prefixData)
{
    if (m_pdata && m_pdata->fUseLandmark)
    {
        const auto tmp = value - m_pdata->vecLandmarkOffset;
        WriteVector(pname, tmp, prefixSize, prefixData);
    }

    WriteVector(pname, value, prefixSize, prefixData);
}


void CSave::WritePositionVector(const char* pname, const float* value, int count, int prefixSize, const char* prefixData)
{
    Vector tmp, input;

    BufferHeader(pname, sizeof(float) * 3 * count + prefixSize);
    if (prefixSize > 0) BufferData(prefixData, prefixSize);
    for (auto i = 0; i < count; i++)
    {
        Vector tmp(value[0], value[1], value[2]);

        if (m_pdata && m_pdata->fUseLandmark)
            tmp = tmp - m_pdata->vecLandmarkOffset;

        BufferData((const char*)&tmp.x, sizeof(float) * 3);
        value += 3;
    }
}


void CSave::WriteFunction(const char* cname, const char* pname, void** data, int count, int prefixSize, const char* prefixData)
{
    const auto* functionName = NAME_FOR_FUNCTION((uint32)*data);
    if (functionName)
        BufferField(pname, strlen(functionName) + 1, functionName, prefixSize, prefixData);
    else
        ALERT(at_error, "Member \"%s\" of \"%s\" contains an invalid function pointer %p!", pname, cname, *data);
}


void EntvarsKeyvalue(entvars_t* pev, KeyValueData* pkvd)
{
    for (auto i = 0; i < ENTVARS_COUNT; i++)
    {
        auto* pField = &gEntvarsDescription[i];

        if (!stricmp(pField->fieldName, pkvd->szKeyName))
        {
            switch (pField->fieldType)
            {
            case FIELD_MODELNAME:
            case FIELD_SOUNDNAME:
            case FIELD_STRING:
                (*(int*)((char*)pev + pField->fieldOffset)) = ALLOC_STRING(pkvd->szValue);
                break;

            case FIELD_TIME:
            case FIELD_FLOAT:
                (*(float*)((char*)pev + pField->fieldOffset)) = atof(pkvd->szValue);
                break;

            case FIELD_INTEGER:
                (*(int*)((char*)pev + pField->fieldOffset)) = atoi(pkvd->szValue);
                break;

            case FIELD_POSITION_VECTOR:
            case FIELD_VECTOR:
                UTIL_StringToVector((float*)((char*)pev + pField->fieldOffset), pkvd->szValue);
                break;

            default:
            case FIELD_EVARS:
            case FIELD_CLASSPTR:
            case FIELD_EDICT:
            case FIELD_ENTITY:
            case FIELD_POINTER:
                ALERT(at_error, "Bad field in entity!!\n");
                break;
            }
            pkvd->fHandled = TRUE;
            return;
        }
    }
}


int CSave::WriteEntVars(const char* pname, entvars_t* pev)
{
    if (pev->targetname)
        return WriteFields(STRING(pev->targetname), pname, pev, gEntvarsDescription, ENTVARS_COUNT);
    else
        return WriteFields(STRING(pev->classname), pname, pev, gEntvarsDescription, ENTVARS_COUNT);
}


int CSave::WriteFields(const char* cname, const char* pname, void* pBaseData, TYPEDESCRIPTION* pFields, int fieldCount)
{
    int i, j, actualCount;
    int entityArray[MAX_ENTITYARRAY];

    // Precalculate the number of empty fields
    auto emptyCount = 0;
    for (i = 0; i < fieldCount; i++)
    {
        if (pFields[i].fieldSize == ARRAYSIZE_STD_VECTOR) continue;
        void* pOutputData = (static_cast<char*>(pBaseData) + pFields[i].fieldOffset);
        if (DataEmpty(static_cast<const char*>(pOutputData), pFields[i].fieldSize * gSizes[pFields[i].fieldType]))
            emptyCount++;
    }

    // Empty fields will not be written, write out the actual number of fields to be written
    actualCount = fieldCount - emptyCount;
    WriteInt(pname, &actualCount, 1, 0, nullptr);

    for (i = 0; i < fieldCount; i++)
    {
        auto* pTest = &pFields[i];
        void* pOutputData = (static_cast<char*>(pBaseData) + pTest->fieldOffset);

        auto fieldSize = pTest->fieldSize;
        const auto isVector = fieldSize == ARRAYSIZE_STD_VECTOR;

        auto prefixSize = 0;
        const char* prefixData = nullptr;

        if (isVector)
        {
            fieldSize = GetVectorFieldSize(pFields[i], pOutputData);
            if (fieldSize < 0)
            {
                // null vector - don't write
                *static_cast<size_t*>(pOutputData) = NULL;
                continue;
            }

            // Assemble the prefix data
            auto prefixValue = fieldSize;
            prefixSize = sizeof(short);
            prefixData = static_cast<char*>(static_cast<void*>(&prefixValue));

            // Use the array data from the vector
            pOutputData = GetVectorData(pFields[i], pOutputData);
        }

        // UNDONE: Must we do this twice?
        if (!isVector && DataEmpty(static_cast<const char*>(pOutputData), fieldSize * gSizes[pTest->fieldType]))
            continue;

        switch (pTest->fieldType)
        {
        case FIELD_FLOAT:
            WriteFloat(pTest->fieldName, static_cast<float*>(pOutputData), fieldSize, prefixSize, prefixData);
            break;
        case FIELD_TIME:
            WriteTime(pTest->fieldName, static_cast<float*>(pOutputData), fieldSize, prefixSize, prefixData);
            break;
        case FIELD_MODELNAME:
        case FIELD_SOUNDNAME:
        case FIELD_STRING:
            WriteString(pTest->fieldName, static_cast<int*>(pOutputData), fieldSize, prefixSize, prefixData);
            break;
        case FIELD_CLASSPTR:
        case FIELD_EVARS:
        case FIELD_EDICT:
        case FIELD_ENTITY:
        case FIELD_EHANDLE:
            if (fieldSize > MAX_ENTITYARRAY)
                ALERT(at_error, "Can't save more than %d entities in an array!!!\n", MAX_ENTITYARRAY);
            for (j = 0; j < fieldSize; j++)
            {
                switch (pTest->fieldType)
                {
                case FIELD_EVARS:
                    entityArray[j] = EntityIndex(static_cast<entvars_t**>(pOutputData)[j]);
                    break;
                case FIELD_CLASSPTR:
                    entityArray[j] = EntityIndex(static_cast<CBaseEntity**>(pOutputData)[j]);
                    break;
                case FIELD_EDICT:
                    entityArray[j] = EntityIndex(static_cast<edict_t**>(pOutputData)[j]);
                    break;
                case FIELD_ENTITY:
                    entityArray[j] = EntityIndex(static_cast<EOFFSET*>(pOutputData)[j]);
                    break;
                case FIELD_EHANDLE:
                    entityArray[j] = EntityIndex(static_cast<CBaseEntity*>(static_cast<EHANDLE*>(pOutputData)[j]));
                    break;
                }
            }
            WriteInt(pTest->fieldName, entityArray, fieldSize, prefixSize, prefixData);
            break;
        case FIELD_POSITION_VECTOR:
            WritePositionVector(pTest->fieldName, static_cast<float*>(pOutputData), fieldSize, prefixSize, prefixData);
            break;
        case FIELD_VECTOR:
            WriteVector(pTest->fieldName, static_cast<float*>(pOutputData), fieldSize, prefixSize, prefixData);
            break;

        case FIELD_BOOLEAN:
        case FIELD_INTEGER:
            WriteInt(pTest->fieldName, static_cast<int*>(pOutputData), fieldSize, prefixSize, prefixData);
            break;

        case FIELD_SHORT:
            WriteData(pTest->fieldName, 2 * fieldSize, static_cast<char*>(pOutputData), prefixSize, prefixData);
            break;

        case FIELD_CHARACTER:
            WriteData(pTest->fieldName, fieldSize, static_cast<char*>(pOutputData), prefixSize, prefixData);
            break;

            // For now, just write the address out, we're not going to change memory while doing this yet!
        case FIELD_POINTER:
            WriteInt(pTest->fieldName, (int*)static_cast<char*>(pOutputData), fieldSize, prefixSize, prefixData);
            break;

        case FIELD_FUNCTION:
            WriteFunction(cname, pTest->fieldName, static_cast<void**>(pOutputData), fieldSize, prefixSize, prefixData);
            break;
        default:
            ALERT(at_error, "Bad field type\n");
        }
    }

    return 1;
}


void CSave::BufferString(char* pdata, int len)
{
    char c = 0;

    BufferData(pdata, len); // Write the string
    BufferData(&c, 1); // Write a null terminator
}


int CSave::DataEmpty(const char* pdata, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (pdata[i])
            return 0;
    }
    return 1;
}


void CSave::BufferField(const char* pname, const int size, const char* pdata, const int prefixSize, const char* prefixData)
{
    BufferHeader(pname, size + prefixSize);
    if (prefixSize > 0) BufferData(prefixData, prefixSize);
    BufferData(pdata, size);
}


void CSave::BufferHeader(const char* pname, int size)
{
    short hashvalue = TokenHash(pname);
    if (size > 1 << (sizeof(short) * 8))
        ALERT(at_error, "CSave :: BufferHeader() size parameter exceeds 'short'!");
    BufferData((const char*)&size, sizeof(short));
    BufferData((const char*)&hashvalue, sizeof(short));
}


void CSave::BufferData(const char* pdata, int size)
{
    if (!m_pdata)
        return;

    if (m_pdata->size + size > m_pdata->bufferSize)
    {
        ALERT(at_error, "Save/Restore overflow!");
        m_pdata->size = m_pdata->bufferSize;
        return;
    }

    memcpy(m_pdata->pCurrentData, pdata, size);
    m_pdata->pCurrentData += size;
    m_pdata->size += size;
}
