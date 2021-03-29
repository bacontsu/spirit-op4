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

#include "extdll.h"
#include "util.h"
#include "monsters.h"
#include "weapons.h"
#include "util/decals.h"
#include "gamerules/CGameRules.h"
#include "util/usermessages.h"

extern int gEvilImpulse101;

#define NOT_USED 255

DLL_GLOBAL  short        g_sModelIndexLaser;            // holds the index for the laser beam
DLL_GLOBAL  const char  *g_pModelNameLaser = "sprites/laserbeam.spr";
DLL_GLOBAL  short        g_sModelIndexLaserDot;         // holds the index for the laser beam dot
DLL_GLOBAL  short        g_sModelIndexFireball;         // holds the index for the fireball
DLL_GLOBAL  short        g_sModelIndexSmoke;            // holds the index for the smoke cloud
DLL_GLOBAL  short        g_sModelIndexWExplosion;       // holds the index for the underwater explosion
DLL_GLOBAL  short        g_sModelIndexBubbles;          // holds the index for the bubbles model
DLL_GLOBAL  short        g_sModelIndexBloodDrop;        // holds the sprite index for the initial blood
DLL_GLOBAL  short        g_sModelIndexBloodSpray;       // holds the sprite index for splattered blood

MULTIDAMAGE gMultiDamage;
    
/*
==============================================================================

MULTI-DAMAGE

Collects multiple small damages into a single damage

==============================================================================
*/

//
// ClearMultiDamage - resets the global multi damage accumulator
//
void ClearMultiDamage()
{
    gMultiDamage.pEntity = NULL;
    gMultiDamage.amount    = 0;
    gMultiDamage.type = 0;
}

//
// ApplyMultiDamage - inflicts contents of global multi damage register on gMultiDamage.pEntity
//
// GLOBALS USED:
//        gMultiDamage

void ApplyMultiDamage(entvars_t *pevInflictor, entvars_t *pevAttacker )
{
    Vector        vecSpot1;//where blood comes from
    Vector        vecDir;//direction blood should go
    TraceResult    tr;
    
    if ( !gMultiDamage.pEntity )
        return;

    gMultiDamage.pEntity->TakeDamage(pevInflictor, pevAttacker, gMultiDamage.amount, gMultiDamage.type );
}

// GLOBALS USED:
//        gMultiDamage

void AddMultiDamage( entvars_t *pevInflictor, CBaseEntity *pEntity, float flDamage, int bitsDamageType)
{
    if ( !pEntity )
        return;
    
    gMultiDamage.type |= bitsDamageType;

    if ( pEntity != gMultiDamage.pEntity )
    {
        ApplyMultiDamage(pevInflictor,pevInflictor); // UNDONE: wrong attacker!
        gMultiDamage.pEntity    = pEntity;
        gMultiDamage.amount        = 0;
    }

    gMultiDamage.amount += flDamage;
}

/*
================
SpawnBlood
================
*/
void SpawnBlood(Vector vecSpot, int bloodColor, float flDamage)
{
    UTIL_BloodDrips( vecSpot, g_vecAttackDir, bloodColor, (int)flDamage );
}

int DamageDecal( CBaseEntity *pEntity, int bitsDamageType )
{
    if ( !pEntity )
        return (DECAL_GUNSHOT1 + RANDOM_LONG(0,4));
    
    return pEntity->DamageDecal( bitsDamageType );
}

void DecalGunshot( TraceResult *pTrace, int iBulletType )
{
    // Is the entity valid
    if ( !UTIL_IsValidEntity( pTrace->pHit ) )
        return;

    if ( VARS(pTrace->pHit)->solid == SOLID_BSP || VARS(pTrace->pHit)->movetype == MOVETYPE_PUSHSTEP )
    {
        CBaseEntity *pEntity = NULL;
        // Decal the wall with a gunshot
        if ( !FNullEnt(pTrace->pHit) )
            pEntity = CBaseEntity::Instance(pTrace->pHit);

        switch( iBulletType )
        {
        case BULLET_PLAYER_9MM:
        case BULLET_MONSTER_9MM:
        case BULLET_PLAYER_MP5:
        case BULLET_MONSTER_MP5:
        case BULLET_PLAYER_BUCKSHOT:
        case BULLET_PLAYER_357:
        default:
            // smoke and decal
            UTIL_GunshotDecalTrace( pTrace, DamageDecal( pEntity, DMG_BULLET ) );
            break;
        case BULLET_MONSTER_12MM:
            // smoke and decal
            UTIL_GunshotDecalTrace( pTrace, DamageDecal( pEntity, DMG_BULLET ) );
            break;
        case BULLET_PLAYER_CROWBAR:
            // wall decal
            UTIL_DecalTrace( pTrace, DamageDecal( pEntity, DMG_CLUB ) );
            break;
        }
    }
}

