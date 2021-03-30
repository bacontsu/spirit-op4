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
#include "archtypes.h"     // DAL

//
// Misc utility code
//
#include "util/activity.h"
#include "util/entity.h"

#ifndef ENGINECALLBACK_H
#include "enginecallback.h"
#endif

inline void MESSAGE_BEGIN( int msg_dest, int msg_type, const float *pOrigin, entvars_t *ent );  // implementation later in this file

extern globalvars_t                *gpGlobals;

// Use this instead of ALLOC_STRING on constant strings
#define STRING(offset)        ((const char *)(gpGlobals->pStringBase + (unsigned int)(offset)))
#define MAKE_STRING(str)    ((uint64)(str) - (uint64)(STRING(0)))

// Keeps clutter down a bit, when using a float as a bit-vector
#define SetBits(flBitVector, bits)        ((flBitVector) = (int)(flBitVector) | (bits))
#define ClearBits(flBitVector, bits)    ((flBitVector) = (int)(flBitVector) & ~(bits))
#define FBitSet(flBitVector, bit)        ((int)(flBitVector) & (bit))

// Makes these more explicit, and easier to find
#define FILE_GLOBAL static
#define DLL_GLOBAL

// Until we figure out why "const" gives the compiler problems, we'll just have to use
// this bogus "empty" define to mark things as constant.
#define CONSTANT

// In case it's not alread defined
typedef int BOOL;

inline void MESSAGE_BEGIN( int msg_dest, int msg_type, const float *pOrigin, entvars_t *ent ) {
    (*g_engfuncs.pfnMessageBegin)(msg_dest, msg_type, pOrigin, ENT(ent));
}

// Testing strings for nullity
#define iStringNull 0
inline BOOL FStringNull(int iString)            { return iString == iStringNull; }

#define cchMapNameMost 32

// Dot products for view cone checking
#define VIEW_FIELD_FULL        (float)-1.0 // +-180 degrees
#define    VIEW_FIELD_WIDE        (float)-0.7 // +-135 degrees 0.1 // +-85 degrees, used for full FOV checks 
#define    VIEW_FIELD_NARROW    (float)0.7 // +-45 degrees, more narrow check used to set up ranged attacks
#define    VIEW_FIELD_ULTRA_NARROW    (float)0.9 // +-25 degrees, more narrow check used to set up ranged attacks

// All monsters need this data
#define        DONT_BLEED            -1
#define        BLOOD_COLOR_RED        (BYTE)247
#define        BLOOD_COLOR_YELLOW    (BYTE)195
#define        BLOOD_COLOR_GREEN    BLOOD_COLOR_YELLOW



// Misc useful
inline BOOL FStrEq(const char*sz1, const char*sz2)
    { return (strcmp(sz1, sz2) == 0); }
inline BOOL FClassnameIs(edict_t* pent, const char* szClassname)
    { return FStrEq(STRING(VARS(pent)->classname), szClassname); }
inline BOOL FClassnameIs(entvars_t* pev, const char* szClassname)
    { return FStrEq(STRING(pev->classname), szClassname); }

class CBaseEntity;

// Misc. Prototypes
extern void            UTIL_SetSize            (entvars_t* pev, const Vector &vecMin, const Vector &vecMax);
extern float        UTIL_VecToYaw            (const Vector &vec);
extern Vector        UTIL_VecToAngles        (const Vector &vec);
extern float        UTIL_AngleMod            (float a);
extern float        UTIL_AngleDiff            ( float destAngle, float srcAngle );

extern Vector        UTIL_AxisRotationToAngles    (const Vector &vec, float angle); //LRC
extern Vector        UTIL_AxisRotationToVec    (const Vector &vec, float angle); //LRC

//LRC 1.8 - renamed CBaseAlias
class CBaseMutableAlias;
extern void            UTIL_AddToAliasList( CBaseMutableAlias *pAlias );
extern void            UTIL_FlushAliases();


// returns a CBaseEntity pointer to a player by index.  Only returns if the player is spawned and connected
// otherwise returns NULL
// Index is 1 based
extern CBaseEntity    *UTIL_PlayerByIndex( int playerIndex );

#define UTIL_EntitiesInPVS(pent)            (*g_engfuncs.pfnEntitiesInPVS)(pent)
extern void            UTIL_MakeVectors        (const Vector &vecAngles);

// Pass in an array of pointers and an array size, it fills the array and returns the number inserted
extern int            UTIL_MonstersInSphere( CBaseEntity **pList, int listMax, const Vector &center, float radius );
extern int            UTIL_EntitiesInBox( CBaseEntity **pList, int listMax, const Vector &mins, const Vector &maxs, int flagMask );

