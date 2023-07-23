/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// hud.cpp
//
// implementation of CHud class
//

//LRC - define to help track what calls are made on changelevel, save/restore, etc
#define ENGINE_DEBUG

#include "hud.h"
#include "cl_util.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h"
#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "mp3.h"
#include "demo.h"
#include "demo_api.h"
#include "vgui_ScorePanel.h"
#include "rain.h"
#include "fog.h"

hud_player_info_t g_PlayerInfoList[MAX_PLAYERS_HUD + 1];	// player info from the engine
extra_player_info_t g_PlayerExtraInfo[MAX_PLAYERS_HUD + 1]; // additional player info sent directly to the client dll

cvar_t* cvar_ladderbob;
int giR, giG, giB;

extern int giOldWeapons;

class CHLVoiceStatusHelper : public IVoiceStatusHelper
{
public:
	void GetPlayerTextColor(int entindex, int color[3]) override
	{
		color[0] = color[1] = color[2] = 255;

		if (entindex >= 0 && entindex < sizeof(g_PlayerExtraInfo) / sizeof(g_PlayerExtraInfo[0]))
		{
			int iTeam = g_PlayerExtraInfo[entindex].teamnumber;

			if (iTeam < 0)
			{
				iTeam = 0;
			}

			iTeam = iTeam % iNumberOfTeamColors;

			color[0] = iTeamColors[iTeam][0];
			color[1] = iTeamColors[iTeam][1];
			color[2] = iTeamColors[iTeam][2];
		}
	}

	void UpdateCursorState() override
	{
		gViewPort->UpdateCursorState();
	}

	int GetAckIconHeight() override
	{
		return ScreenHeight - gHUD.m_iFontHeight * 3 - 6;
	}

	bool CanShowSpeakerLabels() override
	{
		if (gViewPort && gViewPort->m_pScoreBoard)
			return !gViewPort->m_pScoreBoard->isVisible();
		else
			return false;
	}
};
static CHLVoiceStatusHelper g_VoiceStatusHelper;


extern client_sprite_t* GetSpriteList(client_sprite_t* pList, const char* psz, int iRes, int iCount);

extern cvar_t* sensitivity;
cvar_t* cl_lw = NULL;
cvar_t* cl_rollangle = nullptr;
cvar_t* cl_rollspeed = nullptr;
cvar_t* cl_bobtilt = nullptr;

void ShutdownInput();

int __MsgFunc_HudColor(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	giR = READ_BYTE();
	giG = READ_BYTE();
	giB = READ_BYTE();
	return 1;
}

int __MsgFunc_OldWeapon(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	giOldWeapons = READ_BYTE();
	return 1;
}

