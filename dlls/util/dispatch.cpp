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

#include "dispatch.h"
#include "extdll.h"
#include "findentity.h"
#include "util/movewith.h"
#include "entities/player/CBasePlayer.h"
#include "util/skill.h"
#include "util.h"
#include "gamerules/CGameRules.h"
#include "saverestore/CGlobalState.h"

int DispatchSpawn(edict_t* pent)
{
    CBaseEntity* pEntity = (CBaseEntity*)GET_PRIVATE(pent);

    if (pEntity)
    {
        // Initialize these or entities who don't link to the world won't have anything in here
        pEntity->pev->absmin = pEntity->pev->origin - Vector(1, 1, 1);
        pEntity->pev->absmax = pEntity->pev->origin + Vector(1, 1, 1);

        //        pEntity->InitMoveWith(); //LRC
        pEntity->Spawn();

        // Try to get the pointer again, in case the spawn function deleted the entity.
        // UNDONE: Spawn() should really return a code to ask that the entity be deleted, but
        // that would touch too much code for me to do that right now.
        pEntity = (CBaseEntity*)GET_PRIVATE(pent);

        if (pEntity)
        {
            if (g_pGameRules && !g_pGameRules->IsAllowedToSpawn(pEntity))
                return -1; // return that this entity should be deleted
            if (pEntity->pev->flags & FL_KILLME)
                return -1;
            if (g_iSkillLevel == SKILL_EASY && pEntity->m_iLFlags & LF_NOTEASY)
                return -1; //LRC
            if (g_iSkillLevel == SKILL_MEDIUM && pEntity->m_iLFlags & LF_NOTMEDIUM)
                return -1; //LRC
            if (g_iSkillLevel == SKILL_HARD && pEntity->m_iLFlags & LF_NOTHARD)
                return -1; //LRC
        }


        // Handle global stuff here
        if (pEntity && pEntity->pev->globalname)
        {
            const globalentity_t* pGlobal = gGlobalState.EntityFromTable(pEntity->pev->globalname);
            if (pGlobal)
            {
                // Already dead? delete
                if (pGlobal->state == GLOBAL_DEAD)
                    return -1;
                else if (!FStrEq(STRING(gpGlobals->mapname), pGlobal->levelName))
                    pEntity->MakeDormant(); // Hasn't been moved to this level yet, wait but stay alive
                // In this level & not dead, continue on as normal
            }
            else
            {
                // Spawned entities default to 'On'
                gGlobalState.EntityAdd(pEntity->pev->globalname, gpGlobals->mapname, GLOBAL_ON);
                //                ALERT( at_console, "Added global entity %s (%s)\n", STRING(pEntity->pev->classname), STRING(pEntity->pev->globalname) );
            }
        }
    }

    return 0;
}

void DispatchKeyValue(edict_t* pentKeyvalue, KeyValueData* pkvd)
{
    if (!pkvd || !pentKeyvalue)
        return;

    EntvarsKeyvalue(VARS(pentKeyvalue), pkvd);

    // If the key was an entity variable, or there's no class set yet, don't look for the object, it may
    // not exist yet.
    if (pkvd->fHandled || pkvd->szClassName == NULL)
        return;

    // Get the actualy entity object
    CBaseEntity* pEntity = (CBaseEntity*)GET_PRIVATE(pentKeyvalue);

    if (!pEntity)
        return;

    pEntity->KeyValue(pkvd);
}

// HACKHACK -- this is a hack to keep the node graph entity from "touching" things (like triggers)
// while it builds the graph
BOOL gTouchDisabled = FALSE;

void DispatchTouch(edict_t* pentTouched, edict_t* pentOther)
{
    if (gTouchDisabled)
        return;

    CBaseEntity* pEntity = (CBaseEntity*)GET_PRIVATE(pentTouched);
    CBaseEntity* pOther = (CBaseEntity*)GET_PRIVATE(pentOther);

    if (pEntity && pOther && !((pEntity->pev->flags | pOther->pev->flags) & FL_KILLME))
        pEntity->Touch(pOther);
}

void DispatchUse(edict_t* pentUsed, edict_t* pentOther)
{
    CBaseEntity* pEntity = (CBaseEntity*)GET_PRIVATE(pentUsed);
    CBaseEntity* pOther = (CBaseEntity*)GET_PRIVATE(pentOther);

    if (pEntity && !(pEntity->pev->flags & FL_KILLME))
        pEntity->Use(pOther, pOther, USE_TOGGLE, 0);
}

void DispatchThink(edict_t* pent)
{
    CBaseEntity* pEntity = (CBaseEntity*)GET_PRIVATE(pent);

    if (pEntity)
    {
        if (FBitSet(pEntity->pev->flags, FL_DORMANT))
            ALERT(at_error, "Dormant entity %s is thinking!!\n", STRING(pEntity->pev->classname));

        //if (pEntity->pev->classname) ALERT(at_console, "DispatchThink %s\n", STRING(pEntity->pev->targetname));
        pEntity->Think();
    }
}

