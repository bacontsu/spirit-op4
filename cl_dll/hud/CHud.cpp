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
* Valve LLC.  All other use, distribution, or modification is prohibited
* without written permission from Valve LLC.
*
****/

#include "CHud.h"

#include <particleman.h>


#include "CGameStateManager.h"
#include "effects/CWeather.h"
#include "effects/CMP3.h"
#include "api/demo.h"
#include "demo_api.h"
#include "vgui/TeamFortressViewport.h"
#include "voice/CVoiceStatus.h"
#include "voice/CHLVoiceStatusHelper.h"

//LRC - define to help track what calls are made on changelevel, save/restore, etc
// #define ENGINE_DEBUG

static CHLVoiceStatusHelper g_VoiceStatusHelper;

extern cvar_t* sensitivity;
cvar_t* cl_lw = NULL;
cvar_t* cl_rollangle = nullptr;
cvar_t* cl_rollspeed = nullptr;
cvar_t* cl_bobtilt = nullptr;

hud_player_info_t g_PlayerInfoList[MAX_PLAYERS + 1]; // player info from the engine
extra_player_info_t g_PlayerExtraInfo[MAX_PLAYERS + 1]; // additional player info sent directly to the client dll

/// ------------
/// - Messages -
/// ------------


//DECLARE_MESSAGE(m_Logo, Logo)
int __MsgFunc_Logo(const char* pszName, int iSize, void* pbuf)
{
    return gHUD.MsgFunc_Logo(pszName, iSize, pbuf);
}

//LRC
int __MsgFunc_HUDColor(const char* pszName, int iSize, void* pbuf)
{
    return gHUD.MsgFunc_HUDColor(pszName, iSize, pbuf);
}

//LRC
int __MsgFunc_SetFog(const char* pszName, int iSize, void* pbuf)
{
    gHUD.MsgFunc_SetFog(pszName, iSize, pbuf);
    return 1;
}

//LRC
int __MsgFunc_KeyedDLight(const char* pszName, int iSize, void* pbuf)
{
    gHUD.MsgFunc_KeyedDLight(pszName, iSize, pbuf);
    return 1;
}

int __MsgFunc_Test(const char* pszName, int iSize, void* pbuf)
{
    return 1;
}

//LRC
int __MsgFunc_SetSky(const char* pszName, int iSize, void* pbuf)
{
    gHUD.MsgFunc_SetSky(pszName, iSize, pbuf);
    return 1;
}

//LRC 1.8
int __MsgFunc_ClampView(const char* pszName, int iSize, void* pbuf)
{
    gHUD.MsgFunc_ClampView(pszName, iSize, pbuf);
    return 1;
}

int __MsgFunc_Weather(const char* pszName, int iSize, void* pbuf)
{
    gHUD.MsgFunc_Weather(pszName, iSize, pbuf);
    return 1;
}

int __MsgFunc_ServerState(const char* pszName, int iSize, void* pbuf)
{
    gHUD.MsgFunc_ServerState(pszName, iSize, pbuf);
    return 1;
}

int __MsgFunc_ResetHUD(const char* pszName, int iSize, void* pbuf)
{
#ifdef ENGINE_DEBUG
    gEngfuncs.Con_Printf("## ResetHUD\n");
#endif
    return gHUD.MsgFunc_ResetHUD(pszName, iSize, pbuf);
}

int __MsgFunc_InitHUD(const char* pszName, int iSize, void* pbuf)
{
#ifdef ENGINE_DEBUG
    gEngfuncs.Con_Printf("## InitHUD\n");
#endif
    gHUD.MsgFunc_InitHUD(pszName, iSize, pbuf);
    return 1;
}

int __MsgFunc_ViewMode(const char* pszName, int iSize, void* pbuf)
{
    gHUD.MsgFunc_ViewMode(pszName, iSize, pbuf);
    return 1;
}

int __MsgFunc_SetFOV(const char* pszName, int iSize, void* pbuf)
{
    return gHUD.MsgFunc_SetFOV(pszName, iSize, pbuf);
}

int __MsgFunc_Concuss(const char* pszName, int iSize, void* pbuf)
{
    return gHUD.MsgFunc_Concuss(pszName, iSize, pbuf);
}

int __MsgFunc_GameMode(const char* pszName, int iSize, void* pbuf)
{
    return gHUD.MsgFunc_GameMode(pszName, iSize, pbuf);
}

int __MsgFunc_PlayMP3(const char* pszName, int iSize, void* pbuf)
{
    return gHUD.MsgFunc_PlayMP3(pszName, iSize, pbuf);
}


int __MsgFunc_CamData(const char* pszName, int iSize, void* pbuf)
{
    gHUD.MsgFunc_CamData(pszName, iSize, pbuf);
    return 1;
}

// TFFree Command Menu
void __CmdFunc_OpenCommandMenu()
{
    if (gViewPort)
    {
        gViewPort->ShowCommandMenu(gViewPort->m_StandardMenu);
    }
}

void __CmdFunc_CloseCommandMenu()
{
    if (gViewPort)
    {
        gViewPort->InputSignalHideCommandMenu();
    }
}

