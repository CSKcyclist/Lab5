﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2009, 2015-2016, 2018-2019, 2021-2022 - TortoiseGit
// Copyright (C) 2008-2009 - TortoiseSVN

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
#include "stdafx.h"
#include "ConflictEditorCommand.h"
#include "MessageBox.h"
#include "AppUtils.h"

bool ConflictEditorCommand::Execute()
{
	CTGitPath repo{ g_Git.m_CurrentDir };
	if (!repo.HasAdminDir())
	{
		CMessageBox::Show(GetExplorerHWND(), IDS_NOWORKINGCOPY, IDS_APPNAME, MB_ICONERROR);
		return false;
	}

	bool bAlternativeTool = !!parser.HasKey(L"alternative");
	return CAppUtils::ConflictEdit(GetExplorerHWND(), this->cmdLinePath, bAlternativeTool, repo.IsRebaseActive());
}
