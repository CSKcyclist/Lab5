﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2020, 2023 - TortoiseGit
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
#include "StandAloneDlg.h"

/**
 * \ingroup TortoiseProc
 * Helper dialog to get the name of a change list.
 */
class CCreateChangelistDlg : public CStandAloneDialog
{
	DECLARE_DYNAMIC(CCreateChangelistDlg)

public:
	CCreateChangelistDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCreateChangelistDlg();

	enum { IDD = IDD_CREATECHANGELIST };

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	BOOL OnInitDialog() override;
	afx_msg void OnEnChangeName();

	DECLARE_MESSAGE_MAP()
public:
	CString m_sName;
};