inline void UTIL_MakeVectorsPrivate( const Vector &vecAngles, float *p_vForward, float *p_vRight, float *p_vUp )
{
    g_engfuncs.pfnAngleVectors( vecAngles, p_vForward, p_vRight, p_vUp );
}

extern void            UTIL_MakeAimVectors        ( const Vector &vecAngles ); // like MakeVectors, but assumes pitch isn't inverted
extern void            UTIL_MakeInvVectors        ( const Vector &vec, globalvars_t *pgv );

extern void            UTIL_SetEdictOrigin            ( edict_t *pEdict, const Vector &vecOrigin );
extern void            UTIL_SetOrigin            ( CBaseEntity* pEntity, const Vector &vecOrigin );

extern void            UTIL_EmitAmbientSound    ( edict_t *entity, const Vector &vecOrigin, const char *samp, float vol, float attenuation, int fFlags, int pitch );
extern void            UTIL_ParticleEffect        ( const Vector &vecOrigin, const Vector &vecDirection, ULONG ulColor, ULONG ulCount );
extern void            UTIL_ScreenShake        ( const Vector &center, float amplitude, float frequency, float duration, float radius );
extern void            UTIL_ScreenShakeAll        ( const Vector &center, float amplitude, float frequency, float duration );
extern void            UTIL_ShowMessage        ( const char *pString, CBaseEntity *pPlayer );
extern void            UTIL_ShowMessageAll        ( const char *pString );
extern void            UTIL_ScreenFadeAll        ( const Vector &color, float fadeTime, float holdTime, int alpha, int flags );
extern void            UTIL_ScreenFade            ( CBaseEntity *pEntity, const Vector &color, float fadeTime, float fadeHold, int alpha, int flags );

typedef enum { ignore_monsters=1, dont_ignore_monsters=0, missile=2 } IGNORE_MONSTERS;
typedef enum { ignore_glass=1, dont_ignore_glass=0 } IGNORE_GLASS;
extern void            UTIL_TraceLine            (const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, edict_t *pentIgnore, TraceResult *ptr);
extern void            UTIL_TraceLine            (const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t *pentIgnore, TraceResult *ptr);
enum { point_hull=0, human_hull=1, large_hull=2, head_hull=3 };
extern void            UTIL_TraceHull            (const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, int hullNumber, edict_t *pentIgnore, TraceResult *ptr);
extern TraceResult    UTIL_GetGlobalTrace        ();
extern void            UTIL_TraceModel            (const Vector &vecStart, const Vector &vecEnd, int hullNumber, edict_t *pentModel, TraceResult *ptr);
extern Vector        UTIL_GetAimVector        (edict_t* pent, float flSpeed);
extern int            UTIL_PointContents        (const Vector &vec);

extern int            UTIL_IsMasterTriggered    (string_t sMaster, CBaseEntity *pActivator);
extern void            UTIL_BloodStream( const Vector &origin, const Vector &direction, int color, int amount );
extern void            UTIL_BloodDrips( const Vector &origin, const Vector &direction, int color, int amount );
extern Vector        UTIL_RandomBloodVector();
extern BOOL            UTIL_ShouldShowBlood( int bloodColor );
extern void            UTIL_BloodDecalTrace( TraceResult *pTrace, int bloodColor );
extern void            UTIL_DecalTrace( TraceResult *pTrace, int decalNumber );
extern void            UTIL_PlayerDecalTrace( TraceResult *pTrace, int playernum, int decalNumber, BOOL bIsCustom );
extern void            UTIL_GunshotDecalTrace( TraceResult *pTrace, int decalNumber );
extern void            UTIL_Sparks( const Vector &position );
extern void            UTIL_Ricochet( const Vector &position, float scale );
extern void            UTIL_StringToVector( float *pVector, const char *pString );
extern void            UTIL_StringToRandomVector( float *pVector, const char *pString ); //LRC
extern void            UTIL_StringToIntArray( int *pVector, int count, const char *pString );
extern Vector        UTIL_ClampVectorToBox( const Vector &input, const Vector &clampSize );
extern float        UTIL_Approach( float target, float value, float speed );
extern float        UTIL_ApproachAngle( float target, float value, float speed );
extern float        UTIL_AngleDistance( float next, float cur );
inline float        UTIL_Lerp( float lerpfactor, float A, float B ) { return A + lerpfactor*(B-A); } //LRC 1.8 - long-missing convenience!

extern char            *UTIL_VarArgs( const char *format, ... );
extern void            UTIL_Remove( CBaseEntity *pEntity );
extern BOOL            UTIL_IsValidEntity( edict_t *pent );
extern BOOL            UTIL_TeamsMatch( const char *pTeamName1, const char *pTeamName2 );
extern BOOL            UTIL_IsFacing( entvars_t *pevTest, const Vector &reference ); //LRC

