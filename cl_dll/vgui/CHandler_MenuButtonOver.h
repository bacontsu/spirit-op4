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

#include <VGUI_InputSignal.h>

#include "CMenuPanel.h"

class CHandler_MenuButtonOver : public vgui::InputSignal
{
public:
    CHandler_MenuButtonOver(CMenuPanel* pPanel, int iButton)
    {
        m_iButton = iButton;
        m_pMenuPanel = pPanel;
    }

    void cursorEntered(vgui::Panel* panel) override;

    void cursorMoved(int x, int y, vgui::Panel* panel) override {}
    void mousePressed(vgui::MouseCode code, vgui::Panel* panel) override {}
    void mouseReleased(vgui::MouseCode code, vgui::Panel* panel) override {}
    void mouseDoublePressed(vgui::MouseCode code, vgui::Panel* panel) override {}
    void cursorExited(vgui::Panel* panel) override {}
    void mouseWheeled(int delta, vgui::Panel* panel) override {}
    void keyPressed(vgui::KeyCode code, vgui::Panel* panel) override {}
    void keyTyped(vgui::KeyCode code, vgui::Panel* panel) override {}
    void keyReleased(vgui::KeyCode code, vgui::Panel* panel) override {}
    void keyFocusTicked(vgui::Panel* panel) override {}

private:
    int m_iButton;
    CMenuPanel* m_pMenuPanel;
};