//
// EjectBrass - tosses a brass shell from passed origin at passed velocity
//
void EjectBrass ( const Vector &vecOrigin, const Vector &vecVelocity, float rotation, int model, int soundtype )
{
    // FIX: when the player shoots, their gun isn't in the same position as it is on the model other players see.

    MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, vecOrigin );
        WRITE_BYTE( TE_MODEL);
        WRITE_COORD( vecOrigin.x);
        WRITE_COORD( vecOrigin.y);
        WRITE_COORD( vecOrigin.z);
        WRITE_COORD( vecVelocity.x);
        WRITE_COORD( vecVelocity.y);
        WRITE_COORD( vecVelocity.z);
        WRITE_ANGLE( rotation );
        WRITE_SHORT( model );
        WRITE_BYTE ( soundtype);
        WRITE_BYTE ( 25 );// 2.5 seconds
    MESSAGE_END();
}

int giAmmoIndex = 0;

// Precaches the ammo and queues the ammo info for sending to clients
void AddAmmoNameToAmmoRegistry( const char *szAmmoname )
{
    // make sure it's not already in the registry
    for ( int i = 0; i < MAX_AMMO_SLOTS; i++ )
    {
        if ( !CBasePlayerItem::AmmoInfoArray[i].pszName)
            continue;

        if ( stricmp( CBasePlayerItem::AmmoInfoArray[i].pszName, szAmmoname ) == 0 )
            return; // ammo already in registry, just quite
    }


    giAmmoIndex++;
    ASSERT( giAmmoIndex < MAX_AMMO_SLOTS );
    if ( giAmmoIndex >= MAX_AMMO_SLOTS )
        giAmmoIndex = 0;

    CBasePlayerItem::AmmoInfoArray[giAmmoIndex].pszName = szAmmoname;
    CBasePlayerItem::AmmoInfoArray[giAmmoIndex].iId = giAmmoIndex;   // yes, this info is redundant
}

// Precaches the weapon and queues the weapon info for sending to clients
void UTIL_PrecacheOtherWeapon( const char *szClassname )
{
    edict_t    *pent;

    pent = CREATE_NAMED_ENTITY( MAKE_STRING( szClassname ) );
    if ( FNullEnt( pent ) )
    {
        ALERT ( at_debug, "NULL Ent in UTIL_PrecacheOtherWeapon\n" );
        return;
    }
    
    CBaseEntity *pEntity = CBaseEntity::Instance (VARS( pent ));

    if (pEntity)
    {
        ItemInfo II;
        pEntity->Precache( );
        memset( &II, 0, sizeof II );
        if ( ((CBasePlayerItem*)pEntity)->GetItemInfo( &II ) )
        {
            CBasePlayerItem::ItemInfoArray[II.iId] = II;

            if ( II.pszAmmo1 && *II.pszAmmo1 )
            {
                AddAmmoNameToAmmoRegistry( II.pszAmmo1 );
            }

            if ( II.pszAmmo2 && *II.pszAmmo2 )
            {
                AddAmmoNameToAmmoRegistry( II.pszAmmo2 );
            }

            memset( &II, 0, sizeof II );
        }
    }

    REMOVE_ENTITY(pent);
}

