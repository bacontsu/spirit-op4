//=========== (C) Copyright 1996-2002 Valve, L.L.C. All rights reserved. ===========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//
// Purpose: TFC Stats Menu
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================

#include <VGUI_TextImage.h>

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "vgui_StatsMenuPanel.h"

struct team_stat_info_t
{
	byte bChunksRead;
	char szMVP[32];
	char szMostKills[32];
	char szMostCTF[32];
	char szMostOff[32];
	char szMostDef[32];
	char szMostSnipe[32];
	char szMostBarnacle[32];
	char szMostDeaths[32];
	char szMostSuicides[32];
	char szMostDamage[32];
	char szMostAccel[32];
	char szMostBackpack[32];
	char szMostHealth[32];
	char szMostShield[32];
	char szMostJump[32];
	int iMVPVal;
	int iKillsVal;
	int iCTFVal;
	int iOffVal;
	int iDefVal;
	int iSnipeVal;
	int iBarnacleVal;
	int iDeathsVal;
	int iSuicidesVal;
	int iDamageVal;
	int iAccelVal;
	int iBackpackVal;
	int iHealthVal;
	int iShieldVal;
	int iJumpVal;
};


#define STATSMENU_TITLE_X XRES(40)
#define STATSMENU_TITLE_Y YRES(32)
#define STATSMENU_TOPLEFT_BUTTON_X XRES(40)
#define STATSMENU_TOPLEFT_BUTTON_Y YRES(400)
#define STATSMENU_BUTTON_SIZE_X XRES(124)
#define STATSMENU_BUTTON_SIZE_Y YRES(24)
#define STATSMENU_BUTTON_SPACER_Y YRES(8)
#define STATSMENU_WINDOW_X XRES(40)
#define STATSMENU_WINDOW_Y YRES(80)
#define STATSMENU_WINDOW_SIZE_X XRES(600)
#define STATSMENU_WINDOW_SIZE_Y YRES(312)
#define STATSMENU_WINDOW_TEXT_X XRES(150)
#define STATSMENU_WINDOW_TEXT_Y YRES(80)
#define STATSMENU_WINDOW_NAME_X XRES(150)
#define STATSMENU_WINDOW_NAME_Y YRES(8)

static char szStatsBuf[StatsTeamsCount][1024];
static int g_iWinningTeam = 0;
static team_stat_info_t g_TeamStatInfo[3];

CStatsMenuPanel::CStatsMenuPanel(int iTrans, bool iRemoveMe, int x, int y, int wide, int tall)
	: CMenuPanel(iTrans, iRemoveMe, x, y, wide, tall)
{
	
}

void CStatsMenuPanel::Initialize()
{
	setVisible(false);
	m_pScrollPanel->setScrollValue(0, 0);
}

void CStatsMenuPanel::Open()
{
	Update();
	CMenuPanel::Open();
}

void CStatsMenuPanel::Reset()
{
	CMenuPanel::Reset();
	m_iCurrentInfo = 0;
}

void CStatsMenuPanel::SetActiveInfo(int iInput)
{
	for (int i = 0; i < StatsTeamsCount; ++i)
	{
		m_pButtons[i]->setArmed(false);
		m_pClassInfoPanel[i]->setVisible(false);
	}

	if (iInput < 0 || iInput >= StatsTeamsCount)
		iInput = 0;

	m_pButtons[iInput]->setArmed(true);
	m_pClassInfoPanel[iInput]->setVisible(true);

	m_iCurrentInfo = iInput;

	m_pScrollPanel->setScrollValue(0, 0);
	m_pScrollPanel->validate();
}

bool CStatsMenuPanel::SlotInput(int iSlot)
{
	const int buttonIndex = iSlot - 1;

	if (buttonIndex >= 0 && buttonIndex < 3)
	{
		auto button = m_pButtons[buttonIndex];

		if (button && !button->IsNotValid())
		{
			button->fireActionSignal();
			return true;
		}
	}

	return false;
}

void CStatsMenuPanel::SetPlayerImage(const char* szImage)
{
	CImageLabel* image = m_pClassImages[3];

	if (image)
	{
		image->LoadImage(szImage);
	}
}

bool CStatsMenuPanel::MsgFunc_StatsInfo(const char* pszName, int iSize, void* pbuf)
{
	

	return false;
}

bool CStatsMenuPanel::MsgFunc_StatsPlayer(const char* pszName, int iSize, void* pbuf)
{
	

	return false;
}
