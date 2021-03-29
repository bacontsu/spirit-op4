//========= Copyright � 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// Triangle rendering, if any

#include "hud/hud.h"
#include "cl_util.h"

// Triangle rendering apis are in gEngfuncs.pTriAPI

#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "triangleapi.h"
#include "effects/aurora/ParticleSystemManager.h"
#include "com_model.h"

#include "Exports.h"
#include "particleman.h"
#include "effects/CWeather.h"

#include "glInclude.h"
#include "hud/CHud.h"

extern IParticleMan* g_pParticleMan;

extern int g_iWaterLevel;
extern vec3_t v_origin;


//
//-----------------------------------------------------
//

void SetPoint(float x, float y, float z, float (*matrix)[4])
{
    vec3_t point, result;
    point[0] = x;
    point[1] = y;
    point[2] = z;

    VectorTransform(point, matrix, result);

    gEngfuncs.pTriAPI->Vertex3f(result[0], result[1], result[2]);
}


//
//-----------------------------------------------------
//

//LRCT

void BlackFog()
{
    //Not in water and we want fog.
    static float fColorBlack[3] = {0, 0, 0};
    bool bFog = g_iWaterLevel < 2 && g_fog.startDist > 0 && g_fog.endDist > 0;
    if (bFog)
        gEngfuncs.pTriAPI->Fog(fColorBlack, g_fog.startDist, g_fog.endDist, bFog);
    else
        gEngfuncs.pTriAPI->Fog(g_fog.fogColor, g_fog.startDist, g_fog.endDist, bFog);
}

void RenderFog()
{
    //Not in water and we want fog.
    bool bFog = g_iWaterLevel < 2 && g_fog.startDist > 0 && g_fog.endDist > 0;
    if (bFog)
        gEngfuncs.pTriAPI->Fog(g_fog.fogColor, g_fog.startDist, g_fog.endDist, bFog);
    //	else
    //		gEngfuncs.pTriAPI->Fog ( g_fFogColor, 10000, 10001, 0 );
}

void ClearToFogColor()
{
    if (g_fog.startDist > 0 && g_fog.endDist > 0)
    {
        glClearColor(g_fog.fogColor[0], g_fog.fogColor[1], g_fog.fogColor[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

/*
=================
HUD_DrawNormalTriangles

Non-transparent triangles-- add them here
=================
*/
void DLLEXPORT HUD_DrawNormalTriangles()
{
    //	RecClDrawNormalTriangles();

    gHUD.m_Spectator.DrawOverview();
}

/*
=================
HUD_DrawTransparentTriangles

Render any triangles with transparent rendermode needs here
=================
*/
extern ParticleSystemManager* g_pParticleSystems; // LRC

void DLLEXPORT HUD_DrawTransparentTriangles()
{
    //	RecClDrawTransparentTriangles();

    if (g_pParticleMan)
    {
        g_pParticleMan->Update();
        g_Weather.Update();
    }

    BlackFog();

    // LRC: find out the time elapsed since the last redraw
    static float fOldTime, fTime;
    fOldTime = fTime;
    fTime = gEngfuncs.GetClientTime();

    // LRC: draw and update particle systems
    g_pParticleSystems->UpdateSystems(fTime - fOldTime);
}
