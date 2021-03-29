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
#include "hud/CHud.h"

#define MAX_MAPNAME 256

class MapButton : public CommandButton
{
public:
    MapButton(const char* pMapName, const char* text, int x, int y, int wide, int tall) : CommandButton(text, x, y, wide, tall)
    {
        sprintf(m_szMapName, "maps/%s.bsp", pMapName);
    }

    int IsNotValid() override
    {
        const char* level = gEngfuncs.pfnGetLevelName();
        if (!level)
            return true;

        // Does it match the current map name?
        if (strcmp(m_szMapName, level))
            return true;

        return false;
    }

private:
    char m_szMapName[MAX_MAPNAME];
};
