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

#include <VGUI_Button.h>

#include "vgui_common.h"
#include "CommandLabel.h"
#include "CSchemeManager.h"

class CCommandMenu;

class CommandButton : public vgui::Button
{
public:
    // Constructors
    CommandButton(const char* text, int x, int y, int wide, int tall, bool bNoHighlight, bool bFlat);
    CommandButton(const char* text, int x, int y, int wide, int tall, bool bNoHighlight = false);
    CommandButton(int iPlayerClass, const char* text, int x, int y, int wide, int tall, bool bFlat);

    void Init();

    // Menu Handling
    void AddSubMenu(CCommandMenu* pNewMenu);

    void AddSubLabel(CommandLabel* pSubLabel)
    {
        m_pSubLabel = pSubLabel;
    }

    virtual int IsNotValid()
    {
        return false;
    }

    void UpdateSubMenus(int iAdjustment);
    int GetPlayerClass() const { return m_iPlayerClass; }
    CCommandMenu* GetSubMenu() const { return m_pSubMenu; }

    CCommandMenu* getParentMenu();
    void setParentMenu(CCommandMenu* pParentMenu);

    // Overloaded vgui functions
    void paint() override;
    virtual void setText(const char* text);
    void paintBackground() override;

    void cursorEntered();
    void cursorExited();

    void setBoundKey(char boundKey);
    char getBoundKey();

public:
    bool m_bNoHighlight;

private:
    int m_iPlayerClass;
    bool m_bFlat;

    // Submenus under this button
    CCommandMenu* m_pSubMenu;
    CCommandMenu* m_pParentMenu;
    CommandLabel* m_pSubLabel;

    char m_sMainText[MAX_BUTTON_SIZE];
    char m_cBoundKey;

    SchemeHandle_t m_hTextScheme;

    void RecalculateText();
};
