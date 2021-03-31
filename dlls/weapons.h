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

#include "entities/effects/CBeam.h"
#include "entities/effects/CSprite.h"

// Weapon IDs
#define WEAPON_NONE                0
#define WEAPON_CROWBAR             1
#define WEAPON_GLOCK               2
#define WEAPON_PYTHON              3
#define WEAPON_MP5                 4
#define WEAPON_CHAINGUN            5
#define WEAPON_CROSSBOW            6
#define WEAPON_SHOTGUN             7
#define WEAPON_RPG                 8
#define WEAPON_GAUSS               9
#define WEAPON_EGON                10
#define WEAPON_HORNETGUN           11
#define WEAPON_HANDGRENADE         12
#define WEAPON_TRIPMINE            13
#define WEAPON_SATCHEL             14
#define WEAPON_SNARK               15

#define WEAPON_SUIT                31    // ?????
#define MAX_WEAPONS                32
#define WEAPON_ALLWEAPONS          (~(1<<WEAPON_SUIT))

// weapon weight factors (for auto-switching)   (-1 = noswitch)
#define CROWBAR_WEIGHT              0
#define GLOCK_WEIGHT               10
#define PYTHON_WEIGHT              15
#define MP5_WEIGHT                 15
#define SHOTGUN_WEIGHT             15
#define CROSSBOW_WEIGHT            10
#define RPG_WEIGHT                 20
#define GAUSS_WEIGHT               20
#define EGON_WEIGHT                20
#define HORNETGUN_WEIGHT           10
#define HANDGRENADE_WEIGHT          5
#define SNARK_WEIGHT                5
#define SATCHEL_WEIGHT            -10
#define TRIPMINE_WEIGHT           -10

// weapon clip/carry ammo capacities
#define URANIUM_MAX_CARRY         100
#define _9MM_MAX_CARRY            250
#define _357_MAX_CARRY             36
#define BUCKSHOT_MAX_CARRY        125
#define BOLT_MAX_CARRY             50
#define ROCKET_MAX_CARRY            5
#define HANDGRENADE_MAX_CARRY      10
#define SATCHEL_MAX_CARRY           5
#define TRIPMINE_MAX_CARRY          5
#define SNARK_MAX_CARRY            15
#define HORNET_MAX_CARRY            8
#define M203_GRENADE_MAX_CARRY     10

// the maximum amount of ammo each weapon's clip can hold
#define WEAPON_NOCLIP              -1

//#define CROWBAR_MAX_CLIP      WEAPON_NOCLIP
#define GLOCK_MAX_CLIP             17
#define PYTHON_MAX_CLIP             6
#define MP5_MAX_CLIP               50
#define MP5_DEFAULT_AMMO           25
#define SHOTGUN_MAX_CLIP            8
#define CROSSBOW_MAX_CLIP           5
#define RPG_MAX_CLIP                1
#define GAUSS_MAX_CLIP             WEAPON_NOCLIP
#define EGON_MAX_CLIP              WEAPON_NOCLIP
#define HORNETGUN_MAX_CLIP         WEAPON_NOCLIP
#define HANDGRENADE_MAX_CLIP       WEAPON_NOCLIP
#define SATCHEL_MAX_CLIP           WEAPON_NOCLIP
#define TRIPMINE_MAX_CLIP          WEAPON_NOCLIP
#define SNARK_MAX_CLIP             WEAPON_NOCLIP

// the default amount of ammo that comes with each gun when it spawns
#define GLOCK_DEFAULT_GIVE         17
#define PYTHON_DEFAULT_GIVE         6
#define MP5_DEFAULT_GIVE           25
#define MP5_DEFAULT_AMMO           25
#define MP5_M203_DEFAULT_GIVE       0
#define SHOTGUN_DEFAULT_GIVE       12
#define CROSSBOW_DEFAULT_GIVE       5
#define RPG_DEFAULT_GIVE            1
#define GAUSS_DEFAULT_GIVE         20
#define EGON_DEFAULT_GIVE          20
#define HANDGRENADE_DEFAULT_GIVE    5
#define SATCHEL_DEFAULT_GIVE        1
#define TRIPMINE_DEFAULT_GIVE       1
#define SNARK_DEFAULT_GIVE          5
#define HIVEHAND_DEFAULT_GIVE       8

