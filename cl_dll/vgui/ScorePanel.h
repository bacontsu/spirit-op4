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

#include <VGUI_Label.h>
#include <VGUI_Panel.h>

#include "CLabelHeader.h"
#include "CommandButton.h"
#include "controls/vgui_defaultinputsignal.h"
#include "controls/vgui_grid.h"
#include "controls/vgui_listbox.h"

#define MAX_SCORES					10
#define MAX_SCOREBOARD_TEAMS		5

// Scoreboard cells
#define COLUMN_TRACKER	0
#define COLUMN_NAME		1
#define COLUMN_CLASS	2
#define COLUMN_KILLS	3
#define COLUMN_DEATHS	4
#define COLUMN_LATENCY	5
#define COLUMN_VOICE	6
#define COLUMN_BLANK	7
#define NUM_COLUMNS		8
#define NUM_ROWS		(MAX_PLAYERS + (MAX_SCOREBOARD_TEAMS * 2))

// Purpose: Scoreboard back panel
class ScorePanel : public vgui::Panel, public vgui::CDefaultInputSignal
{
private:
    // Default panel implementation doesn't forward mouse messages when there is no cursor and we need them.
    class HitTestPanel : public Panel
    {
    public:
        void internalMousePressed(vgui::MouseCode code) override;
    };

public:
    ScorePanel(int x, int y, int wide, int tall);

    void Update();
    void SortTeams();
    void SortPlayers(int iTeam, char* team);
    void RebuildTeams();
    void FillGrid();
    void DeathMsg(int killer, int victim);
    void Initialize();
    void Open();
    void MouseOverCell(int row, int col);

    // InputSignal overrides.
    void mousePressed(vgui::MouseCode code, Panel* panel) override;
    void cursorMoved(int x, int y, Panel* panel) override;

    friend class CLabelHeader;

private:
    vgui::Label m_TitleLabel;

    // Here is how these controls are arranged hierarchically.
    // m_HeaderGrid
    //     m_HeaderLabels

    // m_PlayerGridScroll
    //     m_PlayerGrid
    //         m_PlayerEntries 

    vgui::CGrid m_HeaderGrid;
    CLabelHeader m_HeaderLabels[NUM_COLUMNS]; // Labels above the 
    CLabelHeader* m_pCurrentHighlightLabel;
    int m_iHighlightRow;

    vgui::CListBox m_PlayerList;
    vgui::CGrid m_PlayerGrids[NUM_ROWS]; // The grid with player and team info. 
    CLabelHeader m_PlayerEntries[NUM_COLUMNS][NUM_ROWS]; // Labels for the grid entries.

    HitTestPanel m_HitTestPanel;
    CommandButton* m_pCloseButton;
    CLabelHeader* GetPlayerEntry(int x, int y) { return &m_PlayerEntries[x][y]; }

public:

    int m_iNumTeams;
    int m_iPlayerNum;
    int m_iShowscoresHeld;

    int m_iRows;
    int m_iSortedRows[NUM_ROWS];
    int m_iIsATeam[NUM_ROWS];
    bool m_bHasBeenSorted[MAX_PLAYERS];
    int m_iLastKilledBy;
    int m_fLastKillTime;
};
