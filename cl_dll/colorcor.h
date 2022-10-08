//========= Copyright � 2004-2008, Raven City Team, All rights reserved. ============//
//																					 //
// Purpose:																			 //
//																					 //
// $NoKeywords: $																	 //
//===================================================================================//
// Original code by Ryokeen, modified by Highlander.

class CColorCorTexture
{
public:
	CColorCorTexture();
	void Init(int width, int height);
	void BindTexture(int width, int height);
	void DrawQuad(int width, int height,int of);
	void Draw(int width, int height);
	unsigned int g_texture;

	float alpha;
	float r,g,b;
	float of;
};
class CColorCor
{
public:
	void InitScreen(void);
	void DrawColorCor(void);
	void ProcessBlackWhite();

	unsigned int g_bwtexture;

	CColorCorTexture m_pTextures;
};

extern CColorCor gColorCor;