// The amount of ammo given to a player by an ammo item.
#define AMMO_URANIUMBOX_GIVE       20
#define AMMO_GLOCKCLIP_GIVE       GLOCK_MAX_CLIP
#define AMMO_357BOX_GIVE          PYTHON_MAX_CLIP
#define AMMO_MP5CLIP_GIVE         MP5_MAX_CLIP
#define AMMO_CHAINBOX_GIVE        200
#define AMMO_M203BOX_GIVE           2
#define AMMO_BUCKSHOTBOX_GIVE      12
#define AMMO_CROSSBOWCLIP_GIVE    CROSSBOW_MAX_CLIP
#define AMMO_RPGCLIP_GIVE         RPG_MAX_CLIP
#define AMMO_URANIUMBOX_GIVE       20
#define AMMO_SNARKBOX_GIVE          5

// bullet types
typedef enum
{
    BULLET_NONE = 0,
    BULLET_PLAYER_9MM, // glock
    BULLET_PLAYER_MP5, // mp5
    BULLET_PLAYER_357, // python
    BULLET_PLAYER_BUCKSHOT, // shotgun
    BULLET_PLAYER_CROWBAR, // crowbar swipe

    BULLET_MONSTER_9MM,
    BULLET_MONSTER_MP5,
    BULLET_MONSTER_12MM,
} Bullet;

#define ITEM_FLAG_SELECTONEMPTY       1
#define ITEM_FLAG_NOAUTORELOAD        2
#define ITEM_FLAG_NOAUTOSWITCHEMPTY   4
#define ITEM_FLAG_LIMITINWORLD        8
#define ITEM_FLAG_EXHAUSTIBLE        16 // A player can totally exhaust their ammo supply and lose this weapon

#define WEAPON_IS_ONTARGET         0x40

typedef struct
{
    int iSlot;
    int iPosition;
    const char* pszAmmo1; // ammo 1 type
    int iMaxAmmo1; // max ammo 1
    const char* pszAmmo2; // ammo 2 type
    int iMaxAmmo2; // max ammo 2
    const char* pszName;
    int iMaxClip;
    int iId;
    int iFlags;
    int iWeight; // this value used to determine this weapon's importance in autoselection.
} ItemInfo;

typedef struct
{
    const char* pszName;
    int iId;
} AmmoInfo;

extern int giAmmoIndex;
void AddAmmoNameToAmmoRegistry(const char* szAmmoname);

extern DLL_GLOBAL  short        g_sModelIndexLaser;      // holds the index for the laser beam
extern DLL_GLOBAL  const char  *g_pModelNameLaser;

extern DLL_GLOBAL  short        g_sModelIndexLaserDot;   // holds the index for the laser beam dot
extern DLL_GLOBAL  short        g_sModelIndexFireball;   // holds the index for the fireball
extern DLL_GLOBAL  short        g_sModelIndexSmoke;      // holds the index for the smoke cloud
extern DLL_GLOBAL  short        g_sModelIndexWExplosion; // holds the index for the underwater explosion
extern DLL_GLOBAL  short        g_sModelIndexBubbles;    // holds the index for the bubbles model
extern DLL_GLOBAL  short        g_sModelIndexBloodDrop;  // holds the sprite index for blood drops
extern DLL_GLOBAL  short        g_sModelIndexBloodSpray; // holds the sprite index for blood spray (bigger)

extern void ClearMultiDamage();
extern void ApplyMultiDamage(entvars_t* pevInflictor, entvars_t* pevAttacker );
extern void AddMultiDamage( entvars_t *pevInflictor, CBaseEntity *pEntity, float flDamage, int bitsDamageType);

extern void DecalGunshot( TraceResult *pTrace, int iBulletType );
extern void SpawnBlood(Vector vecSpot, int bloodColor, float flDamage);
extern int DamageDecal( CBaseEntity *pEntity, int bitsDamageType );
extern void RadiusDamage( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, int bitsDamageType );

typedef struct
{
    CBaseEntity* pEntity;
    float amount;
    int type;
} MULTIDAMAGE;

extern MULTIDAMAGE gMultiDamage;