// Use for ease-in, ease-out style interpolation (accel/decel)
extern float        UTIL_SplineFraction( float value, float scale );

// Search for water transition along a vertical line
extern float        UTIL_WaterLevel( const Vector &position, float minz, float maxz );
extern void            UTIL_Bubbles( Vector mins, Vector maxs, int count );
extern void            UTIL_BubbleTrail( Vector from, Vector to, int count );

// allows precacheing of other entities
extern void            UTIL_PrecacheOther( const char *szClassname );

// prints a message to each client
extern void            UTIL_ClientPrintAll( int msg_dest, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL );
inline void            UTIL_CenterPrintAll( const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL ) 
{
    UTIL_ClientPrintAll( HUD_PRINTCENTER, msg_name, param1, param2, param3, param4 );
}

class CBasePlayerItem;
class CBasePlayer;
extern BOOL UTIL_GetNextBestWeapon( CBasePlayer *pPlayer, CBasePlayerItem *pCurrentWeapon );

// prints messages through the HUD
extern void ClientPrint( entvars_t *client, int msg_dest, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL );

// prints a message to the HUD say (chat)
extern void            UTIL_SayText( const char *pText, CBaseEntity *pEntity );
extern void            UTIL_SayTextAll( const char *pText, CBaseEntity *pEntity );


typedef struct hudtextparms_s
{
    float        x;
    float        y;
    int            effect;
    byte        r1, g1, b1, a1;
    byte        r2, g2, b2, a2;
    float        fadeinTime;
    float        fadeoutTime;
    float        holdTime;
    float        fxTime;
    int            channel;
} hudtextparms_t;

// prints as transparent 'title' to the HUD
extern void            UTIL_HudMessageAll( const hudtextparms_t &textparms, const char *pMessage );
extern void            UTIL_HudMessage( CBaseEntity *pEntity, const hudtextparms_t &textparms, const char *pMessage );

// for handy use with ClientPrint params
extern char *UTIL_dtos1( int d );
extern char *UTIL_dtos2( int d );
extern char *UTIL_dtos3( int d );
extern char *UTIL_dtos4( int d );

// Writes message to console with timestamp and FragLog header.
extern void            UTIL_LogPrintf( const char *fmt, ... );

// Sorta like FInViewCone, but for nonmonsters. 
extern float UTIL_DotPoints ( const Vector &vecSrc, const Vector &vecCheck, const Vector &vecDir );

extern void UTIL_StripToken( const char *pKey, char *pDest );// for redundant keynames

// Misc functions
extern int BuildChangeList( LEVELLIST *pLevelList, int maxList );

//
// How did I ever live without ASSERT?
//
#ifdef    DEBUG
void DBG_AssertFunction(BOOL fExpr, const char* szExpr, const char* szFile, int szLine, const char* szMessage);
#define ASSERT(f)        DBG_AssertFunction(f, #f, __FILE__, __LINE__, NULL)
#define ASSERTSZ(f, sz)    DBG_AssertFunction(f, #f, __FILE__, __LINE__, sz)
#else    // !DEBUG
#define ASSERT(f)
#define ASSERTSZ(f, sz)
#endif    // !DEBUG

//
// Constants that were used only by QC (maybe not used at all now)
//
// Un-comment only as needed
//
#define LANGUAGE_ENGLISH    0
#define LANGUAGE_GERMAN     1
#define LANGUAGE_FRENCH     2
#define LANGUAGE_BRITISH    3

extern DLL_GLOBAL int            g_Language;

#define SVC_TEMPENTITY       23
#define SVC_INTERMISSION     30
#define SVC_CDTRACK          32
#define SVC_WEAPONANIM       35
#define SVC_ROOMTYPE         37
#define SVC_DIRECTOR         51


#define PLAYBACK_EVENT( flags, who, index ) PLAYBACK_EVENT_FULL( flags, who, index, 0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, 0, 0, 0, 0 );
#define PLAYBACK_EVENT_DELAY( flags, who, index, delay ) PLAYBACK_EVENT_FULL( flags, who, index, delay, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, 0, 0, 0, 0 );


int UTIL_SharedRandomLong( unsigned int seed, int low, int high );
float UTIL_SharedRandomFloat( unsigned int seed, float low, float high );

float UTIL_WeaponTimeBase();

CBaseEntity* UTIL_FindEntityForward(CBaseEntity* pMe);

// LRC- for aliases and groups
CBaseEntity* UTIL_FollowReference( CBaseEntity* pStartEntity, const char* szName );

// for trigger_viewset
int HaveCamerasInPVS( edict_t* edict );
