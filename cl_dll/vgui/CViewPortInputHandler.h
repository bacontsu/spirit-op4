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
#include <VGUI_MouseCode.h>

//-----------------------------------------------------------------------------
// Purpose: InputSignal handler for the main viewport
//-----------------------------------------------------------------------------
class CViewPortInputHandler : public vgui::InputSignal
{
public:
    bool bPressed;

    CViewPortInputHandler() {}

    void cursorMoved(int x, int y, vgui::Panel* panel) override {}
    void cursorEntered(vgui::Panel* panel) override {}
    void cursorExited(vgui::Panel* panel) override {}

    void mousePressed(vgui::MouseCode code, vgui::Panel* panel) override
    {
        if (code != vgui::MOUSE_LEFT)
        {
            // send a message to close the command menu
            // this needs to be a message, since a direct call screws the timing
            gEngfuncs.pfnClientCmd("ForceCloseCommandMenu\n");
        }
    }

    void mouseReleased(vgui::MouseCode code, vgui::Panel* panel) override {}
    void mouseDoublePressed(vgui::MouseCode code, vgui::Panel* panel) override {}
    void mouseWheeled(int delta, vgui::Panel* panel) override {}
    void keyPressed(vgui::KeyCode code, vgui::Panel* panel) override {}
    void keyTyped(vgui::KeyCode code, vgui::Panel* panel) override {}
    void keyReleased(vgui::KeyCode code, vgui::Panel* panel) override {}
    void keyFocusTicked(vgui::Panel* panel) override {}
};
