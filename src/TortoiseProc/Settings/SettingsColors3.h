// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2003-2008 - TortoiseSVN

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
class CSettingsColors3 : public ISettingsPropPage
{
	DECLARE_DYNAMIC(CSettingsColors3)

public:
	CSettingsColors3();
	virtual ~CSettingsColors3();

	UINT GetIconID() override { return IDI_LOOKANDFEEL; }

	enum { IDD = IDD_SETTINGSCOLORS_3 };

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	BOOL OnInitDialog() override;
	afx_msg void OnBnClickedColor();
	afx_msg void OnBnClickedRestore();
	afx_msg void OnCbnSelchangeLoggraphlinewidth();
	BOOL OnApply() override;

	DECLARE_MESSAGE_MAP()

private:
	CMFCColorButton	m_cLine[8];
	CColors			m_Colors;
	CComboBox		m_LogGraphLineWidth;
	CComboBox		m_LogGraphNodeSize;
	CRegDWORD		m_regLogGraphLineWidth;
	CRegDWORD		m_regLogGraphNodeSize;
};
