﻿// TortoiseGitMerge - a Diff/Patch program

// Copyright (C) 2023 - TortoiseGit
// Copyright (C) 2006-2008, 2010-2014, 2020 - TortoiseSVN

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

#pragma warning(push)
#include "svn_diff.h"
#include "apr_pools.h"
#pragma warning(pop)
#include "FileTextLines.h"
#include "registry.h"
#include "WorkingFile.h"
#include "ViewData.h"
#include "MovedBlocks.h"

#define DIFF_EMPTYLINENUMBER						(static_cast<DWORD>(-1))

enum class IgnoreWS : int
{
	None = 0,
	AllWhiteSpaces = 1,
	WhiteSpaces = 2, // whitespaces at the beginning of a line
};

/**
 * \ingroup TortoiseMerge
 * Main class for handling diffs.
 */
class CDiffData
{
public:
	CDiffData();
	virtual ~CDiffData();


	BOOL						Load();
	void						SetBlame(bool bBlame = true) {m_bBlame = bBlame;}
	void						SetMovedBlocks(bool bViewMovedBlocks = true);
	int							GetLineCount() const;
	CString						GetError() const  {return m_sError;}
	void						SetCommentTokens(const CString& sLineStart, const CString& sBlockStart, const CString& sBlockEnd);
	void						SetRegexTokens(const std::wregex& rx, const std::wstring& replacement);

	bool	IsBaseFileInUse() const		{ return m_baseFile.InUse(); }
	bool	IsTheirFileInUse() const	{ return m_theirFile.InUse(); }
	bool	IsYourFileInUse() const		{ return m_yourFile.InUse(); }

private:
	bool DoTwoWayDiff(const CString& sBaseFilename, const CString& sYourFilename, IgnoreWS ignoreWs, bool bIgnoreEOL, bool bIgnoreCase, bool bIgnoreComments, apr_pool_t* pool);

	void StickAndSkip(svn_diff_t * &tempdiff, apr_off_t &original_length_sticked, apr_off_t &modified_length_sticked) const;
	bool DoThreeWayDiff(const CString& sBaseFilename, const CString& sYourFilename, const CString& sTheirFilename, IgnoreWS ignoreWs, bool bIgnoreEOL, bool bIgnoreCase, bool bIgnoreComments, apr_pool_t* pool);
	/**
* Moved blocks detection for further highlighting,
* implemented exclusively for TwoWayDiff
**/
	tsvn_svn_diff_t_extension* MovedBlocksDetect(svn_diff_t* diffYourBase, IgnoreWS ignoreWs, apr_pool_t* pool);

	void TieMovedBlocks(int from, int to, apr_off_t length);

	void HideUnchangedSections(CViewData * data1, CViewData * data2, CViewData * data3) const;

	svn_diff_file_ignore_space_t GetIgnoreSpaceMode(IgnoreWS ignoreWs) const;
	svn_diff_file_options_t* CreateDiffFileOptions(IgnoreWS ignoreWs, bool bIgnoreEOL, apr_pool_t* pool);
	bool HandleSvnError(svn_error_t * svnerr);
	bool CompareWithIgnoreWS(CString s1, CString s2, IgnoreWS ignoreWs) const;

public:
	CWorkingFile				m_baseFile;
	CWorkingFile				m_theirFile;
	CWorkingFile				m_yourFile;
	CWorkingFile				m_mergedFile;

	CString						m_sDiffFile;
	CString						m_sPatchPath;
	CString						m_sPatchOriginal;
	CString						m_sPatchPatched;
	bool						m_bPatchRequired = false;

public:
	CFileTextLines				m_arBaseFile;
	CFileTextLines				m_arTheirFile;
	CFileTextLines				m_arYourFile;

	CViewData					m_YourBaseBoth;				///< one-pane view, diff between 'yours' and 'base' (in three-pane view: right view)
	CViewData					m_YourBaseLeft;				///< two-pane view, diff between 'yours' and 'base', left view
	CViewData					m_YourBaseRight;			///< two-pane view, diff between 'yours' and 'base', right view

	CViewData					m_TheirBaseBoth;			///< one-pane view, diff between 'theirs' and 'base' (in three-pane view: left view)
	CViewData					m_TheirBaseLeft;			///< two-pane view, diff between 'theirs' and 'base', left view
	CViewData					m_TheirBaseRight;			///< two-pane view, diff between 'theirs' and 'base', right view

	CViewData					m_Diff3;					///< three-pane view, bottom pane

	CString						m_sError;

	static int					abort_on_pool_failure (int retcode);
protected:
	bool						m_bBlame = false;
	bool						m_bViewMovedBlocks = false;
	CString						m_CommentLineStart;
	CString						m_CommentBlockStart;
	CString						m_CommentBlockEnd;
	std::wregex					m_rx;
	std::wstring				m_replacement;
};
