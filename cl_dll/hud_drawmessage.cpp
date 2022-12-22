
#include "stdio.h"
#include "stdlib.h"
#include <string>
#include <vector>
#include <string.h>

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"
#include "triangleapi.h"
#include "r_studioint.h"
#include "com_model.h"
#include "UserMessages.h"

// imgui
#include "PlatformHeaders.h"
#include "SDL2/SDL.h"
#include <gl/GL.h>

#include "imgui.h"
#include "backends/imgui_impl_opengl2.h"
#include "backends/imgui_impl_sdl.h"


struct CClientPointMessage
{
	Vector origin;
	std::string message;
	bool developerOnly;
	float maxDistance;
};

SDL_Window* mainWindow;
SDL_GLContext mainContext;
std::vector<CClientPointMessage> MessageList;

DECLARE_MESSAGE(m_PointMessage, PointMsg);

bool CPointMessageRenderer::Init()
{
	m_iFlags |= HUD_ACTIVE;
	HOOK_MESSAGE(PointMsg)
	gHUD.AddHudElem(this);

	mainWindow = SDL_GetWindowFromID(1);
	// mainContext = SDL_GL_CreateContext(mainWindow);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplOpenGL2_Init();
	ImGui_ImplSDL2_InitForOpenGL(mainWindow, ImGui::GetCurrentContext());

	return true;
}

bool CPointMessageRenderer::MsgFunc_PointMsg(const char* pszName, int iSize, void* pbuf)
{
	CClientPointMessage local;

	BEGIN_READ(pbuf, iSize);
	local.origin.x = READ_COORD();
	local.origin.y = READ_COORD();
	local.origin.z = READ_COORD();
	local.message = std::string(READ_STRING());
	local.developerOnly = (bool)READ_BYTE();
	local.maxDistance = READ_FLOAT();

	// add to our list
	MessageList.push_back(local);

	return true;
}

void CPointMessageRenderer::Reset()
{
	
}

bool CPointMessageRenderer::VidInit()
{
	// reset list
	MessageList.clear();

	return true;
}

bool CPointMessageRenderer::Draw(float flTime)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(mainWindow);
	ImGui::NewFrame();

	// draw stuffs here 
	for (int i = 0; i < MessageList.size(); i++)
	{
		if (!MessageList[i].message.empty())
		{
			// specify length
			int len = MessageList[i].message.length();
			len *= 10;
			ImGui::SetNextWindowSize(ImVec2(len, 25));

			// World2Screen conversion
			float screen[2];
			gEngfuncs.pTriAPI->WorldToScreen(MessageList[i].origin, screen);
			int x = XPROJECT(screen[0]);
			int y = YPROJECT(screen[1]);
			ImGui::SetNextWindowPos(ImVec2(x - (len/2), y - (25/2)));

			// check player rotation
			Vector vecDir = MessageList[i].origin - gEngfuncs.GetLocalPlayer()->curstate.origin;
			vecDir.z = 0;
			Vector forward, angle;
			angle = gEngfuncs.GetLocalPlayer()->curstate.angles;
			angle.x = 0;
			AngleVectors(angle, forward, null, null);

			// check if the coords are simply out of the screen coord
			if (((((x - (len / 2)) > 0) && ((x - (len / 2)) < ScreenWidth)) && ((y > 0) && (y < ScreenHeight)))
				&& DotProduct(forward, vecDir) > 90										// or player is facing different way,
				&& vecDir.Length() < MessageList[i].maxDistance							// or way too far (out of radius)
				&& !(CVAR_GET_FLOAT("developer") == 0 && MessageList[i].developerOnly))	// or if its developer only
			{
				ImGui::Begin(std::to_string(i).c_str(), null, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
				ImGui::Text(MessageList[i].message.c_str());
				ImGui::End();
			}
		}
	}

	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

	return true;
}