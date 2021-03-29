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

#include "CFuncPlatRot.h"
#include "CFuncTrackTrain.h"

// ----------------------------------------------------------------------------
//
// Track changer / Train elevator
//
// ----------------------------------------------------------------------------

#define SF_TRACK_ACTIVATETRAIN        0x00000001
#define SF_TRACK_RELINK                0x00000002
#define SF_TRACK_ROTMOVE            0x00000004
#define SF_TRACK_STARTBOTTOM        0x00000008
#define SF_TRACK_DONT_MOVE            0x00000010

//
// This entity is a rotating/moving platform that will carry a train to a new track.
// It must be larger in X-Y planar area than the train, since it must contain the
// train within these dimensions in order to operate when the train is near it.
//

typedef enum { TRAIN_SAFE, TRAIN_BLOCKING, TRAIN_FOLLOWING } TRAIN_CODE;

class CFuncTrackChange : public CFuncPlatRot
{
public:
    void Spawn() override;
    void Precache() override;

    //    virtual void    Blocked();
    void DLLEXPORT GoUp() override;
    void DLLEXPORT GoDown() override;

    void KeyValue(KeyValueData* pkvd) override;
    void Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value) override;
    void DLLEXPORT Find();
    TRAIN_CODE EvaluateTrain(CPathTrack* pcurrent);
    void UpdateTrain(Vector& dest);
    void HitBottom() override;
    void HitTop() override;
    void Touch(CBaseEntity* pOther) override;
    virtual void UpdateAutoTargets(int toggleState);
    BOOL IsTogglePlat() override;

    void DisableUse() { m_use = 0; }
    void EnableUse() { m_use = 1; }
    int UseEnabled() { return m_use; }

    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    void OverrideReset() override;


    CPathTrack* m_trackTop;
    CPathTrack* m_trackBottom;

    CFuncTrackTrain* m_train;

    int m_trackTopName;
    int m_trackBottomName;
    int m_trainName;
    TRAIN_CODE m_code;
    int m_targetState;
    int m_use;
};
