// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2010-2011, 2013, 2015, 2023 - TortoiseGit

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

#pragma once

#include "StandAloneDlg.h"

// CCreateRepoDlg dialog

class CCreateRepoDlg : public CStandAloneDialog
{
	DECLARE_DYNCREATE(CCreateRepoDlg)

public:
	CCreateRepoDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCreateRepoDlg();

// Dialog Data
	enum { IDD = IDD_CREATEREPO};

protected:
	// Overrides
	void OnOK() override;
	void OnCancel() override;

	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	BOOL OnInitDialog() override;
	CString m_ModuleName;

	DECLARE_MESSAGE_MAP()

public:
	BOOL	m_bBare;
	CString	m_folder;

protected:
	afx_msg void OnBnClickedCheckBare();
};