void DispatchBlocked(edict_t* pentBlocked, edict_t* pentOther)
{
    CBaseEntity* pEntity = (CBaseEntity*)GET_PRIVATE(pentBlocked);
    CBaseEntity* pOther = (CBaseEntity*)GET_PRIVATE(pentOther);

    if (pEntity)
        pEntity->Blocked(pOther);
}

void DispatchSave(edict_t* pent, SAVERESTOREDATA* pSaveData)
{
	gpGlobals->time = pSaveData->time;

    CBaseEntity* pEntity = (CBaseEntity*)GET_PRIVATE(pent);

    if (pEntity && pSaveData)
    {
        ENTITYTABLE* pTable = &pSaveData->pTable[pSaveData->currentIndex];

        if (pTable->pent != pent)
            ALERT(at_error, "ENTITY TABLE OR INDEX IS WRONG!!!!\n");

        if (pEntity->ObjectCaps() & FCAP_DONT_SAVE)
            return;

        // These don't use ltime & nextthink as times really, but we'll fudge around it.
        if (pEntity->pev->movetype == MOVETYPE_PUSH)
        {
            //LRC - rearranged so that we can correct m_fNextThink too.
            float delta = gpGlobals->time - pEntity->pev->ltime;
            pEntity->pev->ltime += delta;
            pEntity->pev->nextthink += delta;
            pEntity->m_fPevNextThink = pEntity->pev->nextthink;
            pEntity->m_fNextThink += delta;
        }

        pTable->location = pSaveData->size; // Remember entity position for file I/O
        pTable->classname = pEntity->pev->classname; // Remember entity class for respawn

        CSave saveHelper(pSaveData);
        pEntity->OnBeforeSave(saveHelper);
        pEntity->Save(saveHelper);
        pEntity->OnAfterSave(saveHelper);

        pTable->size = pSaveData->size - pTable->location; // Size of entity block is data size written to block
    }
}

void OnFreeEntPrivateData(edict_s* pEdict)
{
	if (pEdict && pEdict->pvPrivateData)
	{
		((CBaseEntity*)pEdict->pvPrivateData)->~CBaseEntity();
	}
}

// Find the matching global entity.  Spit out an error if the designer made entities of
// different classes with the same global name
CBaseEntity* FindGlobalEntity(string_t classname, string_t globalname)
{
    CBaseEntity* pReturn = UTIL_FindEntityByString(NULL, "globalname", STRING(globalname));
    if (pReturn)
    {
        if (!FClassnameIs(pReturn->pev, STRING(classname)))
        {
            ALERT(at_debug, "Global entity found %s, wrong class %s\n", STRING(globalname), STRING(pReturn->pev->classname));
            pReturn = NULL;
        }
    }

    return pReturn;
}

