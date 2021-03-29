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

#include <VGUI_FileInputStream.h>

#include "hud/CHud.h"
#include "cl_util.h"
#include "CImageLabel.h"
#include "vgui/vgui_common.h"

using namespace vgui;

//===========================================================
// Button with Class image beneath it
CImageLabel::CImageLabel(const char* pImageName, int x, int y) : Label("", x, y)
{
    setContentFitted(true);
    m_pTGA = LoadTGAForRes(pImageName);
    setImage(m_pTGA);
}

CImageLabel::CImageLabel(const char* pImageName, int x, int y, int wide, int tall) : Label("", x, y, wide, tall)
{
    setContentFitted(true);
    m_pTGA = LoadTGAForRes(pImageName);
    setImage(m_pTGA);
}

//===========================================================
// Image size
int CImageLabel::getImageWide()
{
    if (m_pTGA)
    {
        int iXSize, iYSize;
        m_pTGA->getSize(iXSize, iYSize);
        return iXSize;
    }
    else
    {
        return 1;
    }
}

int CImageLabel::getImageTall()
{
    if (m_pTGA)
    {
        int iXSize, iYSize;
        m_pTGA->getSize(iXSize, iYSize);
        return iYSize;
    }
    else
    {
        return 1;
    }
}

void CImageLabel::LoadImage(const char* pImageName)
{
    if (m_pTGA)
        delete m_pTGA;

    // Load the Image
    m_pTGA = LoadTGAForRes(pImageName);

    if (m_pTGA == NULL)
    {
        // we didn't find a matching image file for this resolution
        // try to load file resolution independent

        char sz[256];
        sprintf(sz, "%s/%s", gEngfuncs.pfnGetGameDirectory(), pImageName);
        vgui::FileInputStream* fis = new FileInputStream(sz, false);
        m_pTGA = new BitmapTGA(fis, true);
        fis->close();
    }

    if (m_pTGA == NULL)
        return; // unable to load image

    int w, t;

    m_pTGA->getSize(w, t);

    setSize(XRES(w), YRES(t));
    setImage(m_pTGA);
}
