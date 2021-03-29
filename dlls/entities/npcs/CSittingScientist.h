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

#include "CScientist.h"

//=========================================================
// Sitting Scientist PROP
//=========================================================
class CSittingScientist : public CScientist // kdb: changed from public CBaseMonster so he can speak
{
public:
    void Spawn() override;
    void Precache() override;

    void DLLEXPORT SittingThink();
    int Classify() override;
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;
    static TYPEDESCRIPTION m_SaveData[];

    void SetAnswerQuestion(CTalkMonster* pSpeaker) override;
    int FriendNumber(int arrayNumber) override;

    int FIdleSpeak();
    int m_baseSequence;
    int m_headTurn;
    float m_flResponseDelay;
};
