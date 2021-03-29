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
* Valve LLC.  All other use, distribution, or modification is prohibited
* without written permission from Valve LLC.
*
****/

#include "CTFSlider.h"

using namespace vgui;

void CTFSlider::paintBackground()
{
    int wide, tall, nobx, noby;
    getPaintSize(wide, tall);
    getNobPos(nobx, noby);

    // Border
    drawSetColor(Scheme::sc_secondary1);
    drawOutlinedRect(0, 0, wide, tall);

    if (isVertical())
    {
        // Nob Fill
        drawSetColor(Scheme::sc_primary2);
        drawFilledRect(0, nobx, wide, noby);

        // Nob Outline
        drawSetColor(Scheme::sc_primary1);
        drawOutlinedRect(0, nobx, wide, noby);
    }
    else
    {
        // Nob Fill
        drawSetColor(Scheme::sc_primary2);
        drawFilledRect(nobx, 0, noby, tall);

        // Nob Outline
        drawSetColor(Scheme::sc_primary1);
        drawOutlinedRect(nobx, 0, noby, tall);
    }
}