#define LOUD_GUN_VOLUME          1000
#define NORMAL_GUN_VOLUME         600
#define QUIET_GUN_VOLUME          200

#define BRIGHT_GUN_FLASH          512
#define NORMAL_GUN_FLASH          256
#define DIM_GUN_FLASH             128

#define BIG_EXPLOSION_VOLUME     2048
#define NORMAL_EXPLOSION_VOLUME  1024
#define SMALL_EXPLOSION_VOLUME    512

#define WEAPON_ACTIVITY_VOLUME     64

#define VECTOR_CONE_1DEGREES     Vector( 0.00873, 0.00873, 0.00873 )
#define VECTOR_CONE_2DEGREES     Vector( 0.01745, 0.01745, 0.01745 )
#define VECTOR_CONE_3DEGREES     Vector( 0.02618, 0.02618, 0.02618 )
#define VECTOR_CONE_4DEGREES     Vector( 0.03490, 0.03490, 0.03490 )
#define VECTOR_CONE_5DEGREES     Vector( 0.04362, 0.04362, 0.04362 )
#define VECTOR_CONE_6DEGREES     Vector( 0.05234, 0.05234, 0.05234 )
#define VECTOR_CONE_7DEGREES     Vector( 0.06105, 0.06105, 0.06105 )
#define VECTOR_CONE_8DEGREES     Vector( 0.06976, 0.06976, 0.06976 )
#define VECTOR_CONE_9DEGREES     Vector( 0.07846, 0.07846, 0.07846 )
#define VECTOR_CONE_10DEGREES    Vector( 0.08716, 0.08716, 0.08716 )
#define VECTOR_CONE_15DEGREES    Vector( 0.13053, 0.13053, 0.13053 )
#define VECTOR_CONE_20DEGREES    Vector( 0.17365, 0.17365, 0.17365 )

#ifdef CLIENT_DLL
bool bIsMultiplayer ();
void LoadVModel ( const char *szViewModel, CBasePlayer *m_pPlayer );
#endif

enum glock_e
{
    GLOCK_IDLE1 = 0,
    GLOCK_IDLE2,
    GLOCK_IDLE3,
    GLOCK_SHOOT,
    GLOCK_SHOOT_EMPTY,
    GLOCK_RELOAD,
    GLOCK_RELOAD_NOT_EMPTY,
    GLOCK_DRAW,
    GLOCK_HOLSTER,
    GLOCK_ADD_SILENCER
};

enum crowbar_e
{
    CROWBAR_IDLE = 0,
    CROWBAR_DRAW,
    CROWBAR_HOLSTER,
    CROWBAR_ATTACK1HIT,
    CROWBAR_ATTACK1MISS,
    CROWBAR_ATTACK2MISS,
    CROWBAR_ATTACK2HIT,
    CROWBAR_ATTACK3MISS,
    CROWBAR_ATTACK3HIT
};

enum python_e
{
    PYTHON_IDLE1 = 0,
    PYTHON_FIDGET,
    PYTHON_FIRE1,
    PYTHON_RELOAD,
    PYTHON_HOLSTER,
    PYTHON_DRAW,
    PYTHON_IDLE2,
    PYTHON_IDLE3
};

enum mp5_e
{
    MP5_LONGIDLE = 0,
    MP5_IDLE1,
    MP5_LAUNCH,
    MP5_RELOAD,
    MP5_DEPLOY,
    MP5_FIRE1,
    MP5_FIRE2,
    MP5_FIRE3,
    MP5_HOLSTER
};

enum crossbow_e
{
    CROSSBOW_IDLE1 = 0, // full
    CROSSBOW_IDLE2,     // empty
    CROSSBOW_FIDGET1,   // full
    CROSSBOW_FIDGET2,   // empty
    CROSSBOW_FIRE1,     // full
    CROSSBOW_FIRE2,     // reload
    CROSSBOW_FIRE3,     // empty
    CROSSBOW_RELOAD,    // from empty
    CROSSBOW_DRAW1,     // full
    CROSSBOW_DRAW2,     // empty
    CROSSBOW_HOLSTER1,  // full
    CROSSBOW_HOLSTER2,  // empty
};