//DECLARE_MESSAGE(m_Logo, Logo)
int __MsgFunc_Logo(const char* pszName, int iSize, void* pbuf)
{
	return static_cast<int>(gHUD.MsgFunc_Logo(pszName, iSize, pbuf));
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

//LRC
int __MsgFunc_AddShine(const char* pszName, int iSize, void* pbuf)
{
	gHUD.MsgFunc_AddShine(pszName, iSize, pbuf);
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

// G-Cont. rain message
int __MsgFunc_RainData(const char* pszName, int iSize, void* pbuf)
{
	return gHUD.MsgFunc_RainData(pszName, iSize, pbuf);
}

//LRC 1.8
int __MsgFunc_ClampView(const char* pszName, int iSize, void* pbuf)
{
	gHUD.MsgFunc_ClampView(pszName, iSize, pbuf);
	return 1;
}

int __MsgFunc_ResetHUD(const char* pszName, int iSize, void* pbuf)
{
#ifdef ENGINE_DEBUG
	CONPRINT("## ResetHUD\n");
#endif
	return static_cast<int>(gHUD.MsgFunc_ResetHUD(pszName, iSize, pbuf));
}

int __MsgFunc_InitHUD(const char* pszName, int iSize, void* pbuf)
{
#ifdef ENGINE_DEBUG
	CONPRINT("## InitHUD\n");
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
	return static_cast<int>(gHUD.MsgFunc_SetFOV(pszName, iSize, pbuf));
}

int __MsgFunc_Concuss(const char* pszName, int iSize, void* pbuf)
{
	return static_cast<int>(gHUD.MsgFunc_Concuss(pszName, iSize, pbuf));
}

int __MsgFunc_Weapons(const char* pszName, int iSize, void* pbuf)
{
	return static_cast<int>(gHUD.MsgFunc_Weapons(pszName, iSize, pbuf));
}

int __MsgFunc_GameMode(const char* pszName, int iSize, void* pbuf)
{
	return static_cast<int>(gHUD.MsgFunc_GameMode(pszName, iSize, pbuf));
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

int __MsgFunc_Inventory(const char* pszName, int iSize, void* pbuf)
{
	gHUD.MsgFunc_Inventory(pszName, iSize, pbuf);
	return 1;
}

int __MsgFunc_UseEnt(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	int index = READ_BYTE();
	gHUD.m_bDrawUsable = index > 0;
	return 1;
}

int __MsgFunc_Smoke(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	gHUD.m_iSmokeFrame = 0;
	return 1;
}

int __MsgFunc_PlayerData(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);
	gHUD.m_bSliding = READ_BYTE();
	gHUD.m_flLeanAngle = READ_FLOAT();
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

// TFC "special" command
void __CmdFunc_InputPlayerSpecial()
{
	if (gViewPort)
	{
		gViewPort->InputPlayerSpecial();
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
		return static_cast<int>(gViewPort->MsgFunc_ValClass(pszName, iSize, pbuf));
	return 0;
}

int __MsgFunc_TeamNames(const char* pszName, int iSize, void* pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_TeamNames(pszName, iSize, pbuf));
	return 0;
}

int __MsgFunc_Feign(const char* pszName, int iSize, void* pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_Feign(pszName, iSize, pbuf));
	return 0;
}

int __MsgFunc_Detpack(const char* pszName, int iSize, void* pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_Detpack(pszName, iSize, pbuf));
	return 0;
}

int __MsgFunc_VGUIMenu(const char* pszName, int iSize, void* pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_VGUIMenu(pszName, iSize, pbuf));
	return 0;
}

int __MsgFunc_MOTD(const char* pszName, int iSize, void* pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_MOTD(pszName, iSize, pbuf));
	return 0;
}

int __MsgFunc_BuildSt(const char* pszName, int iSize, void* pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_BuildSt(pszName, iSize, pbuf));
	return 0;
}

int __MsgFunc_RandomPC(const char* pszName, int iSize, void* pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_RandomPC(pszName, iSize, pbuf));
	return 0;
}

int __MsgFunc_ServerName(const char* pszName, int iSize, void* pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_ServerName(pszName, iSize, pbuf));
	return 0;
}

/*
int __MsgFunc_ScoreInfo(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_ScoreInfo( pszName, iSize, pbuf ));
	return 0;
}

int __MsgFunc_TeamScore(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_TeamScore( pszName, iSize, pbuf ));
	return 0;
}

int __MsgFunc_TeamInfo(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_TeamInfo( pszName, iSize, pbuf ));
	return 0;
}
*/

int __MsgFunc_Spectator(const char* pszName, int iSize, void* pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_Spectator(pszName, iSize, pbuf));
	return 0;
}

int __MsgFunc_SpecFade(const char* pszName, int iSize, void* pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_SpecFade(pszName, iSize, pbuf));
	return 0;
}

int __MsgFunc_ResetFade(const char* pszName, int iSize, void* pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_ResetFade(pszName, iSize, pbuf));
	return 0;
}

int __MsgFunc_AllowSpec(const char* pszName, int iSize, void* pbuf)
{
	if (gViewPort)
		return static_cast<int>(gViewPort->MsgFunc_AllowSpec(pszName, iSize, pbuf));
	return 0;
}


