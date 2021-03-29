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

#include "CommandButton.h"

// Custom drawn scroll bars
class CTFScrollButton : public CommandButton
{
public:
    CTFScrollButton(int iArrow, const char* text, int x, int y, int wide, int tall);

    void paint() override;
    void paintBackground() override;

private:
    vgui::BitmapTGA* m_pTGA;
};
