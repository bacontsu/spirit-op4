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
#include <VGUI_InputSignal.h>

class CHandler_ButtonHighlight : public vgui::InputSignal
{
public:
    CHandler_ButtonHighlight(vgui::Button* pButton)
    {
        m_pButton = pButton;
    }

    void cursorEntered(vgui::Panel* panel) override
    {
        m_pButton->setArmed(true);
    }

    void cursorExited(vgui::Panel* Panel) override
    {
        m_pButton->setArmed(false);
    }

    void mousePressed(vgui::MouseCode code, vgui::Panel* panel) override {}
    void mouseReleased(vgui::MouseCode code, vgui::Panel* panel) override {}
    void cursorMoved(int x, int y, vgui::Panel* panel) override {}
    void mouseDoublePressed(vgui::MouseCode code, vgui::Panel* panel) override {}
    void mouseWheeled(int delta, vgui::Panel* panel) override {}
    void keyPressed(vgui::KeyCode code, vgui::Panel* panel) override {}
    void keyTyped(vgui::KeyCode code, vgui::Panel* panel) override {}
    void keyReleased(vgui::KeyCode code, vgui::Panel* panel) override {}
    void keyFocusTicked(vgui::Panel* panel) override {}

private:
    vgui::Button* m_pButton;
};
