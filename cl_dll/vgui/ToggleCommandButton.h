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

#include "CImageLabel.h"
#include "CommandButton.h"
#include "hud/CHud.h"

class ToggleCommandButton : public CommandButton, public vgui::InputSignal
{
public:
    ToggleCommandButton(const char* cvarname, const char* text, int x, int y, int wide, int tall, bool flat);

    void cursorEntered(Panel* panel) override;
    void cursorExited(Panel* panel) override;
    void mousePressed(vgui::MouseCode code, Panel* panel) override;

    void cursorMoved(int x, int y, Panel* panel) override {}
    void mouseDoublePressed(vgui::MouseCode code, Panel* panel) override {}
    void mouseReleased(vgui::MouseCode code, Panel* panel) override {}
    void mouseWheeled(int delta, Panel* panel) override {}
    void keyPressed(vgui::KeyCode code, Panel* panel) override {}
    void keyTyped(vgui::KeyCode code, Panel* panel) override {}
    void keyReleased(vgui::KeyCode code, Panel* panel) override {}
    void keyFocusTicked(Panel* panel) override {}

    void paint() override;

private:
    struct cvar_s* m_cvar;
    CImageLabel* pLabelOn;
    CImageLabel* pLabelOff;
};
