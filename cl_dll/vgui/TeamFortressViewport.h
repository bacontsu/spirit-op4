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

#include <VGUI_Panel.h>

#include "CCommandMenu.h"
#include "CMenuPanel.h"
#include "CSchemeManager.h"
#include "CTeamMenuPanel.h"
#include "vgui_common.h"
#include "ScorePanel.h"
#include "SpectatorPanel.h"
#include "hud/hud.h"

class TeamFortressViewport : public vgui::Panel
{
public:
    TeamFortressViewport(int x, int y, int wide, int tall);
    void Initialize();

    int CreateCommandMenu(const char* menuFile, int direction, int yOffset, bool flatDesign, float flButtonSizeX, float flButtonSizeY, int xOffset);
    void CreateScoreBoard();
    CommandButton* CreateCustomButton(char* pButtonText, char* pButtonName, int iYOffset);
    CCommandMenu* CreateDisguiseSubmenu(CommandButton* pButton, CCommandMenu* pParentMenu, const char* commandText, int iYOffset, int iXOffset = 0);

    void UpdateCursorState();
    void UpdateCommandMenu(int menuIndex);
    void UpdateOnPlayerInfo();
    void UpdateHighlights();
    void UpdateSpectatorPanel();

    int KeyInput(int down, int keynum, const char* pszCurrentBinding);
    void GetAllPlayersInfo();
    void DeathMsg(int killer, int victim);

    void ShowCommandMenu(int menuIndex);
    void InputSignalHideCommandMenu();
    void HideCommandMenu();
    void SetCurrentCommandMenu(CCommandMenu* pNewMenu);
    void SetCurrentMenu(CMenuPanel* pMenu);

    void ShowScoreBoard();
    void HideScoreBoard();
    bool IsScoreBoardVisible();

    bool AllowedToPrintText();

    void ShowVGUIMenu(int iMenu);
    void HideVGUIMenu();
    void HideTopMenu();

    CMenuPanel* CreateTextWindow(int iTextToShow);

    CCommandMenu* CreateSubMenu(CommandButton* pButton, CCommandMenu* pParentMenu, int iYOffset, int iXOffset = 0);

    // Data Handlers
    int GetValidClasses(int iTeam) { return m_iValidClasses[iTeam]; }
    int GetNumberOfTeams() const { return m_iNumberOfTeams; }
    int GetIsFeigning() const { return m_iIsFeigning; }
    int GetIsSettingDetpack() const { return m_iIsSettingDetpack; }
    int GetBuildState() const { return m_iBuildState; }
    int IsRandomPC() const { return m_iRandomPC; }
    char* GetTeamName(int iTeam) { return m_sTeamNames[iTeam]; }
    int GetAllowSpectators() const { return m_iAllowSpectators; }

    // Message Handlers
    int MsgFunc_ValClass(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_TeamNames(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_Feign(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_Detpack(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_VGUIMenu(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_MOTD(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_BuildSt(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_RandomPC(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_ServerName(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_ScoreInfo(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_TeamScore(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_TeamInfo(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_Spectator(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_AllowSpec(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_SpecFade(const char* pszName, int iSize, void* pbuf);
    int MsgFunc_ResetFade(const char* pszName, int iSize, void* pbuf);

    // Input
    bool SlotInput(int iSlot);

    virtual void paintBackground();

    CSchemeManager* GetSchemeManager() { return &m_SchemeManager; }
    ScorePanel* GetScoreBoard() const { return m_pScoreBoard; }

    void* operator new(size_t stAllocateBlock);

public:
    // VGUI Menus
    CMenuPanel* m_pCurrentMenu;
    CTeamMenuPanel* m_pTeamMenu;
    int m_StandardMenu; // indexs in m_pCommandMenus
    int m_SpectatorOptionsMenu;
    int m_SpectatorCameraMenu;
    int m_PlayerMenu; // a list of current player
    ScorePanel* m_pScoreBoard;
    SpectatorPanel* m_pSpectatorPanel;
    char m_szServerName[MAX_SERVERNAME_LENGTH];

private:
    vgui::Cursor* _cursorNone;
    vgui::Cursor* _cursorArrow;

    int m_iInitialized;

    CCommandMenu* m_pCommandMenus[MAX_MENUS];
    CCommandMenu* m_pCurrentCommandMenu;
    float m_flMenuOpenTime;
    float m_flScoreBoardLastUpdated;
    float m_flSpectatorPanelLastUpdated;
    int m_iNumMenus;
    int m_iCurrentTeamNumber;
    int m_iCurrentPlayerClass;
    int m_iUser1;
    int m_iUser2;
    int m_iUser3;

    // VGUI Menus
    void CreateTeamMenu();
    CMenuPanel* ShowTeamMenu();
    void CreateSpectatorMenu();

    // Scheme handler
    CSchemeManager m_SchemeManager;

    // MOTD
    int m_iGotAllMOTD;
    char m_szMOTD[MAX_MOTD_LENGTH];

    //  Command Menu Team buttons
    CommandButton* m_pTeamButtons[6];
    CommandButton* m_pDisguiseButtons[5];

    int m_iAllowSpectators;

    // Data for specific sections of the Command Menu
    int m_iValidClasses[5];
    int m_iIsFeigning;
    int m_iIsSettingDetpack;
    int m_iNumberOfTeams;
    int m_iBuildState;
    int m_iRandomPC;
    char m_sTeamNames[5][MAX_TEAMNAME_SIZE];

    // Localisation strings
    char m_sDetpackStrings[3][MAX_BUTTON_SIZE];

    char m_sMapName[64];

    // helper function to update the player menu entries
    void UpdatePlayerMenu(int menuIndex);
};

extern TeamFortressViewport* gViewPort;
