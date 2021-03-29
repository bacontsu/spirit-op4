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

#include "CTFScrollButton.h"
#include "CHandler_CommandButtonHighlight.h"

using namespace vgui;

// Arrow filenames
const char* sArrowFilenames[] =
{
    "arrowup",
    "arrowdn",
    "arrowlt",
    "arrowrt",
};

CTFScrollButton::CTFScrollButton(int iArrow, const char* text, int x, int y, int wide, int tall) : CommandButton(text, x, y, wide, tall)
{
    // Set text color to orange
    setFgColor(Scheme::sc_primary1);

    // Load in the arrow
    m_pTGA = LoadTGAForRes(sArrowFilenames[iArrow]);
    setImage(m_pTGA);

    // Highlight signal
    InputSignal* pISignal = new CHandler_CommandButtonHighlight(this);
    addInputSignal(pISignal);
}

void CTFScrollButton::paint()
{
    if (!m_pTGA)
        return;

    // draw armed button text in white
    if (isArmed())
    {
        m_pTGA->setColor(Color(255, 255, 255, 0));
    }
    else
    {
        m_pTGA->setColor(Color(255, 255, 255, 128));
    }

    m_pTGA->doPaint(this);
}

void CTFScrollButton::paintBackground()
{
    /*
        if ( isArmed() )
        {
            // Orange highlight background
            drawSetColor( Scheme::sc_primary2 );
            drawFilledRect(0,0,_size[0],_size[1]);
        }

        // Orange Border
        drawSetColor( Scheme::sc_secondary1 );
        drawOutlinedRect(0,0,_size[0]-1,_size[1]);
    */
}
