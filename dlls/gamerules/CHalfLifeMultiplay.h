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

#include "CGameRules.h"

//=========================================================
// CHalfLifeMultiplay - rules for the basic half life multiplayer
// competition
//=========================================================
class CHalfLifeMultiplay : public CGameRules
{
public:
    CHalfLifeMultiplay();

    // GR_Think
    void Think() override;
    void RefreshSkillData() override;
    BOOL IsAllowedToSpawn(CBaseEntity* pEntity) override;
    BOOL FAllowFlashlight() override;

    BOOL FShouldSwitchWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon) override;
    BOOL GetNextBestWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pCurrentWeapon) override;

    // Functions to verify the single/multiplayer status of a game
    BOOL IsMultiplayer() override;
    BOOL IsDeathmatch() override;
    BOOL IsCoOp() override;

    // Client connection/disconnection
    // If ClientConnected returns FALSE, the connection is rejected and the user is provided the reason specified in
    //  svRejectReason
    // Only the client's name and remote address are provided to the dll for verification.
    BOOL ClientConnected(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]) override;
    void InitHUD(CBasePlayer* pl) override; // the client dll is ready for updating
    void ClientDisconnected(edict_t* pClient) override;
    void UpdateGameMode(CBasePlayer* pPlayer) override; // the client needs to be informed of the current game mode

    // Client damage rules
    float FlPlayerFallDamage(CBasePlayer* pPlayer) override;
    BOOL FPlayerCanTakeDamage(CBasePlayer* pPlayer, CBaseEntity* pAttacker) override;

    // Client spawn/respawn control
    void PlayerSpawn(CBasePlayer* pPlayer) override;
    void PlayerThink(CBasePlayer* pPlayer) override;
    BOOL FPlayerCanRespawn(CBasePlayer* pPlayer) override;
    float FlPlayerSpawnTime(CBasePlayer* pPlayer) override;
    edict_t* GetPlayerSpawnSpot(CBasePlayer* pPlayer) override;

    BOOL AllowAutoTargetCrosshair() override;
    BOOL ClientCommand(CBasePlayer* pPlayer, const char* pcmd) override;
    void ClientUserInfoChanged(CBasePlayer* pPlayer, char* infobuffer) override;

    // Client kills/scoring
    int IPointsForKill(CBasePlayer* pAttacker, CBasePlayer* pKilled) override;
    void PlayerKilled(CBasePlayer* pVictim, entvars_t* pKiller, entvars_t* pInflictor) override;
    void DeathNotice(CBasePlayer* pVictim, entvars_t* pKiller, entvars_t* pInflictor) override;

    // Weapon retrieval
    void PlayerGotWeapon(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon) override;
    BOOL CanHavePlayerItem(CBasePlayer* pPlayer, CBasePlayerItem* pWeapon) override; // The player is touching an CBasePlayerItem, do I give it to him?

    // Weapon spawn/respawn control
    int WeaponShouldRespawn(CBasePlayerItem* pWeapon) override;
    float FlWeaponRespawnTime(CBasePlayerItem* pWeapon) override;
    float FlWeaponTryRespawn(CBasePlayerItem* pWeapon) override;
    Vector VecWeaponRespawnSpot(CBasePlayerItem* pWeapon) override;

    // Item retrieval
    BOOL CanHaveItem(CBasePlayer* pPlayer, CItem* pItem) override;
    void PlayerGotItem(CBasePlayer* pPlayer, CItem* pItem) override;

    // Item spawn/respawn control
    int ItemShouldRespawn(CItem* pItem) override;
    float FlItemRespawnTime(CItem* pItem) override;
    Vector VecItemRespawnSpot(CItem* pItem) override;

    // Ammo retrieval
    void PlayerGotAmmo(CBasePlayer* pPlayer, char* szName, int iCount) override;

    // Ammo spawn/respawn control
    int AmmoShouldRespawn(CBasePlayerAmmo* pAmmo) override;
    float FlAmmoRespawnTime(CBasePlayerAmmo* pAmmo) override;
    Vector VecAmmoRespawnSpot(CBasePlayerAmmo* pAmmo) override;

    // Healthcharger respawn control
    float FlHealthChargerRechargeTime() override;
    float FlHEVChargerRechargeTime() override;

    // What happens to a dead player's weapons
    int DeadPlayerWeapons(CBasePlayer* pPlayer) override;

    // What happens to a dead player's ammo    
    int DeadPlayerAmmo(CBasePlayer* pPlayer) override;

    // Teamplay stuff    
    const char* GetTeamID(CBaseEntity* pEntity) override { return ""; }
    int PlayerRelationship(CBaseEntity* pPlayer, CBaseEntity* pTarget) override;

    BOOL PlayTextureSounds() override { return FALSE; }
    BOOL PlayFootstepSounds(CBasePlayer* pl, float fvol) override;

    // Monsters
    BOOL FAllowMonsters() override;

    // Immediately end a multiplayer game
    void EndMultiplayerGame() override { GoToIntermission(); }

protected:
    virtual void ChangeLevel();
    virtual void GoToIntermission();
    float m_flIntermissionEndTime;
    BOOL m_iEndIntermissionButtonHit;
    void SendMOTDToClient(edict_t* client);
};
