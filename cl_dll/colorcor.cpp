//========= Copyright � 2011, Fire64, All rights reserved. ============//
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
}

void CColorCorTexture::BindTexture(int width, int height)
{ 
     glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_texture);

	if (!(gEngfuncs.GetViewModel() && gEngfuncs.GetViewModel()->model))
	{
		 if (CVAR_GET_FLOAT("colorcor_blackwhite") == 1)
		 {
			 glCopyTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_LUMINANCE, 0, 0, ScreenWidth, ScreenHeight, 0);
		 }
		 else
		 {
			 glCopyTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGBA8, 0, 0, ScreenWidth, ScreenHeight, 0);
		 }
	}
	 else
		glCopyTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGBA8, 0, 0, ScreenWidth, ScreenHeight, 0);
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

	if( CVAR_GET_FLOAT( "colorcor_inverse" ) == 1 )
	{
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_COPY_INVERTED);
	}

	glBegin(GL_QUADS);
		DrawQuad(width, height,of);
	glEnd();

	if( CVAR_GET_FLOAT( "colorcor_inverse" ) == 1 )
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

void CColorCor::DrawColorCor()
{
	m_pTextures.r = CVAR_GET_FLOAT( "colorcor_r" );//was 0.3
	m_pTextures.g = CVAR_GET_FLOAT( "colorcor_g" );//was 0.3
	m_pTextures.b = CVAR_GET_FLOAT( "colorcor_b" );//was 0.3


	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);  
	glEnable(GL_BLEND);	

	m_pTextures.BindTexture(ScreenWidth,ScreenHeight);
	m_pTextures.Draw(ScreenWidth,ScreenHeight);

	m_pTextures.alpha = CVAR_GET_FLOAT( "colorcor_alpha" );//was 0.2
	m_pTextures.of = 0;

	glDisable(GL_BLEND);
}

void CColorCor::ProcessBlackWhite()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);
	glEnable(GL_BLEND);	

	 glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_bwtexture);

	if (CVAR_GET_FLOAT("colorcor_blackwhite") == 1)
	{
		glCopyTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_LUMINANCE, 0, 0, ScreenWidth, ScreenHeight, 0);
	}
	else
	{
		glCopyTexImage2D(GL_TEXTURE_RECTANGLE_NV, 0, GL_RGBA8, 0, 0, ScreenWidth, ScreenHeight, 0);
	}

	// enable some OpenGL stuff
	glEnable(GL_TEXTURE_RECTANGLE_NV);
	glColor3f(1, 1, 1);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1, 1, 0, 0.1, 100);

	glBindTexture(GL_TEXTURE_RECTANGLE_NV, g_bwtexture);

	glColor4f(1, 1, 1, 1);

	int width = ScreenWidth;
	int height = ScreenHeight;
	int of = 0;

	glBegin(GL_QUADS);
	
	glTexCoord2f(0 - of, 0 - of);
	glVertex3f(0, 1, -1);
	glTexCoord2f(0 - of, height + of);
	glVertex3f(0, 0, -1);
	glTexCoord2f(width + of, height + of);
	glVertex3f(1, 0, -1);
	glTexCoord2f(width + of, 0 - of);
	glVertex3f(1, 1, -1);

	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glDisable(GL_TEXTURE_RECTANGLE_NV);
	glEnable(GL_DEPTH_TEST);


	glDisable(GL_BLEND);
	
}


