﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2003-2008 - TortoiseSVN
// Copyright (C) 2009, 2011-2013, 2017, 2020, 2023 - TortoiseGit

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#pragma once
#include "SettingsPropPage.h"
#include "Colors.h"

/**
 * \ingroup TortoiseProc
 * Settings property page to set custom colors used in TortoiseSVN
 */
class CSettingsColors : public ISettingsPropPage
{
	DECLARE_DYNAMIC(CSettingsColors)

public:
	CSettingsColors();
	virtual ~CSettingsColors();

	UINT GetIconID() override { return IDI_LOOKANDFEEL; }

	enum { IDD = IDD_SETTINGSCOLORS_1 };

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	BOOL OnInitDialog() override;
	afx_msg void OnBnClickedColor();
	afx_msg void OnBnClickedTheme();
	afx_msg void OnBnClickedRestore();
	BOOL OnApply() override;

	DECLARE_MESSAGE_MAP()

private:
	CMFCColorButton	m_cConflict;
	CMFCColorButton	m_cAdded;
	CMFCColorButton	m_cDeleted;
	CMFCColorButton	m_cMerged;
	CMFCColorButton	m_cModified;
	CMFCColorButton	m_cNoteNode;
	CMFCColorButton m_cOtherRefs;
	CMFCColorButton	m_cRenamed;
	CColors			m_Colors;
	CButton			m_RevGraphUseLocalForCur;
	CRegDWORD		m_regRevGraphUseLocalForCur;
	CRegDWORD		m_regUseDarkMode;
	CButton			m_chkUseDarkMode;
};