// called by worldspawn
void W_Precache()
{
    memset( CBasePlayerItem::ItemInfoArray, 0, sizeof(CBasePlayerItem::ItemInfoArray) );
    memset( CBasePlayerItem::AmmoInfoArray, 0, sizeof(CBasePlayerItem::AmmoInfoArray) );
    giAmmoIndex = 0;

    UTIL_PrecacheOtherWeapon( "weapon_debug" );

    // shotgun
    UTIL_PrecacheOtherWeapon( "weapon_shotgun" );
    UTIL_PrecacheOther( "ammo_buckshot" );

    // crowbar
    UTIL_PrecacheOtherWeapon( "weapon_crowbar" );

    // glock
    UTIL_PrecacheOtherWeapon( "weapon_9mmhandgun" );
    UTIL_PrecacheOther( "ammo_9mmclip" );
    UTIL_PrecacheOther( "ammo_9mmbox" ); //LRC

    // mp5
    UTIL_PrecacheOtherWeapon( "weapon_9mmAR" );
    UTIL_PrecacheOther( "ammo_9mmAR" );
    UTIL_PrecacheOther( "ammo_ARgrenades" );

#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )
    // python
    UTIL_PrecacheOtherWeapon( "weapon_357" );
    UTIL_PrecacheOther( "ammo_357" );
#endif
    
#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )
    // gauss
    UTIL_PrecacheOtherWeapon( "weapon_gauss" );
    UTIL_PrecacheOther( "ammo_gaussclip" );
#endif

#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )
    // rpg
    UTIL_PrecacheOtherWeapon( "weapon_rpg" );
    UTIL_PrecacheOther( "ammo_rpgclip" );
#endif

#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )
    // crossbow
    UTIL_PrecacheOtherWeapon( "weapon_crossbow" );
    UTIL_PrecacheOther( "ammo_crossbow" );
#endif

#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )
    // egon
    UTIL_PrecacheOtherWeapon( "weapon_egon" );
#endif

    // tripmine
    UTIL_PrecacheOtherWeapon( "weapon_tripmine" );

#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )
    // satchel charge
    UTIL_PrecacheOtherWeapon( "weapon_satchel" );
#endif

    // hand grenade
    UTIL_PrecacheOtherWeapon("weapon_handgrenade");

#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )
    // squeak grenade
    UTIL_PrecacheOtherWeapon( "weapon_snark" );
#endif

#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )
    // hornetgun
    UTIL_PrecacheOtherWeapon( "weapon_hornetgun" );
#endif


#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )
    if ( g_pGameRules->IsDeathmatch() )
    {
        UTIL_PrecacheOther( "weaponbox" );// container for dropped deathmatch weapons
    }
#endif

    g_sModelIndexFireball = PRECACHE_MODEL ("sprites/zerogxplode.spr");// fireball
    g_sModelIndexWExplosion = PRECACHE_MODEL ("sprites/WXplo1.spr");// underwater fireball
    g_sModelIndexSmoke = PRECACHE_MODEL ("sprites/steam1.spr");// smoke
    g_sModelIndexBubbles = PRECACHE_MODEL ("sprites/bubble.spr");//bubbles
    g_sModelIndexBloodSpray = PRECACHE_MODEL ("sprites/bloodspray.spr"); // initial blood
    g_sModelIndexBloodDrop = PRECACHE_MODEL ("sprites/blood.spr"); // splattered blood 

    g_sModelIndexLaser = PRECACHE_MODEL( (char *)g_pModelNameLaser );
    g_sModelIndexLaserDot = PRECACHE_MODEL("sprites/laserdot.spr");


    // used by explosions
    PRECACHE_MODEL ("models/grenade.mdl");
    PRECACHE_MODEL ("sprites/explode1.spr");

    PRECACHE_SOUND ("weapons/debris1.wav");// explosion aftermaths
    PRECACHE_SOUND ("weapons/debris2.wav");// explosion aftermaths
    PRECACHE_SOUND ("weapons/debris3.wav");// explosion aftermaths

    PRECACHE_SOUND ("weapons/grenade_hit1.wav");//grenade
    PRECACHE_SOUND ("weapons/grenade_hit2.wav");//grenade
    PRECACHE_SOUND ("weapons/grenade_hit3.wav");//grenade

    PRECACHE_SOUND ("weapons/bullet_hit1.wav");    // hit by bullet
    PRECACHE_SOUND ("weapons/bullet_hit2.wav");    // hit by bullet
    
    PRECACHE_SOUND ("items/weapondrop1.wav");// weapon falls to the ground

}