extern void EV_HLDM_Particles(vec_t Pos_X, vec_t Pos_Y, vec_t Pos_Z, float PosNorm_X, float PosNorm_Y, float PosNorm_Z, int DoPuff, int Material);
int __MsgFunc_Impact(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);

	int MatType = READ_SHORT();
	int DoPuffSpr = READ_BYTE();

	vec_t Pos_X, Pos_Y, Pos_Z;
	float PosNorm_X, PosNorm_Y, PosNorm_Z;

	Pos_X = READ_COORD();
	Pos_Y = READ_COORD();
	Pos_Z = READ_COORD();
	PosNorm_X = READ_COORD();
	PosNorm_Y = READ_COORD();
	PosNorm_Z = READ_COORD();

	EV_HLDM_Particles(Pos_X, Pos_Y, Pos_Z, PosNorm_X, PosNorm_Y, PosNorm_Z, DoPuffSpr, MatType);

	return 1;
}

// This is called every time the DLL is loaded
void CHud::Init()
{
#ifdef ENGINE_DEBUG
	CONPRINT("## CHud::Init\n");
#endif
	HOOK_MESSAGE(Logo);
	HOOK_MESSAGE(ResetHUD);
	HOOK_MESSAGE(GameMode);
	HOOK_MESSAGE(InitHUD);
	HOOK_MESSAGE(ViewMode);
	HOOK_MESSAGE(SetFOV);
	HOOK_MESSAGE(Concuss);
	HOOK_MESSAGE(HudColor);
	HOOK_MESSAGE(OldWeapon);
	HOOK_MESSAGE(Weapons);
	HOOK_MESSAGE(HUDColor);	   //LRC
	HOOK_MESSAGE(SetFog);	   //LRC
	HOOK_MESSAGE(KeyedDLight); //LRC
							   //	HOOK_MESSAGE( KeyedELight ); //LRC
	HOOK_MESSAGE(AddShine);	   //LRC
	HOOK_MESSAGE(Test);		   //LRC
	HOOK_MESSAGE(SetSky);	   //LRC
	HOOK_MESSAGE(CamData);	   //G-Cont. for new camera style
	HOOK_MESSAGE(RainData);	   //G-Cont. for rain control
	HOOK_MESSAGE(Inventory);   //AJH Inventory system
	HOOK_MESSAGE(ClampView);   //LRC 1.8

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
	HOOK_COMMAND("special", InputPlayerSpecial);

	HOOK_MESSAGE(ValClass);
	HOOK_MESSAGE(TeamNames);
	HOOK_MESSAGE(Feign);
	HOOK_MESSAGE(Detpack);
	HOOK_MESSAGE(MOTD);
	HOOK_MESSAGE(BuildSt);
	HOOK_MESSAGE(RandomPC);
	HOOK_MESSAGE(ServerName);
	/*
	HOOK_MESSAGE( ScoreInfo );
	HOOK_MESSAGE( TeamScore );
	HOOK_MESSAGE( TeamInfo );
	*/

	HOOK_MESSAGE(Spectator);
	HOOK_MESSAGE(AllowSpec);

	HOOK_MESSAGE(SpecFade);
	HOOK_MESSAGE(ResetFade);

	// VGUI Menus
	HOOK_MESSAGE(VGUIMenu);

	HOOK_MESSAGE(Impact);

	// bacontsu
	HOOK_MESSAGE(UseEnt);
	HOOK_MESSAGE(Smoke);
	HOOK_MESSAGE(PlayerData);

	CVAR_CREATE("hud_classautokill", "1", FCVAR_ARCHIVE | FCVAR_USERINFO); // controls whether or not to suicide immediately on TF class switch
	CVAR_CREATE("hud_takesshots", "0", FCVAR_ARCHIVE);					   // controls whether or not to automatically take screenshots at the end of a round

	// Custom Cvars
	CVAR_CREATE("cl_viewmodel_bob", "0", FCVAR_ARCHIVE);
	CVAR_CREATE("cl_viewmodel_sway", "0", FCVAR_ARCHIVE);

	cvar_ladderbob = CVAR_CREATE("cl_ladderbob", "1", FCVAR_ARCHIVE);

	CVAR_CREATE("sv_noclipspeed", "1", FCVAR_ARCHIVE);

	//start glow effect --FragBait0
	CVAR_CREATE("r_glow", "0", FCVAR_ARCHIVE);
	//CVAR_CREATE("r_glowmode", "0", FCVAR_ARCHIVE ); //AJH this is now redundant
	CVAR_CREATE("r_glowstrength", "1", FCVAR_ARCHIVE);
	CVAR_CREATE("r_glowblur", "4", FCVAR_ARCHIVE);
	CVAR_CREATE("r_glowdark", "2", FCVAR_ARCHIVE);
	//end glow effect

	clientmaxspeed = CVAR_CREATE("cl_maxspeed", "360", FCVAR_ARCHIVE);
	cl_movement_toggle_mode = CVAR_CREATE("cl_movement_mode", "1", FCVAR_ARCHIVE);
	cl_enable_viewroll = CVAR_CREATE("cl_enable_viewroll", "1", FCVAR_ARCHIVE);

	viewEntityIndex = 0; // trigger_viewset stuff
	viewFlags = 0;
	m_iLogo = 0;
	m_iFOV = 0;

	numMirrors = 0;
	m_iHUDColor = 0x00FFA000; //255,160,0 -- LRC

	setNightVisionState(false);


	CVAR_CREATE("zoom_sensitivity_ratio", "1.2", 0);
	CVAR_CREATE("cl_autowepswitch", "1", FCVAR_ARCHIVE | FCVAR_USERINFO);
	default_fov = CVAR_CREATE("default_fov", "90", FCVAR_ARCHIVE);
	m_pCvarStealMouse = CVAR_CREATE("hud_capturemouse", "1", FCVAR_ARCHIVE);
	m_pCvarDraw = CVAR_CREATE("hud_draw", "1", FCVAR_ARCHIVE);
	cl_lw = gEngfuncs.pfnGetCvarPointer("cl_lw");
	cl_rollangle = CVAR_CREATE("cl_rollangle", "2.0", FCVAR_ARCHIVE);
	cl_rollspeed = CVAR_CREATE("cl_rollspeed", "200", FCVAR_ARCHIVE);
	cl_bobtilt = CVAR_CREATE("cl_bobtilt", "0", FCVAR_ARCHIVE);

	RainInfo = gEngfuncs.pfnRegisterVariable("cl_raininfo", "0", 0);
	m_pSpriteList = NULL;
	m_pShinySurface = NULL; //LRC

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
	m_Scoreboard.Init();
	m_StatusBar.Init();
	m_DeathNotice.Init();
	m_AmmoSecondary.Init();
	m_TextMessage.Init();
	m_StatusIcons.Init();
	m_FlagIcons.Init();
	m_PlayerBrowse.Init();
	GetClientVoiceMgr()->Init(&g_VoiceStatusHelper, (vgui::Panel**)&gViewPort);
	m_Particle.Init(); // (LRC) -- 30/08/02 November235: Particles to Order
	m_PointMessage.Init();

	m_Menu.Init();
	InitRain();

	g_Bloom.Init();
	gFog.Init();
	g_WaterRenderer.Init();

	MsgFunc_ResetHUD(0, 0, NULL);
}