int DispatchRestore(edict_t* pent, SAVERESTOREDATA* pSaveData, int globalEntity)
{
	gpGlobals->time = pSaveData->time;

    CBaseEntity* pEntity = (CBaseEntity*)GET_PRIVATE(pent);

    if (pEntity && pSaveData)
    {
        entvars_t tmpVars;
        Vector oldOffset;

        CRestore restoreHelper(pSaveData);
        if (globalEntity)
        {
            CRestore tmpRestore(pSaveData);
            tmpRestore.PrecacheMode(0);
            tmpRestore.ReadEntVars("ENTVARS", &tmpVars);

            // HACKHACK - reset the save pointers, we're going to restore for real this time
            pSaveData->size = pSaveData->pTable[pSaveData->currentIndex].location;
            pSaveData->pCurrentData = pSaveData->pBaseData + pSaveData->size;
            // -------------------


            const globalentity_t* pGlobal = gGlobalState.EntityFromTable(tmpVars.globalname);

            // Don't overlay any instance of the global that isn't the latest
            // pSaveData->szCurrentMapName is the level this entity is coming from
            // pGlobla->levelName is the last level the global entity was active in.
            // If they aren't the same, then this global update is out of date.
            if (!FStrEq(pSaveData->szCurrentMapName, pGlobal->levelName))
                return 0;

            // Compute the new global offset
            oldOffset = pSaveData->vecLandmarkOffset;
            CBaseEntity* pNewEntity = FindGlobalEntity(tmpVars.classname, tmpVars.globalname);
            if (pNewEntity)
            {
                //                ALERT( at_console, "Overlay %s with %s\n", STRING(pNewEntity->pev->classname), STRING(tmpVars.classname) );
                // Tell the restore code we're overlaying a global entity from another level
                restoreHelper.SetGlobalMode(1); // Don't overwrite global fields
                pSaveData->vecLandmarkOffset = (pSaveData->vecLandmarkOffset - pNewEntity->pev->mins) + tmpVars.mins;
                pEntity = pNewEntity; // we're going to restore this data OVER the old entity
                pent = ENT(pEntity->pev);
                // Update the global table to say that the global definition of this entity should come from this level
                gGlobalState.EntityUpdate(pEntity->pev->globalname, gpGlobals->mapname);
            }
            else
            {
                // This entity will be freed automatically by the engine.  If we don't do a restore on a matching entity (below)
                // or call EntityUpdate() to move it to this level, we haven't changed global state at all.
                return 0;
            }
        }
        
        pEntity->OnBeforeRestore(restoreHelper);
        if (pEntity->ObjectCaps() & FCAP_MUST_SPAWN)
        {
            pEntity->Restore(restoreHelper);
            pEntity->Spawn();
        }
        else
        {
            pEntity->Restore(restoreHelper);
            pEntity->Precache();
        }

        // Again, could be deleted, get the pointer again.
        pEntity = (CBaseEntity*)GET_PRIVATE(pent);

#if 0
        if (pEntity && pEntity->pev->globalname && globalEntity)
        {
            ALERT(at_debug, "Global %s is %s\n", STRING(pEntity->pev->globalname), STRING(pEntity->pev->model));
        }
#endif

        // Is this an overriding global entity (coming over the transition), or one restoring in a level
        if (globalEntity)
        {
            //            ALERT( at_console, "After: %f %f %f %s\n", pEntity->pev->origin.x, pEntity->pev->origin.y, pEntity->pev->origin.z, STRING(pEntity->pev->model) );
            pSaveData->vecLandmarkOffset = oldOffset;
            if (pEntity)
            {
                UTIL_SetOrigin(pEntity, pEntity->pev->origin);
                pEntity->OverrideReset();
            }
        }
        else if (pEntity && pEntity->pev->globalname)
        {
            const globalentity_t* pGlobal = gGlobalState.EntityFromTable(pEntity->pev->globalname);
            if (pGlobal)
            {
                // Already dead? delete
                if (pGlobal->state == GLOBAL_DEAD)
                    return -1;
                else if (!FStrEq(STRING(gpGlobals->mapname), pGlobal->levelName))
                {
                    pEntity->MakeDormant(); // Hasn't been moved to this level yet, wait but stay alive
                }
                // In this level & not dead, continue on as normal
            }
            else
            {
                ALERT(at_error, "Global Entity %s (%s) not in table!!!\n", STRING(pEntity->pev->globalname), STRING(pEntity->pev->classname));
                // Spawned entities default to 'On'
                gGlobalState.EntityAdd(pEntity->pev->globalname, gpGlobals->mapname, GLOBAL_ON);
            }
        }
        
        pEntity->OnAfterRestore(restoreHelper);
    }
    return 0;
}

// Initialize absmin & absmax to the appropriate box
void SetObjectCollisionBox(entvars_t* pev)
{
    if ((pev->solid == SOLID_BSP) &&
        (pev->angles.x || pev->angles.y || pev->angles.z))
    {
        // expand for rotation
        float max, v;
        int i;

        max = 0;
        for (i = 0; i < 3; i++)
        {
            v = fabs(((float*)pev->mins)[i]);
            if (v > max)
                max = v;
            v = fabs(((float*)pev->maxs)[i]);
            if (v > max)
                max = v;
        }
        for (i = 0; i < 3; i++)
        {
            ((float*)pev->absmin)[i] = ((float*)pev->origin)[i] - max;
            ((float*)pev->absmax)[i] = ((float*)pev->origin)[i] + max;
        }
    }
    else
    {
        pev->absmin = pev->origin + pev->mins;
        pev->absmax = pev->origin + pev->maxs;
    }

    pev->absmin.x -= 1;
    pev->absmin.y -= 1;
    pev->absmin.z -= 1;
    pev->absmax.x += 1;
    pev->absmax.y += 1;
    pev->absmax.z += 1;
}

void DispatchObjectCollsionBox(edict_t* pent)
{
    CBaseEntity* pEntity = (CBaseEntity*)GET_PRIVATE(pent);
    if (pEntity)
    {
        pEntity->SetObjectCollisionBox();
    }
    else
        SetObjectCollisionBox(&pent->v);
}

void SaveWriteFields(SAVERESTOREDATA* pSaveData, const char* pname, void* pBaseData, TYPEDESCRIPTION* pFields, int fieldCount)
{
    CSave saveHelper(pSaveData);
    saveHelper.WriteFields("SWF", pname, pBaseData, pFields, fieldCount);
}

void SaveReadFields(SAVERESTOREDATA* pSaveData, const char* pname, void* pBaseData, TYPEDESCRIPTION* pFields, int fieldCount)
{
    CRestore restoreHelper(pSaveData);
    restoreHelper.ReadFields(pname, pBaseData, pFields, fieldCount);
}

void SaveGlobalState(SAVERESTOREDATA* pSaveData)
{
    CSave saveHelper(pSaveData);
    gGlobalState.Save(saveHelper);
}

void RestoreGlobalState(SAVERESTOREDATA* pSaveData)
{
    CRestore restoreHelper(pSaveData);
    gGlobalState.Restore(restoreHelper);
}

void ResetGlobalState()
{
    gGlobalState.ClearStates();
    gInitHUD = TRUE; // Init the HUD on a new game / load game
}