enum shotgun_e
{
    SHOTGUN_IDLE = 0,
    SHOTGUN_FIRE,
    SHOTGUN_FIRE2,
    SHOTGUN_RELOAD,
    SHOTGUN_PUMP,
    SHOTGUN_START_RELOAD,
    SHOTGUN_DRAW,
    SHOTGUN_HOLSTER,
    SHOTGUN_IDLE4,
    SHOTGUN_IDLE_DEEP
};

enum rpg_e
{
    RPG_IDLE = 0,
    RPG_FIDGET,
    RPG_RELOAD,    // to reload
    RPG_FIRE2,     // to empty
    RPG_HOLSTER1,  // loaded
    RPG_DRAW1,     // loaded
    RPG_HOLSTER2,  // unloaded
    RPG_DRAW_UL,   // unloaded
    RPG_IDLE_UL,   // unloaded idle
    RPG_FIDGET_UL, // unloaded fidget
};

#define GAUSS_PRIMARY_CHARGE_VOLUME  256 // how loud gauss is while charging
#define GAUSS_PRIMARY_FIRE_VOLUME    450 // how loud gauss is when discharged

enum gauss_e
{
    GAUSS_IDLE = 0,
    GAUSS_IDLE2,
    GAUSS_FIDGET,
    GAUSS_SPINUP,
    GAUSS_SPIN,
    GAUSS_FIRE,
    GAUSS_FIRE2,
    GAUSS_HOLSTER,
    GAUSS_DRAW
};

enum egon_e
{
    EGON_IDLE1 = 0,
    EGON_FIDGET1,
    EGON_ALTFIREON,
    EGON_ALTFIRECYCLE,
    EGON_ALTFIREOFF,
    EGON_FIRE1,
    EGON_FIRE2,
    EGON_FIRE3,
    EGON_FIRE4,
    EGON_DRAW,
    EGON_HOLSTER
};

enum EGON_FIRESTATE
{
    FIRE_OFF,
    FIRE_CHARGE
};

enum EGON_FIREMODE
{
    FIRE_NARROW,
    FIRE_WIDE
};

#define EGON_PRIMARY_VOLUME  450
#define EGON_BEAM_SPRITE     "sprites/xbeam1.spr"
#define EGON_FLARE_SPRITE    "sprites/XSpark1.spr"
#define EGON_SOUND_OFF       "weapons/egon_off1.wav"
#define EGON_SOUND_RUN       "weapons/egon_run3.wav"
#define EGON_SOUND_STARTUP   "weapons/egon_windup2.wav"

enum hgun_e
{
    HGUN_IDLE1 = 0,
    HGUN_FIDGETSWAY,
    HGUN_FIDGETSHAKE,
    HGUN_DOWN,
    HGUN_UP,
    HGUN_SHOOT
};

enum handgrenade_e
{
    HANDGRENADE_IDLE = 0,
    HANDGRENADE_FIDGET,
    HANDGRENADE_PINPULL,
    HANDGRENADE_THROW1,  // toss
    HANDGRENADE_THROW2,  // medium
    HANDGRENADE_THROW3,  // hard
    HANDGRENADE_HOLSTER,
    HANDGRENADE_DRAW
};

enum satchel_e
{
    SATCHEL_IDLE1 = 0,
    SATCHEL_FIDGET1,
    SATCHEL_DRAW,
    SATCHEL_DROP
};

enum satchel_radio_e
{
    SATCHEL_RADIO_IDLE1 = 0,
    SATCHEL_RADIO_FIDGET1,
    SATCHEL_RADIO_DRAW,
    SATCHEL_RADIO_FIRE,
    SATCHEL_RADIO_HOLSTER
};

enum tripmine_e
{
    TRIPMINE_IDLE1 = 0,
    TRIPMINE_IDLE2,
    TRIPMINE_ARM1,
    TRIPMINE_ARM2,
    TRIPMINE_FIDGET,
    TRIPMINE_HOLSTER,
    TRIPMINE_DRAW,
    TRIPMINE_WORLD,
    TRIPMINE_GROUND,
};

enum squeak_e
{
    SQUEAK_IDLE1 = 0,
    SQUEAK_FIDGETFIT,
    SQUEAK_FIDGETNIP,
    SQUEAK_DOWN,
    SQUEAK_UP,
    SQUEAK_THROW
};