// CHud destructor
// cleans up memory allocated for m_rg* arrays
CHud::~CHud()
{
#ifdef ENGINE_DEBUG
	CONPRINT("## CHud::destructor\n");
#endif
	delete[] m_rghSprites;
	delete[] m_rgrcRects;
	delete[] m_rgszSpriteNames;
	gMP3.Shutdown();
	ResetRain();
	//LRC - clear all shiny surfaces
	if (m_pShinySurface)
	{
		delete m_pShinySurface;
		m_pShinySurface = NULL;
	}

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

// GetSpriteIndex()
// searches through the sprite list loaded from hud.txt for a name matching SpriteName
// returns an index into the gHUD.m_rghSprites[] array
// returns 0 if sprite not found
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
	CONPRINT("## CHud::VidInit (hi from me)\n");
#endif
	m_scrinfo.iSize = sizeof(m_scrinfo);
	GetScreenInfo(&m_scrinfo);

	// ----------
	// Load Sprites
	// ---------
	//	m_hsprFont = LoadSprite("sprites/%d_font.spr");

	m_hsprLogo = 0;
	m_hsprCursor = 0;
	numMirrors = 0;
	ResetRain();

	//LRC - clear all shiny surfaces
	if (m_pShinySurface)
	{
		delete m_pShinySurface;
		m_pShinySurface = NULL;
	}

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
			m_rgrcRects = new Rect[m_iSpriteCount];
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
	m_Scoreboard.VidInit();
	m_StatusBar.VidInit();
	m_DeathNotice.VidInit();
	m_SayText.VidInit();
	m_Menu.VidInit();
	m_AmmoSecondary.VidInit();
	m_TextMessage.VidInit();
	m_StatusIcons.VidInit();
	m_FlagIcons.VidInit();
	m_PlayerBrowse.VidInit();
	GetClientVoiceMgr()->VidInit();
	m_Particle.VidInit(); // (LRC) -- 30/08/02 November235: Particles to Order
	m_PointMessage.VidInit();
	gFog.VidInit();
	g_WaterRenderer.VidInit();

	usableSpr = SPR_Load("sprites/markent.spr");
	usableRect.bottom = SPR_Height(usableSpr, 0);
	usableRect.right = SPR_Width(usableSpr, 0);

	if (gHUD.m_iSmokeFrame == 0)
		m_iSmokeFrame = 19;

	// bacontsu - movement system
	servermaxspeed = NULL;
	m_bSprintButtonHeld = false;
	m_iSprintCounter = 200;
	m_flSprintUpdate = 0;
}

