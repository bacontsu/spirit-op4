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

#include "entities/CBaseToggle.h"
#include "util/locksounds.h"

#define SF_BUTTON_DONTMOVE           1

#define SF_ROTBUTTON_NOTSOLID        1
#define SF_BUTTON_ONLYDIRECT        16  // LRC - button can't be used through walls.
#define SF_BUTTON_TOGGLE            32  // button stays pushed until reactivated
#define SF_BUTTON_SPARK_IF_OFF      64  // button sparks in OFF state
#define SF_BUTTON_NOT_SOLID        128  // button isn't solid
#define SF_BUTTON_TOUCH_ONLY       256  // button must be touched to be used.
#define SF_BUTTON_USEKEY           512  // change the reaction of the button to the USE key.
                                        // (i.e. if it's meant to be ignored, don't ignore it; otherwise ignore it.)

const char* ButtonSound(int sound); // get string of button sound number

//
// Generic Button
//
class CBaseButton : public CBaseToggle
{
public:
    void Spawn() override;
    void PostSpawn() override; //LRC
    void Precache() override;
    void RotSpawn();
    void KeyValue(KeyValueData* pkvd) override;

    void ButtonActivate();
    void SparkSoundCache();

    void DLLEXPORT ButtonShot();
    void DLLEXPORT ButtonTouch(CBaseEntity* pOther);
    void DLLEXPORT ButtonSpark();
    void DLLEXPORT TriggerAndWait();
    void DLLEXPORT ButtonReturn();
    void DLLEXPORT ButtonBackHome();
    void DLLEXPORT ButtonUse_IgnorePlayer(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
    void DLLEXPORT ButtonUse(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value);
    int TakeDamage(entvars_t* pevInflictor, entvars_t* pevAttacker, float flDamage, int bitsDamageType) override;
    int Save(CSave& save) override;
    int Restore(CRestore& restore) override;

    enum BUTTON_CODE { BUTTON_NOTHING, BUTTON_ACTIVATE, BUTTON_RETURN };

    BUTTON_CODE ButtonResponseToTouch(void);

    static TYPEDESCRIPTION m_SaveData[];
    int ObjectCaps() override;

    BOOL m_fStayPushed; // button stays pushed in until touched again?
    BOOL m_fRotating; // a rotating button?  default is a sliding button.

    string_t m_strChangeTarget; // if this field is not null, this is an index into the engine string array.
    // when this button is touched, it's target entity's TARGET field will be set
    // to the button's ChangeTarget. This allows you to make a func_train switch paths, etc.

    locksound_t m_ls; // door lock sounds

    BYTE m_bLockedSound; // ordinals from entity selection
    BYTE m_bLockedSentence;
    BYTE m_bUnlockedSound;
    BYTE m_bUnlockedSentence;
    int m_sounds;
};