void __CmdFunc_ForceCloseCommandMenu()
{
    if (gViewPort)
    {
        gViewPort->HideCommandMenu();
    }
}

void __CmdFunc_StopMP3()
{
    gMP3.StopMP3();
}

// TFFree Command Menu Message Handlers
int __MsgFunc_ValClass(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_ValClass(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_TeamNames(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_TeamNames(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_Feign(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_Feign(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_Detpack(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_Detpack(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_VGUIMenu(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_VGUIMenu(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_MOTD(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_MOTD(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_BuildSt(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_BuildSt(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_RandomPC(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_RandomPC(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_ServerName(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_ServerName(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_ScoreInfo(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_ScoreInfo(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_TeamScore(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_TeamScore(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_TeamInfo(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_TeamInfo(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_Spectator(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_Spectator(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_SpecFade(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_SpecFade(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_ResetFade(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_ResetFade(pszName, iSize, pbuf);
    return 0;
}

int __MsgFunc_AllowSpec(const char* pszName, int iSize, void* pbuf)
{
    if (gViewPort)
        return gViewPort->MsgFunc_AllowSpec(pszName, iSize, pbuf);
    return 0;
}


/// ----------------
/// - End Messages -
/// ----------------


// This is called every time the DLL is loaded
void CHud::Init()
{
#ifdef ENGINE_DEBUG
    gEngfuncs.Con_Printf("## CHud::Init\n");
#endif
    HOOK_MESSAGE(Logo);
    HOOK_MESSAGE(ResetHUD);
    HOOK_MESSAGE(GameMode);
    HOOK_MESSAGE(InitHUD);
    HOOK_MESSAGE(ViewMode);
    HOOK_MESSAGE(SetFOV);
    HOOK_MESSAGE(Concuss);
    HOOK_MESSAGE(HUDColor); //LRC
    HOOK_MESSAGE(SetFog); //LRC
    HOOK_MESSAGE(KeyedDLight); //LRC
    //	HOOK_MESSAGE( KeyedELight ); //LRC
    HOOK_MESSAGE(Test); //LRC
    HOOK_MESSAGE(SetSky); //LRC
    HOOK_MESSAGE(CamData); //G-Cont. for new camera style 	
    HOOK_MESSAGE(ClampView); //LRC 1.8
    HOOK_MESSAGE(Weather);
    HOOK_MESSAGE(ServerState);

    //KILLAR: MP3	
    if (gMP3.Initialize())
    {
        HOOK_MESSAGE(PlayMP3);
        HOOK_COMMAND("stopaudio", StopMP3);
    }

    // TFFree CommandMenu
    HOOK_COMMAND("+commandmenu", OpenCommandMenu);
    HOOK_COMMAND("-commandmenu", CloseCommandMenu);
    HOOK_COMMAND("ForceCloseCommandMenu", ForceCloseCommandMenu);

    HOOK_MESSAGE(ValClass);
    HOOK_MESSAGE(TeamNames);
    HOOK_MESSAGE(Feign);
    HOOK_MESSAGE(Detpack);
    HOOK_MESSAGE(MOTD);
    HOOK_MESSAGE(BuildSt);
    HOOK_MESSAGE(RandomPC);
    HOOK_MESSAGE(ServerName);
    HOOK_MESSAGE(ScoreInfo);
    HOOK_MESSAGE(TeamScore);
    HOOK_MESSAGE(TeamInfo);

    HOOK_MESSAGE(Spectator);
    HOOK_MESSAGE(AllowSpec);

    HOOK_MESSAGE(SpecFade);
    HOOK_MESSAGE(ResetFade);

    // VGUI Menus
    HOOK_MESSAGE(VGUIMenu);

    CVAR_CREATE("hud_classautokill", "1", FCVAR_ARCHIVE | FCVAR_USERINFO); // controls whether or not to suicide immediately on TF class switch
    CVAR_CREATE("hud_takesshots", "0", FCVAR_ARCHIVE); // controls whether or not to automatically take screenshots at the end of a round

    viewEntityIndex = 0; // trigger_viewset stuff
    viewFlags = 0;
    m_iLogo = 0;
    m_iFOV = 0;
    m_iHUDColor = 0x00FFA000; //255,160,0 -- LRC

    CVAR_CREATE("zoom_sensitivity_ratio", "1.2", 0);
    CVAR_CREATE("cl_autowepswitch", "1", FCVAR_ARCHIVE | FCVAR_USERINFO);
    default_fov = CVAR_CREATE("default_fov", "90", FCVAR_ARCHIVE);
    m_pCvarStealMouse = CVAR_CREATE("hud_capturemouse", "1", FCVAR_ARCHIVE);
    m_pCvarDraw = CVAR_CREATE("hud_draw", "1", FCVAR_ARCHIVE);
    cl_lw = gEngfuncs.pfnGetCvarPointer("cl_lw");
    cl_rollangle = CVAR_CREATE("cl_rollangle", "2.0", FCVAR_ARCHIVE);
    cl_rollspeed = CVAR_CREATE("cl_rollspeed", "200", FCVAR_ARCHIVE);
    cl_bobtilt = CVAR_CREATE("cl_bobtilt", "0", FCVAR_ARCHIVE);

    m_pSpriteList = NULL;

    // Clear any old HUD list
    if (m_pHudList)
    {
        HUDLIST* pList;
        while (m_pHudList)
        {
            pList = m_pHudList;
            m_pHudList = m_pHudList->pNext;
            free(pList);
        }
        m_pHudList = NULL;
    }

    // In case we get messages before the first update -- time will be valid
    m_flTime = 1.0;

    m_Ammo.Init();
    m_Health.Init();
    m_SayText.Init();
    m_Spectator.Init();
    m_Geiger.Init();
    m_Train.Init();
    m_Battery.Init();
    m_Flash.Init();
    m_Message.Init();
    m_StatusBar.Init();
    m_DeathNotice.Init();
    m_AmmoSecondary.Init();
    m_TextMessage.Init();
    m_StatusIcons.Init();
    GetClientVoiceMgr()->Init(&g_VoiceStatusHelper, (vgui::Panel**)&gViewPort);
    m_Particle.Init(); // (LRC) -- 30/08/02 November235: Particles to Order

    m_Menu.Init();

    MsgFunc_ResetHUD(0, 0, NULL);
}

// CHud destructor
// cleans up memory allocated for m_rg* arrays
CHud::~CHud()
{
#ifdef ENGINE_DEBUG
    gEngfuncs.Con_Printf("## CHud::destructor\n");
#endif
    delete[] m_rghSprites;
    delete[] m_rgrcRects;
    delete[] m_rgszSpriteNames;
    gMP3.Shutdown();

    if (m_pHudList)
    {
        HUDLIST* pList;
        while (m_pHudList)
        {
            pList = m_pHudList;
            m_pHudList = m_pHudList->pNext;
            free(pList);
        }
        m_pHudList = NULL;
    }
}

int CHud::GetSpriteIndex(const char* SpriteName)
{
    // look through the loaded sprite name list for SpriteName
    for (int i = 0; i < m_iSpriteCount; i++)
    {
        if (strncmp(SpriteName, m_rgszSpriteNames + (i * MAX_SPRITE_NAME_LENGTH), MAX_SPRITE_NAME_LENGTH) == 0)
            return i;
    }

    return -1; // invalid sprite
}

void CHud::VidInit()
{
#ifdef ENGINE_DEBUG
    gEngfuncs.Con_Printf("## CHud::VidInit (hi from me)\n");
#endif
    m_scrinfo.iSize = sizeof(m_scrinfo);
    GetScreenInfo(&m_scrinfo);

    // ----------
    // Load Sprites
    // ---------
    //	m_hsprFont = LoadSprite("sprites/%d_font.spr");

    m_hsprLogo = 0;
    m_hsprCursor = 0;

    if (ScreenWidth < 640)
        m_iRes = 320;
    else
        m_iRes = 640;

    // Only load this once
    if (!m_pSpriteList)
    {
        // we need to load the hud.txt, and all sprites within
        m_pSpriteList = SPR_GetList("sprites/hud.txt", &m_iSpriteCountAllRes);

        if (m_pSpriteList)
        {
            // count the number of sprites of the appropriate res
            m_iSpriteCount = 0;
            client_sprite_t* p = m_pSpriteList;
            int j;
            for (j = 0; j < m_iSpriteCountAllRes; j++)
            {
                if (p->iRes == m_iRes)
                    m_iSpriteCount++;
                p++;
            }

            // allocated memory for sprite handle arrays
            m_rghSprites = new HSPRITE[m_iSpriteCount];
            m_rgrcRects = new wrect_t[m_iSpriteCount];
            m_rgszSpriteNames = new char[m_iSpriteCount * MAX_SPRITE_NAME_LENGTH];

            p = m_pSpriteList;
            int index = 0;
            for (j = 0; j < m_iSpriteCountAllRes; j++)
            {
                if (p->iRes == m_iRes)
                {
                    char sz[256];
                    sprintf(sz, "sprites/%s.spr", p->szSprite);
                    m_rghSprites[index] = SPR_Load(sz);
                    m_rgrcRects[index] = p->rc;
                    strncpy(&m_rgszSpriteNames[index * MAX_SPRITE_NAME_LENGTH], p->szName, MAX_SPRITE_NAME_LENGTH);

                    index++;
                }

                p++;
            }
        }
    }
    else
    {
        // we have already have loaded the sprite reference from hud.txt, but
        // we need to make sure all the sprites have been loaded (we've gone through a transition, or loaded a save game)
        client_sprite_t* p = m_pSpriteList;
        int index = 0;
        for (int j = 0; j < m_iSpriteCountAllRes; j++)
        {
            if (p->iRes == m_iRes)
            {
                char sz[256];
                sprintf(sz, "sprites/%s.spr", p->szSprite);
                m_rghSprites[index] = SPR_Load(sz);
                index++;
            }

            p++;
        }
    }

    // assumption: number_1, number_2, etc, are all listed and loaded sequentially
    m_HUD_number_0 = GetSpriteIndex("number_0");

    m_iFontHeight = m_rgrcRects[m_HUD_number_0].bottom - m_rgrcRects[m_HUD_number_0].top;

    m_Ammo.VidInit();
    m_Health.VidInit();
    m_Spectator.VidInit();
    m_Geiger.VidInit();
    m_Train.VidInit();
    m_Battery.VidInit();
    m_Flash.VidInit();
    m_Message.VidInit();
    m_StatusBar.VidInit();
    m_DeathNotice.VidInit();
    m_SayText.VidInit();
    m_Menu.VidInit();
    m_AmmoSecondary.VidInit();
    m_TextMessage.VidInit();
    m_StatusIcons.VidInit();
    GetClientVoiceMgr()->VidInit();
    m_Particle.VidInit(); // (LRC) -- 30/08/02 November235: Particles to Order

    g_Weather.Initialise();
}

int CHud::MsgFunc_Logo(const char* pszName, int iSize, void* pbuf)
{
    BEGIN_READ(pbuf, iSize);

    // update Train data
    m_iLogo = READ_BYTE();

    return 1;
}

//LRC
int CHud::MsgFunc_HUDColor(const char* pszName, int iSize, void* pbuf)
{
    BEGIN_READ(pbuf, iSize);

    m_iHUDColor = READ_LONG();

    return 1;
}

float g_lastFOV = 0.0;

/*
=====================
HUD_GetFOV

Returns last FOV
=====================
*/
float HUD_GetFOV()
{
    if (gEngfuncs.pDemoAPI->IsRecording())
    {
        // Write it
        int i = 0;
        unsigned char buf[100];

        // Active
        *(float*)&buf[i] = g_lastFOV;
        i += sizeof(float);

        Demo_WriteBuffer(TYPE_ZOOM, i, buf);
    }

    if (gEngfuncs.pDemoAPI->IsPlayingback())
    {
        g_lastFOV = g_demozoom;
    }
    return g_lastFOV;
}


int CHud::MsgFunc_SetFOV(const char* pszName, int iSize, void* pbuf)
{
    BEGIN_READ(pbuf, iSize);

    int newfov = READ_BYTE();
    int def_fov = CVAR_GET_FLOAT("default_fov");

    //Weapon prediction already takes care of changing the fog. ( g_lastFOV ).
    //But it doesn't restore correctly so this still needs to be used
    /*
    if ( cl_lw && cl_lw->value )
        return 1;
        */

    g_lastFOV = newfov;

    if (newfov == 0)
    {
        m_iFOV = def_fov;
    }
    else
    {
        m_iFOV = newfov;
    }

    // the clients fov is actually set in the client data update section of the hud

    // Set a new sensitivity
    if (m_iFOV == def_fov)
    {
        // reset to saved sensitivity
        m_flMouseSensitivity = 0;
    }
    else
    {
        // set a new sensitivity that is proportional to the change from the FOV default
        m_flMouseSensitivity = sensitivity->value * ((float)newfov / (float)def_fov) * CVAR_GET_FLOAT("zoom_sensitivity_ratio");
    }

    return 1;
}


void CHud::AddHudElem(CHudBase* phudelem)
{
    HUDLIST *pdl, *ptemp;

    //phudelem->Think();

    if (!phudelem)
        return;

    pdl = (HUDLIST*)malloc(sizeof(HUDLIST));
    if (!pdl)
        return;

    memset(pdl, 0, sizeof(HUDLIST));
    pdl->p = phudelem;

    if (!m_pHudList)
    {
        m_pHudList = pdl;
        return;
    }

    ptemp = m_pHudList;

    while (ptemp->pNext)
        ptemp = ptemp->pNext;

    ptemp->pNext = pdl;
}

float CHud::GetSensitivity()
{
    return m_flMouseSensitivity;
}


//LRC - the fogging fog
FogSettings g_fog;
FogSettings g_fogPreFade;
FogSettings g_fogPostFade;
float g_fFogFadeDuration;
float g_fFogFadeFraction;

#define MAX_CLIENTS 32

extern BEAM* pBeam;
extern BEAM* pBeam2;
extern TEMPENTITY* pFlare; // Vit_amiN

extern float g_clampMinYaw, g_clampMaxYaw, g_clampMinPitch, g_clampMaxPitch;
extern float g_clampTurnSpeed;

/// USER-DEFINED SERVER MESSAGE HANDLERS

int CHud::MsgFunc_ResetHUD(const char* pszName, int iSize, void* pbuf)
{
    ASSERT(iSize == 0);

    // clear all hud data
    HUDLIST* pList = m_pHudList;

    while (pList)
    {
        if (pList->p)
            pList->p->Reset();
        pList = pList->pNext;
    }

    // reset sensitivity
    m_flMouseSensitivity = 0;

    // reset concussion effect
    m_iConcussionEffect = 0;

    /*	//LRC - reset fog
        g_fStartDist = 0;
        g_fEndDist = 0;
    */
    return 1;
}

//void CAM_ToFirstPerson();

void CHud::MsgFunc_ViewMode(const char* pszName, int iSize, void* pbuf)
{
    //CAM_ToFirstPerson();
}

void CHud::MsgFunc_InitHUD(const char* pszName, int iSize, void* pbuf)
{
    //	gEngfuncs.Con_Printf("MSG:InitHUD");
    //LRC - clear the fog
    g_fog.startDist = -1;
    g_fog.endDist = -1;
    g_fog.fogColor[0] = -1;
    g_fog.fogColor[1] = -1;
    g_fog.fogColor[2] = -1;
    //LRC 1.8 - clear view clamps
    g_clampMinPitch = -90;
    g_clampMaxPitch = 90;
    g_clampMinYaw = 0;
    g_clampMaxYaw = 360;
    g_clampTurnSpeed = 1E6;

    m_iSkyMode = SKY_OFF; //LRC

    // prepare all hud data
    HUDLIST* pList = m_pHudList;

    while (pList)
    {
        if (pList->p)
            pList->p->InitHUDData();
        pList = pList->pNext;
    }

    if (g_pParticleMan)
        g_pParticleMan->ResetParticles();

    //Probably not a good place to put this.
    pBeam = pBeam2 = NULL;
    pFlare = NULL;

    g_gameStateManager.InitHud();
}

//LRC
void CHud::MsgFunc_SetFog(const char* pszName, int iSize, void* pbuf)
{
    //	gEngfuncs.Con_Printf("MSG:SetFog");
    BEGIN_READ(pbuf, iSize);

    for (int i = 0; i < 3; i++)
    {
        g_fogPostFade.fogColor[i] = READ_BYTE();

        if (g_fog.fogColor[i] >= 0)
            g_fogPreFade.fogColor[i] = g_fog.fogColor[i];
        else
            g_fogPreFade.fogColor[i] = g_fogPostFade.fogColor[i];
    }

    g_fFogFadeDuration = READ_SHORT();

    g_fogPostFade.startDist = READ_SHORT();
    if (g_fog.startDist >= 0)
        g_fogPreFade.startDist = g_fog.startDist;
    else
        g_fogPreFade.startDist = g_fogPostFade.startDist;

    g_fogPostFade.endDist = READ_SHORT();
    if (g_fog.endDist >= 0)
        g_fogPreFade.endDist = g_fog.endDist;
    else
        g_fogPreFade.endDist = g_fogPostFade.endDist;

    if (g_fFogFadeDuration < 0)
    {
        g_fFogFadeDuration *= -1;
        g_fogPostFade.startDist = FOG_LIMIT;
        g_fogPostFade.endDist = FOG_LIMIT;
    }
    else if (g_fFogFadeDuration == 0)
    {
        g_fog.endDist = g_fogPostFade.endDist;
        for (int i = 0; i < 3; i++)
        {
            g_fogPreFade.fogColor[i] = g_fog.fogColor[i];
        }
    }
    g_fFogFadeFraction = 0;
}

//LRC
void CHud::MsgFunc_KeyedDLight(const char* pszName, int iSize, void* pbuf)
{
    //	gEngfuncs.Con_Printf("MSG:KeyedDLight");
    BEGIN_READ(pbuf, iSize);

    // as-yet unused:
    //	float	decay;				// drop this each second
    //	float	minlight;			// don't add when contributing less
    //	qboolean	dark;			// subtracts light instead of adding (doesn't seem to do anything?)

    int iKey = READ_BYTE();
    dlight_t* dl = gEngfuncs.pEfxAPI->CL_AllocDlight(iKey);

    int bActive = READ_BYTE();
    if (!bActive)
    {
        // die instantly
        dl->die = gEngfuncs.GetClientTime();
    }
    else
    {
        // never die
        dl->die = gEngfuncs.GetClientTime() + 1E6;

        dl->origin[0] = READ_COORD();
        dl->origin[1] = READ_COORD();
        dl->origin[2] = READ_COORD();
        dl->radius = READ_BYTE();
        dl->color.r = READ_BYTE();
        dl->color.g = READ_BYTE();
        dl->color.b = READ_BYTE();
    }
}

//LRC
void CHud::MsgFunc_SetSky(const char* pszName, int iSize, void* pbuf)
{
    //	gEngfuncs.Con_Printf("MSG:SetSky");
    BEGIN_READ(pbuf, iSize);

    m_iSkyMode = READ_BYTE();
    m_vecSkyPos.x = READ_COORD();
    m_vecSkyPos.y = READ_COORD();
    m_vecSkyPos.z = READ_COORD();
    m_iSkyScale = READ_BYTE();
}

//LRC 1.8
void CHud::MsgFunc_ClampView(const char* pszName, int iSize, void* pbuf)
{
    BEGIN_READ(pbuf, iSize);

    g_clampMinYaw = READ_SHORT();
    g_clampMaxYaw = READ_SHORT();
    g_clampMinPitch = READ_BYTE() - 128;
    g_clampMaxPitch = READ_BYTE() - 128;
    *(long*)&g_clampTurnSpeed = READ_LONG();
}

int CHud::MsgFunc_GameMode(const char* pszName, int iSize, void* pbuf)
{
    BEGIN_READ(pbuf, iSize);
    m_Teamplay = READ_BYTE();

    return 1;
}

int CHud::MsgFunc_Damage(const char* pszName, int iSize, void* pbuf)
{
    int armor, blood;
    Vector from;
    int i;
    float count;

    BEGIN_READ(pbuf, iSize);
    armor = READ_BYTE();
    blood = READ_BYTE();

    for (i = 0; i < 3; i++)
        from[i] = READ_COORD();

    count = (blood * 0.5) + (armor * 0.5);

    if (count < 10)
        count = 10;

    // TODO: kick viewangles,  show damage visually

    return 1;
}

int CHud::MsgFunc_Concuss(const char* pszName, int iSize, void* pbuf)
{
    BEGIN_READ(pbuf, iSize);
    m_iConcussionEffect = READ_BYTE();
    if (m_iConcussionEffect)
        this->m_StatusIcons.EnableIcon("dmg_concuss", 255, 160, 0);
    else
        this->m_StatusIcons.DisableIcon("dmg_concuss");
    return 1;
}

int CHud::MsgFunc_PlayMP3(const char* pszName, int iSize, void* pbuf) //AJH -Killar MP3
{
    BEGIN_READ(pbuf, iSize);

    gMP3.PlayMP3(READ_STRING());

    return 1;
}

// trigger_viewset message
int CHud::MsgFunc_CamData(const char* pszName, int iSize, void* pbuf)
{
    BEGIN_READ(pbuf, iSize);
    gHUD.viewEntityIndex = READ_SHORT();
    gHUD.viewFlags = READ_SHORT();
    //	gEngfuncs.Con_Printf( "Got view entity with index %i\n", gHUD.viewEntityIndex );
    return 1;
}

void CHud::MsgFunc_Weather(const char* pszName, int iSize, void* pBuf)
{
    BEGIN_READ(pBuf, iSize);
    {
        auto messageType = READ_BYTE();
        if (messageType == 1)
        {
            // Precipitation
            auto type = static_cast<PrecipitationType>(READ_BYTE());
            auto numParticles = READ_SHORT();
            auto sprayDensity = READ_BYTE();
            g_Weather.SetPrecipitation(type, numParticles, sprayDensity);
        }
        else if (messageType == 2)
        {
            // Wind
            auto yaw = READ_COORD();
            auto speed = READ_COORD();
            auto yawVariance = READ_COORD();
            auto speedVariance = READ_COORD();
            auto changeFrequency = READ_COORD();
            auto changeSpeed = READ_COORD();
            g_Weather.SetWind(yaw, speed, yawVariance, speedVariance, changeFrequency, changeSpeed);
        }
    }
}

void CHud::MsgFunc_ServerState(const char* pszName, int iSize, void* pBuf)
{
    BEGIN_READ(pBuf, iSize);
    const auto state = READ_BYTE();

    g_gameStateManager.UpdateServerState(state);
}


#define MAX_LOGO_FRAMES 56

int grgLogoFrame[MAX_LOGO_FRAMES] =
{
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 13, 13, 13, 13, 13, 12, 11, 10, 9, 8, 14, 15,
    16, 17, 18, 19, 20, 20, 20, 20, 20, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    29, 29, 29, 29, 29, 28, 27, 26, 25, 24, 30, 31
};


extern int g_iVisibleMouse;

float HUD_GetFOV();

extern cvar_t* sensitivity;

// Think
void CHud::Think()
{
    m_scrinfo.iSize = sizeof(m_scrinfo);
    GetScreenInfo(&m_scrinfo);

    int newfov;
    HUDLIST* pList = m_pHudList;

    while (pList)
    {
        if (pList->p->m_iFlags & HUD_ACTIVE)
            pList->p->Think();
        pList = pList->pNext;
    }

    newfov = HUD_GetFOV();
    if (newfov == 0)
    {
        m_iFOV = default_fov->value;
    }
    else
    {
        m_iFOV = newfov;
    }

    // the clients fov is actually set in the client data update section of the hud

    // Set a new sensitivity
    if (m_iFOV == default_fov->value)
    {
        // reset to saved sensitivity
        m_flMouseSensitivity = 0;
    }
    else
    {
        // set a new sensitivity that is proportional to the change from the FOV default
        m_flMouseSensitivity = sensitivity->value * ((float)newfov / (float)default_fov->value) * CVAR_GET_FLOAT("zoom_sensitivity_ratio");
    }

    // think about default fov
    if (m_iFOV == 0)
    {
        // only let players adjust up in fov,  and only if they are not overriden by something else
        m_iFOV = V_max(default_fov->value, 90);
    }

    if (gEngfuncs.IsSpectateOnly())
    {
        m_iFOV = gHUD.m_Spectator.GetFOV(); // default_fov->value;
    }
}

// Redraw
// step through the local data,  placing the appropriate graphics & text as appropriate
// returns 1 if they've changed, 0 otherwise
int CHud::Redraw(float flTime, int intermission)
{
    m_fOldTime = m_flTime; // save time of previous redraw
    m_flTime = flTime;
    m_flTimeDelta = (double)m_flTime - m_fOldTime;
    static float m_flShotTime = 0;

    //LRC - handle fog fading effects. (is this the right place for it?)
    if (g_fFogFadeDuration)
    {
        // Nicer might be to use some kind of logarithmic fade-in?
        double fFraction = m_flTimeDelta / g_fFogFadeDuration;
        if (fFraction > 0)
        {
            g_fFogFadeFraction += fFraction;

            //		gEngfuncs.Con_Printf("FogFading: %f - %f, frac %f, time %f, final %d\n", g_fStartDist, g_fEndDist, fFraction, flTime, g_iFinalEndDist);

            if (g_fFogFadeFraction >= 1.0f)
            {
                // fading complete
                g_fFogFadeFraction = 1.0f;
                g_fFogFadeDuration = 0.0f;
            }

            // set the new fog values
            g_fog.endDist = UTIL_Lerp(g_fFogFadeFraction, g_fogPreFade.endDist, g_fogPostFade.endDist);
            g_fog.startDist = UTIL_Lerp(g_fFogFadeFraction, g_fogPreFade.startDist, g_fogPostFade.startDist);
            g_fog.fogColor[0] = UTIL_Lerp(g_fFogFadeFraction, g_fogPreFade.fogColor[0], g_fogPostFade.fogColor[0]);
            g_fog.fogColor[1] = UTIL_Lerp(g_fFogFadeFraction, g_fogPreFade.fogColor[1], g_fogPostFade.fogColor[1]);
            g_fog.fogColor[2] = UTIL_Lerp(g_fFogFadeFraction, g_fogPreFade.fogColor[2], g_fogPostFade.fogColor[2]);
        }
    }

    // Clock was reset, reset delta
    if (m_flTimeDelta < 0)
        m_flTimeDelta = 0;

    // Bring up the scoreboard during intermission
    if (gViewPort)
    {
        if (m_iIntermission && !intermission)
        {
            // Have to do this here so the scoreboard goes away
            m_iIntermission = intermission;
            gViewPort->HideCommandMenu();
            gViewPort->HideScoreBoard();
            gViewPort->UpdateSpectatorPanel();
        }
        else if (!m_iIntermission && intermission)
        {
            m_iIntermission = intermission;
            gViewPort->HideCommandMenu();
            gViewPort->HideVGUIMenu();
            gViewPort->ShowScoreBoard();
            gViewPort->UpdateSpectatorPanel();

            // Take a screenshot if the client's got the cvar set
            if (CVAR_GET_FLOAT("hud_takesshots") != 0)
                m_flShotTime = flTime + 1.0; // Take a screenshot in a second
        }
    }

    if (m_flShotTime && m_flShotTime < flTime)
    {
        gEngfuncs.pfnClientCmd("snapshot\n");
        m_flShotTime = 0;
    }

    m_iIntermission = intermission;

    // if no redrawing is necessary
    // return 0;

    // trigger_viewset stuff
    if ((viewFlags & 1) && !(viewFlags & 2)) // custom view active, and flag "draw hud" isnt set
        return 1;

    // draw all registered HUD elements
    if (m_pCvarDraw->value)
    {
        HUDLIST* pList = m_pHudList;

        while (pList)
        {
            if (!intermission)
            {
                if ((pList->p->m_iFlags & HUD_ACTIVE) && !(m_iHideHUDDisplay & HIDEHUD_ALL))
                    pList->p->Draw(flTime);
            }
            else
            {
                // it's an intermission,  so only draw hud elements that are set to draw during intermissions
                if (pList->p->m_iFlags & HUD_INTERMISSION)
                    pList->p->Draw(flTime);
            }

            pList = pList->pNext;
        }
    }

    // are we in demo mode? do we need to draw the logo in the top corner?
    if (m_iLogo)
    {
        int x, y, i;

        if (m_hsprLogo == 0)
            m_hsprLogo = LoadSprite("sprites/%d_logo.spr");

        SPR_Set(m_hsprLogo, 250, 250, 250);

        x = SPR_Width(m_hsprLogo, 0);
        x = ScreenWidth - x;
        y = SPR_Height(m_hsprLogo, 0) / 2;

        // Draw the logo at 20 fps
        int iFrame = (int)(flTime * 20) % MAX_LOGO_FRAMES;
        i = grgLogoFrame[iFrame] - 1;

        SPR_DrawAdditive(i, x, y, NULL);
    }

    /*
    if ( g_iVisibleMouse )
    {
        void IN_GetMousePos( int *mx, int *my );
        int mx, my;

        IN_GetMousePos( &mx, &my );

        if (m_hsprCursor == 0)
        {
            char sz[256];
            sprintf( sz, "sprites/cursor.spr" );
            m_hsprCursor = SPR_Load( sz );
        }

        SPR_Set(m_hsprCursor, 250, 250, 250 );

        // Draw the logo at 20 fps
        SPR_DrawAdditive( 0, mx, my, NULL );
    }
    */

    return 1;
}

void ScaleColors(int& r, int& g, int& b, int a)
{
    float x = (float)a / 255;
    r = (int)(r * x);
    g = (int)(g * x);
    b = (int)(b * x);
}

int CHud::DrawHudString(int xpos, int ypos, int iMaxX, char* szIt, int r, int g, int b)
{
    return xpos + gEngfuncs.pfnDrawString(xpos, ypos, szIt, r, g, b);
}

int CHud::DrawHudNumberString(int xpos, int ypos, int iMinX, int iNumber, int r, int g, int b)
{
    char szString[32];
    sprintf(szString, "%d", iNumber);
    return DrawHudStringReverse(xpos, ypos, iMinX, szString, r, g, b);
}

// draws a string from right to left (right-aligned)
int CHud::DrawHudStringReverse(int xpos, int ypos, int iMinX, char* szString, int r, int g, int b)
{
    return xpos - gEngfuncs.pfnDrawStringReverse(xpos, ypos, szString, r, g, b);
}

int CHud::DrawHudNumber(int x, int y, int iFlags, int iNumber, int r, int g, int b)
{
    int iWidth = GetSpriteRect(m_HUD_number_0).right - GetSpriteRect(m_HUD_number_0).left;
    int k;

    if (iNumber > 0)
    {
        // SPR_Draw 100's
        if (iNumber >= 100)
        {
            k = iNumber / 100;
            SPR_Set(GetSprite(m_HUD_number_0 + k), r, g, b);
            SPR_DrawAdditive(0, x, y, &GetSpriteRect(m_HUD_number_0 + k));
            x += iWidth;
        }
        else if (iFlags & (DHN_3DIGITS))
        {
            //SPR_DrawAdditive( 0, x, y, &rc );
            x += iWidth;
        }

        // SPR_Draw 10's
        if (iNumber >= 10)
        {
            k = (iNumber % 100) / 10;
            SPR_Set(GetSprite(m_HUD_number_0 + k), r, g, b);
            SPR_DrawAdditive(0, x, y, &GetSpriteRect(m_HUD_number_0 + k));
            x += iWidth;
        }
        else if (iFlags & (DHN_3DIGITS | DHN_2DIGITS))
        {
            //SPR_DrawAdditive( 0, x, y, &rc );
            x += iWidth;
        }

        // SPR_Draw ones
        k = iNumber % 10;
        SPR_Set(GetSprite(m_HUD_number_0 + k), r, g, b);
        SPR_DrawAdditive(0, x, y, &GetSpriteRect(m_HUD_number_0 + k));
        x += iWidth;
    }
    else if (iFlags & DHN_DRAWZERO)
    {
        SPR_Set(GetSprite(m_HUD_number_0), r, g, b);

        // SPR_Draw 100's
        if (iFlags & (DHN_3DIGITS))
        {
            //SPR_DrawAdditive( 0, x, y, &rc );
            x += iWidth;
        }

        if (iFlags & (DHN_3DIGITS | DHN_2DIGITS))
        {
            //SPR_DrawAdditive( 0, x, y, &rc );
            x += iWidth;
        }

        // SPR_Draw ones

        SPR_DrawAdditive(0, x, y, &GetSpriteRect(m_HUD_number_0));
        x += iWidth;
    }

    return x;
}


int CHud::GetNumWidth(int iNumber, int iFlags)
{
    if (iFlags & (DHN_3DIGITS))
        return 3;

    if (iFlags & (DHN_2DIGITS))
        return 2;

    if (iNumber <= 0)
    {
        if (iFlags & (DHN_DRAWZERO))
            return 1;
        else
            return 0;
    }

    if (iNumber < 10)
        return 1;

    if (iNumber < 100)
        return 2;

    return 3;
}


int CL_ButtonBits(int);
void CL_ResetButtonBits(int bits);

extern float v_idlescale;
float in_fov;
extern void HUD_SetCmdBits(int bits);

int CHud::UpdateClientData(client_data_t* cdata, float time)
{
    memcpy(m_vecOrigin, cdata->origin, sizeof(vec3_t));
    memcpy(m_vecAngles, cdata->viewangles, sizeof(vec3_t));

    m_iKeyBits = CL_ButtonBits(0);
    m_iWeaponBits = cdata->iWeaponBits;

    in_fov = cdata->fov;

    Think();

    cdata->fov = m_iFOV;

    v_idlescale = m_iConcussionEffect;

    CL_ResetButtonBits(m_iKeyBits);

    // return 1 if in anything in the client_data struct has been changed, 0 otherwise
    return 1;
}


HSPRITE LoadSprite(const char* pszName)
{
    int i;
    char sz[256];

    if (ScreenWidth < 640)
        i = 320;
    else
        i = 640;

    sprintf(sz, pszName, i);

    return SPR_Load(sz);
}