bool CHud::MsgFunc_Logo(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);

	// update Train data
	m_iLogo = READ_BYTE();

	return true;
}

//LRC
bool CHud::MsgFunc_HUDColor(const char* pszName, int iSize, void* pbuf)
{
	BEGIN_READ(pbuf, iSize);

	m_iHUDColor = READ_LONG();

	return 1;
}

float g_lastFOV = 0.0;

/*
============
COM_FileBase
============
*/
// Extracts the base name of a file (no path, no extension, assumes '/' as path separator)
void COM_FileBase(const char* in, char* out)
{
	int len, start, end;

	len = strlen(in);

	// scan backward for '.'
	end = len - 1;
	while (0 != end && in[end] != '.' && in[end] != '/' && in[end] != '\\')
		end--;

	if (in[end] != '.') // no '.', copy to end
		end = len - 1;
	else
		end--; // Found ',', copy to left of '.'


	// Scan backward for '/'
	start = len - 1;
	while (start >= 0 && in[start] != '/' && in[start] != '\\')
		start--;

	if (in[start] != '/' && in[start] != '\\')
		start = 0;
	else
		start++;

	// Length of new sting
	len = end - start + 1;

	// Copy partial string
	strncpy(out, &in[start], len);
	// Terminate it
	out[len] = 0;
}

/*
=================
HUD_IsGame

=================
*/
bool HUD_IsGame(const char* game)
{
	const char* gamedir;
	char gd[1024];

	gamedir = gEngfuncs.pfnGetGameDirectory();
	if (gamedir && '\0' != gamedir[0])
	{
		COM_FileBase(gamedir, gd);
		if (!stricmp(gd, game))
			return true;
	}
	return false;
}

/*
=====================
HUD_GetFOV

Returns last FOV
=====================
*/
float HUD_GetFOV()
{
	if (0 != gEngfuncs.pDemoAPI->IsRecording())
	{
		// Write it
		int i = 0;
		unsigned char buf[100];

		// Active
		*(float*)&buf[i] = g_lastFOV;
		i += sizeof(float);

		Demo_WriteBuffer(TYPE_ZOOM, i, buf);
	}

	if (0 != gEngfuncs.pDemoAPI->IsPlayingback())
	{
		g_lastFOV = g_demozoom;
	}
	return g_lastFOV;
}

bool CHud::MsgFunc_SetFOV(const char* pszName, int iSize, void* pbuf)
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

	return true;
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

void CHud::setNightVisionState(bool state)
{
	mNightVisionState = state;
}

extern void V_LadderBob();
void TiltLadderCamera()
{
	if (cvar_ladderbob && cvar_ladderbob->value > 0)
		V_LadderBob();
}