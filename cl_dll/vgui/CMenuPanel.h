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

#include "CTransparentPanel.h"

// Menu Panel that supports buffering of menus
class CMenuPanel : public CTransparentPanel
{
public:
    CMenuPanel(int iRemoveMe, int x, int y, int wide, int tall);
    CMenuPanel(int iTrans, int iRemoveMe, int x, int y, int wide, int tall);

    virtual void Reset(void);
    void SetNextMenu(CMenuPanel* pNextPanel);
    void SetMenuID(int iID);
    void SetActive(int iState);
    virtual void Open(void);
    virtual void Close(void);

    int ShouldBeRemoved() const { return m_iRemoveMe; }
    CMenuPanel* GetNextMenu() const { return m_pNextMenu; }
    int GetMenuID() const { return m_iMenuID; }
    int IsActive() const { return m_iIsActive; }
    float GetOpenTime() const { return m_flOpenTime; }

    // Numeric input
    virtual bool SlotInput(int iSlot) { return false; }
    virtual void SetActiveInfo(int iInput) {}

private:
    CMenuPanel* m_pNextMenu;
    int m_iMenuID;
    int m_iRemoveMe;
    int m_iIsActive;
    float m_flOpenTime;
};
