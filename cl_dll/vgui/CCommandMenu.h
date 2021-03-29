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

#include "CommandButton.h"
#include "vgui_common.h"

class CCommandMenu : public vgui::Panel
{
private:
    CCommandMenu* m_pParentMenu;
    int m_iXOffset;
    int m_iYOffset;

    // Buttons in this menu
    CommandButton* m_aButtons[MAX_BUTTONS];
    int m_iButtons;

    // opens menu from top to bottom (0 = default), or from bottom to top (1)?
    int m_iDirection;
public:
    CCommandMenu(CCommandMenu* pParentMenu, int x, int y, int wide, int tall);
    CCommandMenu(CCommandMenu* pParentMenu, int direction, int x, int y, int wide, int tall);

    float m_flButtonSizeY;
    int m_iSpectCmdMenu;

    void AddButton(CommandButton* pButton);
    void RemoveAllButtons();
    bool KeyInput(int keyNum);
    void ClearButtonsOfArmedState();
    CommandButton* FindButtonWithSubmenu(CCommandMenu* pSubMenu);

    bool RecalculateVisibles(int iNewYPos, bool bHideAll);
    void RecalculatePositions(int iYOffset);
    void MakeVisible(CCommandMenu* pChildMenu);

    CCommandMenu* GetParentMenu() const { return m_pParentMenu; }
    int GetXOffset() const { return m_iXOffset; }
    int GetYOffset() const { return m_iYOffset; }
    int GetDirection() const { return m_iDirection; }
    int GetNumButtons() const { return m_iButtons; }

    void paintBackground() override;
};
