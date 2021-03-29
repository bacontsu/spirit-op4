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

#include "CHudAmmo.h"
#include "CHudAmmoSecondary.h"
#include "CHudBase.h"
#include "CHudBattery.h"
#include "CHudDeathNotice.h"
#include "CHudFlashlight.h"
#include "CHudGeiger.h"
#include "CHudHealth.h"
#include "CHudMenu.h"
#include "CHudMessage.h"
#include "CHudParticle.h"
#include "CHudSayText.h"
#include "CHudSpectator.h"
#include "CHudStatusBar.h"
#include "CHudStatusIcons.h"
#include "CHudTextMessage.h"
#include "CHudTrain.h"

#define HUD_ACTIVE	1
#define HUD_INTERMISSION 2

#define MAX_PLAYER_NAME_LENGTH 32

struct HUDLIST
{
    CHudBase* p;
    HUDLIST* pNext;
};

/// <summary>
/// CHud handles the message, calculation, and drawing the HUD
/// </summary>
class CHud
{
private:
    HUDLIST* m_pHudList;
    HSPRITE m_hsprLogo;
    int m_iLogo;
    client_sprite_t* m_pSpriteList;
    int m_iSpriteCount;
    int m_iSpriteCountAllRes;
    float m_flMouseSensitivity;
    int m_iConcussionEffect;

public:

    HSPRITE m_hsprCursor;
    float m_flTime; // the current client time
    float m_fOldTime; // the time at which the HUD was last redrawn
    double m_flTimeDelta; // the difference between flTime and fOldTime
    Vector m_vecOrigin;
    Vector m_vecAngles;
    int m_iKeyBits;
    int m_iHideHUDDisplay;
    int m_iFOV;
    int m_Teamplay;
    int m_iRes;
    cvar_t* m_pCvarStealMouse;
    cvar_t* m_pCvarDraw;
    Vector m_vecSkyPos; //LRC
    int m_iSkyMode; //LRC
    int m_iSkyScale; //AJH Allows parallax for the sky. 0 means no parallax, i.e infinitly large & far away.
    int m_iCameraMode; //G-Cont. clipping thirdperson camera

    int m_iFontHeight;
    int DrawHudNumber(int x, int y, int iFlags, int iNumber, int r, int g, int b);
    int DrawHudString(int x, int y, int iMaxX, char* szString, int r, int g, int b);
    int DrawHudStringReverse(int xpos, int ypos, int iMinX, char* szString, int r, int g, int b);
    int DrawHudNumberString(int xpos, int ypos, int iMinX, int iNumber, int r, int g, int b);
    int GetNumWidth(int iNumber, int iFlags);
    int viewEntityIndex; // for trigger_viewset
    int viewFlags;

    int m_iHUDColor; //LRC

private:
    // the memory for these arrays are allocated in the first call to CHud::VidInit(), when the hud.txt and associated sprites are loaded.
    // freed in ~CHud()
    HSPRITE* m_rghSprites; /*[HUD_SPRITE_COUNT]*/ // the sprites loaded from hud.txt
    wrect_t* m_rgrcRects; /*[HUD_SPRITE_COUNT]*/
    char* m_rgszSpriteNames; /*[HUD_SPRITE_COUNT][MAX_SPRITE_NAME_LENGTH]*/

    struct cvar_s* default_fov;
public:
    HSPRITE GetSprite(int index) const
    {
        return (index < 0) ? 0 : m_rghSprites[index];
    }

    wrect_t& GetSpriteRect(int index) const
    {
        return m_rgrcRects[index];
    }


    int GetSpriteIndex(const char* SpriteName); // gets a sprite index, for use in the m_rghSprites[] array

    CHudAmmo m_Ammo;
    CHudHealth m_Health;
    CHudSpectator m_Spectator;
    CHudGeiger m_Geiger;
    CHudBattery m_Battery;
    CHudTrain m_Train;
    CHudFlashlight m_Flash;
    CHudMessage m_Message;
    CHudStatusBar m_StatusBar;
    CHudDeathNotice m_DeathNotice;
    CHudSayText m_SayText;
    CHudMenu m_Menu;
    CHudAmmoSecondary m_AmmoSecondary;
    CHudTextMessage m_TextMessage;
    CHudStatusIcons m_StatusIcons;
    CHudParticle m_Particle; // (LRC) -- 30/08/02 November235: Particles to Order

    void Init();
    void VidInit();
    void Think();
    int Redraw(float flTime, int intermission);
    int UpdateClientData(client_data_t* cdata, float time);

    CHud() : m_iSpriteCount(0), m_pHudList(NULL)
    {
    }

    ~CHud(); // destructor, frees allocated memory

    // user messages
    int _cdecl MsgFunc_Damage(const char* pszName, int iSize, void* pbuf);
    int _cdecl MsgFunc_GameMode(const char* pszName, int iSize, void* pbuf);
    int _cdecl MsgFunc_Logo(const char* pszName, int iSize, void* pbuf);
    int _cdecl MsgFunc_ResetHUD(const char* pszName, int iSize, void* pbuf);
    void _cdecl MsgFunc_InitHUD(const char* pszName, int iSize, void* pbuf);
    void _cdecl MsgFunc_ViewMode(const char* pszName, int iSize, void* pbuf);
    int _cdecl MsgFunc_SetFOV(const char* pszName, int iSize, void* pbuf);
    int _cdecl MsgFunc_Concuss(const char* pszName, int iSize, void* pbuf);
    int _cdecl MsgFunc_PlayMP3(const char* pszName, int iSize, void* pbuf); //KILLAR
    int _cdecl MsgFunc_HUDColor(const char* pszName, int iSize, void* pbuf); //LRC
    void _cdecl MsgFunc_SetFog(const char* pszName, int iSize, void* pbuf); //LRC
    void _cdecl MsgFunc_KeyedDLight(const char* pszName, int iSize, void* pbuf); //LRC
    void _cdecl MsgFunc_SetSky(const char* pszName, int iSize, void* pbuf); //LRC
    int _cdecl MsgFunc_CamData(const char* pszName, int iSize, void* pbuf); //G-Cont
    void _cdecl MsgFunc_ClampView(const char* pszName, int iSize, void* pbuf); //LRC 1.8
    void _cdecl MsgFunc_Weather(const char* pszName, int iSize, void* pBuf);
    void _cdecl MsgFunc_ServerState(const char* pszName, int iSize, void* pBuf);

    // Screen information
    SCREENINFO m_scrinfo;

    int m_iWeaponBits;
    int m_fPlayerDead;
    int m_iIntermission;

    // sprite indexes
    int m_HUD_number_0;


    void AddHudElem(CHudBase* p);

    float GetSensitivity();
};

extern CHud gHUD;

HSPRITE LoadSprite(const char* pszName);
