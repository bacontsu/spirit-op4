//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================
#pragma once

#include "../../shared/Vector.h"

// Camera.h  --  defines and such for a 3rd person camera
// NOTE: must include quakedef.h first

// pitch, yaw, dist
extern vec3_t cam_ofs;
// Using third person camera
extern int cam_thirdperson;

void CAM_Init();
void CAM_ClearStates();
void CAM_StartMouseMove();
void CAM_EndMouseMove();
