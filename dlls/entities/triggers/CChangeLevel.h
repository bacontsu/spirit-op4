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

#include "CBaseTrigger.h"

extern DLL_GLOBAL BOOL g_fGameOver;

#define SF_CHANGELEVEL_USEONLY        0x0002

class CChangeLevel : public CBaseTrigger
{
public:
    void Spawn() override;
    void KeyValue(KeyValueData* pkvd) override;
    void DLLEXPORT UseChangeLevel(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
    void DLLEXPORT TriggerChangeLevel();
    void DLLEXPORT ExecuteChangeLevel();
    void DLLEXPORT TouchChangeLevel(CBaseEntity* pOther);
    void ChangeLevelNow(CBaseEntity* pActivator);

    static edict_t* FindLandmark(const char* pLandmarkName);
    static int ChangeList(LEVELLIST* pLevelList, int maxList);
    static int AddTransitionToList(LEVELLIST* pLevelList, int listCount, const char* pMapName, const char* pLandmarkName, edict_t* pentLandmark);
    static int InTransitionVolume(CBaseEntity* pEntity, char* pVolumeName);

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;

    static TYPEDESCRIPTION m_SaveData[];

    char m_szMapName[cchMapNameMost]; // trigger_changelevel only:  next map
    char m_szLandmarkName[cchMapNameMost]; // trigger_changelevel only:  landmark on next map
    int m_changeTarget;
    float m_changeTargetDelay;
};
