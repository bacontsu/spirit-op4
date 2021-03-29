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

#include <VGUI_Slider.h>

// Custom drawn slider bar
class CTFSlider : public vgui::Slider
{
public:
    CTFSlider(int x, int y, int wide, int tall, bool vertical) : Slider(x, y, wide, tall, vertical)
    {
    }

    void paintBackground() override;
};
