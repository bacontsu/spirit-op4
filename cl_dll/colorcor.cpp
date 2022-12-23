//========= Copyright © 2011, Fire64, All rights reserved. ============//
//																					 //
// Purpose:																			 //
//																					 //
// $NoKeywords: $																	 //
//===================================================================================//

#include "hud.h"
#include "cl_util.h"

#include "PlatformHeaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <gl\gl.h>										// Header File For The OpenGL32 Library							// Header File For The 32 Library

#include "colorcor.h"

#ifndef GL_TEXTURE_RECTANGLE_NV
#define GL_TEXTURE_RECTANGLE_NV 0x84F5
#endif

cvar_t* cvar_red;
cvar_t* cvar_green;
cvar_t* cvar_blue;
cvar_t* cvar_alpha;
cvar_t* cvar_bnw;
cvar_t* cvar_inverse;
cvar_t* cvar_noirmode;

CColorCorTexture::CColorCorTexture() {};

void CColorCorTexture::Init(int width, int height)
{	
		 // create a load of blank pixels to create textures with
     unsigned char* pBlankTex = new unsigned char[width*height*3];
     memset(pBlankTex, 0, width*height*3);

	  // Create the SCREEN-HOLDING TEXTURE
     glGenTextures(1, &g_texture);
     glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_texture);

     glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_RECTANGLE_NV, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGBA8, width, height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, 0);

	   // free the memory
     delete[] pBlankTex;

	 // Color correction
	 cvar_red = CVAR_CREATE("colorcor_r", "1", FCVAR_ARCHIVE);
	 cvar_green = CVAR_CREATE("colorcor_g", "1", FCVAR_ARCHIVE);
	 cvar_blue = CVAR_CREATE("colorcor_b", "1", FCVAR_ARCHIVE);
	 cvar_alpha = CVAR_CREATE("colorcor_alpha", "1.0", FCVAR_ARCHIVE);
	 cvar_inverse =CVAR_CREATE("colorcor_inverse", "0", FCVAR_ARCHIVE);
	 cvar_bnw = CVAR_CREATE("colorcor_blackwhite", "0", FCVAR_ARCHIVE);

	 cvar_noirmode = CVAR_CREATE("cl_noirmode", "0", FCVAR_ARCHIVE);
}

void CColorCorTexture::BindTexture(int width, int height)
{ 
     glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_texture);

	if (cvar_bnw->value == 1 || cvar_noirmode->value == 1)
	 {
		 glCopyTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_LUMINANCE, 0, 0, ScreenWidth, ScreenHeight, 0);
	 }
	 else
	 {
		 glCopyTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGBA8, 0, 0, ScreenWidth, ScreenHeight, 0);
	 }
}

void CColorCorTexture:: Draw(int width, int height)
{
	 // enable some OpenGL stuff
     glEnable(GL_TEXTURE_RECTANGLE_NV);     
     glColor3f(1,1,1);
     glDisable(GL_DEPTH_TEST);

	 glMatrixMode(GL_MODELVIEW);
     glPushMatrix();
     glLoadIdentity();
     
     glMatrixMode(GL_PROJECTION);
     glPushMatrix();
     glLoadIdentity();
     glOrtho(0, 1, 1, 0, 0.1, 100);

	 glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_texture);

	 glColor4f(r,g,b,alpha);

	if( cvar_inverse->value == 1 )
	{
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_COPY_INVERTED);
	}

	glBegin(GL_QUADS);
		DrawQuad(width, height,of);
	glEnd();

	if (cvar_inverse->value == 1)
	{
		glDisable(GL_COLOR_LOGIC_OP);
	}

	 glMatrixMode(GL_PROJECTION);
     glPopMatrix();

     glMatrixMode(GL_MODELVIEW);
     glPopMatrix();

     glDisable(GL_TEXTURE_RECTANGLE_NV);
     glEnable(GL_DEPTH_TEST);
}

void CColorCorTexture::DrawQuad(int width, int height,int of)
{
	 glTexCoord2f(0-of,0-of);
     glVertex3f(0, 1, -1);
     glTexCoord2f(0-of,height+of);
     glVertex3f(0, 0, -1);
     glTexCoord2f(width+of,height+of);
     glVertex3f(1, 0, -1);
     glTexCoord2f(width+of,0-of);
     glVertex3f(1, 1, -1);
}

CColorCor gColorCor;

void CColorCor::InitScreen()
{	
	m_pTextures.Init(ScreenWidth,ScreenHeight);
}

void CColorCor::VidInit()
{

}

void CColorCor::DrawColorCor()
{
	float targetR = cvar_red->value; // was 0.3
	lerpR = (targetR * 0.03f * 300 * gHUD.m_flTimeDelta) + (lerpR * (1.0 - 0.03f * 300 * gHUD.m_flTimeDelta));

	float targetG = cvar_green->value; // was 0.3
	lerpG = (targetG * 0.03f * 300 * gHUD.m_flTimeDelta) + (lerpG * (1.0 - 0.03f * 300 * gHUD.m_flTimeDelta));

	float targetB = cvar_blue->value; // was 0.3
	lerpB = (targetB * 0.03f * 300 * gHUD.m_flTimeDelta) + (lerpB * (1.0 - 0.03f * 300 * gHUD.m_flTimeDelta));

	float targetAlpha = cvar_alpha->value; // was 0.2
	lerpAlpha = (targetAlpha * 0.03f * 300 * gHUD.m_flTimeDelta) + (lerpAlpha * (1.0 - 0.03f * 300 * gHUD.m_flTimeDelta));

	if (cvar_noirmode->value == 1)
	{
		lerpR = 1.0f;
		lerpG = 1.0f;
		lerpB = 1.0f;
	}

	m_pTextures.r = lerpR;
	m_pTextures.g = lerpG;
	m_pTextures.b = lerpB;

	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);  
	glEnable(GL_BLEND);	

	m_pTextures.BindTexture(ScreenWidth,ScreenHeight);
	m_pTextures.Draw(ScreenWidth,ScreenHeight);

	m_pTextures.alpha = lerpAlpha;
	m_pTextures.of = 0;

	glDisable(GL_BLEND);
}

void CColorCor::ProcessBlackWhite()
{

}


