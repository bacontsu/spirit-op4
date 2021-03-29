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

#include "entities/CBaseEntity.h"
#include "entities/items/CItem.h"
#include "entities/ammo/CBasePlayerAmmo.h"

#define GAME_NAME "Spirit-Level - Development"

// weapon respawning return codes
enum
{
    GR_NONE = 0,

    GR_WEAPON_RESPAWN_YES,
    GR_WEAPON_RESPAWN_NO,

    GR_AMMO_RESPAWN_YES,
    GR_AMMO_RESPAWN_NO,

    GR_ITEM_RESPAWN_YES,
    GR_ITEM_RESPAWN_NO,

    GR_PLR_DROP_GUN_ALL,
    GR_PLR_DROP_GUN_ACTIVE,
    GR_PLR_DROP_GUN_NO,

    GR_PLR_DROP_AMMO_ALL,
    GR_PLR_DROP_AMMO_ACTIVE,
    GR_PLR_DROP_AMMO_NO,
};

// Player relationship return codes
enum
{
    GR_NOTTEAMMATE = 0,
    GR_TEAMMATE,
    GR_ENEMY,
    GR_ALLY,
    GR_NEUTRAL,
};

class CGameRules
{
public:
    virtual void RefreshSkillData(); // fill skill data struct with proper values
    virtual void Think() = 0; // GR_Think - runs every server frame, should handle any timer tasks, periodic events, etc.
    virtual BOOL IsAllowedToSpawn(CBaseEntity* pEntity) = 0; // Can this item spawn (eg monsters don't spawn in deathmatch).

