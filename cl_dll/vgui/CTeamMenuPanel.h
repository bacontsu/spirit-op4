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

#include <VGUI_ScrollPanel.h>
#include <VGUI_TextPanel.h>

#include "CMenuPanel.h"
#include "CommandButton.h"

// Team Menu Dimensions
#define TEAMMENU_TITLE_X				XRES(40)
#define TEAMMENU_TITLE_Y				YRES(32)
#define TEAMMENU_TOPLEFT_BUTTON_X		XRES(40)
#define TEAMMENU_TOPLEFT_BUTTON_Y		YRES(80)
#define TEAMMENU_BUTTON_SIZE_X			XRES(124)
#define TEAMMENU_BUTTON_SIZE_Y			YRES(24)
#define TEAMMENU_BUTTON_SPACER_Y		YRES(8)
#define TEAMMENU_WINDOW_X				XRES(176)
#define TEAMMENU_WINDOW_Y				YRES(80)
#define TEAMMENU_WINDOW_SIZE_X			XRES(424)
#define TEAMMENU_WINDOW_SIZE_Y			YRES(312)
#define TEAMMENU_WINDOW_TITLE_X			XRES(16)
#define TEAMMENU_WINDOW_TITLE_Y			YRES(16)
#define TEAMMENU_WINDOW_TEXT_X			XRES(16)
#define TEAMMENU_WINDOW_TEXT_Y			YRES(48)
#define TEAMMENU_WINDOW_TEXT_SIZE_Y		YRES(178)
#define TEAMMENU_WINDOW_INFO_X			XRES(16)
#define TEAMMENU_WINDOW_INFO_Y			YRES(234)

class CTeamMenuPanel : public CMenuPanel
{
public:
    CTeamMenuPanel(int iTrans, int iRemoveMe, int x, int y, int wide, int tall);

    bool SlotInput(int iSlot) override;
    void Open() override;
    virtual void Update();
    void SetActiveInfo(int iInput) override;
    void paintBackground() override;

    virtual void Initialize();

    void Reset() override
    {
        CMenuPanel::Reset();
        m_iCurrentInfo = 0;
    }

public:
    vgui::ScrollPanel* m_pScrollPanel;
    CTransparentPanel* m_pTeamWindow;
    vgui::Label* m_pMapTitle;
    vgui::TextPanel* m_pBriefing;
    vgui::TextPanel* m_pTeamInfoPanel[6];
    CommandButton* m_pButtons[6];
    bool m_bUpdatedMapName;
    CommandButton* m_pCancelButton;
    CommandButton* m_pSpectateButton;
    int m_iCurrentInfo;
};