    virtual BOOL FAllowFlashlight() = 0; // Are players allowed to switch on their flashlight?
    virtual BOOL FShouldSwitchWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon) = 0; // should the player switch to this weapon?
    virtual BOOL GetNextBestWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pCurrentWeapon) = 0; // I can't use this weapon anymore, get me the next best one.

    // Functions to verify the single/multiplayer status of a game
    virtual BOOL IsMultiplayer() = 0; // is this a multiplayer game? (either coop or deathmatch)
    virtual BOOL IsDeathmatch() = 0; //is this a deathmatch game?
    virtual BOOL IsTeamplay() { return FALSE; } // is this deathmatch game being played with team rules?
    virtual BOOL IsCoOp() = 0; // is this a coop game?
    virtual const char* GetGameDescription() { return GAME_NAME; } // this is the game name that gets seen in the server browser

    // Client connection/disconnection
    virtual BOOL ClientConnected(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]) = 0;
    // a client just connected to the server (player hasn't spawned yet)
    virtual void InitHUD(CBasePlayer* pl) = 0; // the client dll is ready for updating
    virtual void ClientDisconnected(edict_t* pClient) = 0; // a client just disconnected from the server
    virtual void UpdateGameMode(CBasePlayer* pPlayer)
    {
    } // the client needs to be informed of the current game mode

    // Client damage rules
    virtual float FlPlayerFallDamage(CBasePlayer* pPlayer) = 0; // this client just hit the ground after a fall. How much damage?
    virtual BOOL FPlayerCanTakeDamage(CBasePlayer* pPlayer, CBaseEntity* pAttacker) { return TRUE; } // can this player take damage from this attacker?
    virtual BOOL ShouldAutoAim(CBasePlayer* pPlayer, edict_t* target) { return TRUE; }

    // Client spawn/respawn control
    virtual void PlayerSpawn(CBasePlayer* pPlayer) = 0; // called by CBasePlayer::Spawn just before releasing player into the game
    virtual void PlayerThink(CBasePlayer* pPlayer) = 0; // called by CBasePlayer::PreThink every frame, before physics are run and after keys are accepted
    virtual BOOL FPlayerCanRespawn(CBasePlayer* pPlayer) = 0; // is this player allowed to respawn now?
    virtual float FlPlayerSpawnTime(CBasePlayer* pPlayer) = 0; // When in the future will this player be able to spawn?
    virtual edict_t* GetPlayerSpawnSpot(CBasePlayer* pPlayer); // Place this player on their spawnspot and face them the proper direction.

    virtual BOOL AllowAutoTargetCrosshair() { return TRUE; }
    virtual BOOL ClientCommand(CBasePlayer* pPlayer, const char* pcmd) { return FALSE; }
    // handles the user commands;  returns TRUE if command handled properly
    virtual void ClientUserInfoChanged(CBasePlayer* pPlayer, char* infobuffer)
    {
    } // the player has changed userinfo;  can change it now

    // Client kills/scoring
    virtual int IPointsForKill(CBasePlayer* pAttacker, CBasePlayer* pKilled) = 0; // how many points do I award whoever kills this player?
    virtual void PlayerKilled(CBasePlayer* pVictim, entvars_t* pKiller, entvars_t* pInflictor) = 0; // Called each time a player dies
    virtual void DeathNotice(CBasePlayer* pVictim, entvars_t* pKiller, entvars_t* pInflictor) = 0;
    // Call this from within a GameRules class to report an obituary.
    // Weapon retrieval
    virtual BOOL CanHavePlayerItem(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon); // The player is touching an CBasePlayerItem, do I give it to him?
    virtual void PlayerGotWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon) = 0; // Called each time a player picks up a weapon from the ground

    // Weapon spawn/respawn control
    virtual int WeaponShouldRespawn(CBasePlayerItem* pWeapon) = 0; // should this weapon respawn?
    virtual float FlWeaponRespawnTime(CBasePlayerItem* pWeapon) = 0; // when may this weapon respawn?
    virtual float FlWeaponTryRespawn(CBasePlayerItem* pWeapon) = 0; // can i respawn now,  and if not, when should i try again?
    virtual Vector VecWeaponRespawnSpot(CBasePlayerItem* pWeapon) = 0; // where in the world should this weapon respawn?

    // Item retrieval
    virtual BOOL CanHaveItem(CBasePlayer* pPlayer, CItem* pItem) = 0; // is this player allowed to take this item?
    virtual void PlayerGotItem(CBasePlayer* pPlayer, CItem* pItem) = 0; // call each time a player picks up an item (battery, healthkit, longjump)

    // Item spawn/respawn control
    virtual int ItemShouldRespawn(CItem* pItem) = 0; // Should this item respawn?
    virtual float FlItemRespawnTime(CItem* pItem) = 0; // when may this item respawn?
    virtual Vector VecItemRespawnSpot(CItem* pItem) = 0; // where in the world should this item respawn?

    // Ammo retrieval
    virtual BOOL CanHaveAmmo(CBasePlayer* pPlayer, const char* pszAmmoName, int iMaxCarry); // can this player take more of this ammo?
    virtual void PlayerGotAmmo(CBasePlayer* pPlayer, char* szName, int iCount) = 0; // called each time a player picks up some ammo in the world

    // Ammo spawn/respawn control
    virtual int AmmoShouldRespawn(CBasePlayerAmmo* pAmmo) = 0; // should this ammo item respawn?
    virtual float FlAmmoRespawnTime(CBasePlayerAmmo* pAmmo) = 0; // when should this ammo item respawn?
    virtual Vector VecAmmoRespawnSpot(CBasePlayerAmmo* pAmmo) = 0; // where in the world should this ammo item respawn?
    // by default, everything spawns

    // Healthcharger respawn control
    virtual float FlHealthChargerRechargeTime() = 0; // how long until a depleted HealthCharger recharges itself?
    virtual float FlHEVChargerRechargeTime() { return 0; } // how long until a depleted HealthCharger recharges itself?

    // What happens to a dead player's weapons
    virtual int DeadPlayerWeapons(CBasePlayer* pPlayer) = 0; // what do I do with a player's weapons when he's killed?

    // What happens to a dead player's ammo    
    virtual int DeadPlayerAmmo(CBasePlayer* pPlayer) = 0; // Do I drop ammo when the player dies? How much?

    // Teamplay stuff
    virtual const char* GetTeamID(CBaseEntity* pEntity) = 0; // what team is this entity on?
    virtual int PlayerRelationship(CBaseEntity* pPlayer, CBaseEntity* pTarget) = 0; // What is the player's relationship with this entity?
    virtual int GetTeamIndex(const char* pTeamName) { return -1; }
    virtual const char* GetIndexedTeamName(int teamIndex) { return ""; }
    virtual BOOL IsValidTeam(const char* pTeamName) { return TRUE; }

    virtual void ChangePlayerTeam(CBasePlayer* pPlayer, const char* pTeamName, BOOL bKill, BOOL bGib)
    {
    }

    virtual const char* SetDefaultPlayerTeam(CBasePlayer* pPlayer) { return ""; }

    // Sounds
    virtual BOOL PlayTextureSounds() { return TRUE; }
    virtual BOOL PlayFootstepSounds(CBasePlayer* pl, float fvol) { return TRUE; }

    // Monsters
    virtual BOOL FAllowMonsters() = 0; //are monsters allowed

    // Immediately end a multiplayer game
    virtual void EndMultiplayerGame()
    {
    }
};

extern CGameRules* InstallGameRules();

extern DLL_GLOBAL CGameRules* g_pGameRules;

//LRC- much as I hate to add new globals, I can't see how to read data from the World entity.
extern BOOL g_startSuit;
