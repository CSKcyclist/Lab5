﻿// TortoiseGitMerge - a Diff/Patch program

// Copyright (C) 2023 - TortoiseGit
// Copyright (C) 2007-2011, 2013-2014 - TortoiseSVN

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
#include "DiffStates.h"
#include "EOL.h"

#include <vector>

enum class HideState
{
	Shown,
	Hidden,
	Marker,
};

/**
 * \ingroup TortoiseMerge
 * Holds the information which is required to define a single line of text.
 */
class viewdata
{
public:
	viewdata() = default;

	viewdata(
			const CString& sLineInit,
			DiffState stateInit,
			int linenumberInit,
			EOL endingInit,
			HideState hideInit,
			bool markedInit = false)
		: state(stateInit)
		, linenumber(linenumberInit)
		, ending(endingInit)
		, hidestate(hideInit)
		, marked(markedInit)
		, sLine(sLineInit)
	{
	}

	CString			sLine;
	DiffState		state = DiffState::Unknown;
	int				linenumber = -1;
	int				movedIndex = -1;
	bool			movedFrom = true;
	EOL				ending = EOL::AutoLine;
	HideState		hidestate = HideState::Hidden;
	bool			marked = false;
};

/**
 * \ingroup TortoiseMerge
 * Handles the view and diff data a TortoiseMerge view needs.
 */
class CViewData
{
public:
	CViewData();
	~CViewData();

	void			AddData(const CString& sLine, DiffState state, int linenumber, EOL ending, HideState hide, int movedline);
	void			AddData(const viewdata& data);
	void			AddEmpty() {AddData(CString(), DiffState::Empty, -1, EOL::NoEnding, HideState::Shown, -1);}
	void			InsertData(int index, const CString& sLine, DiffState state, int linenumber, EOL ending, HideState hide, int movedline);
	void			InsertData(int index, const viewdata& data);
	void			RemoveData(int index) {m_data.erase(m_data.begin() + index);}

	const viewdata&	GetData(int index) const {return m_data[index];}
	const CString&	GetLine(int index) const {return m_data[index].sLine;}
	DiffState		GetState(int index) const {return m_data[index].state;}
	HideState		GetHideState(int index) const {return m_data[index].hidestate;}
	int				GetLineNumber(int index) const {return m_data.size() ? m_data[index].linenumber : 0;}
	int				GetMovedIndex(int index) const {return m_data.size() ? m_data[index].movedIndex: 0;}
	bool			IsMoved(int index) const {return m_data.size() ? m_data[index].movedIndex >= 0 : false;}
	bool			IsMovedFrom(int index) const {return m_data.size() ? m_data[index].movedFrom : true;}
	int				FindLineNumber(int number) const;
	EOL				GetLineEnding(int index) const {return m_data[index].ending;}
	bool			GetMarked(int index) const {return m_data[index].marked;}

	int				GetCount() const { return static_cast<int>(m_data.size()); }

	void			SetData(int index, const viewdata& data)
	{
		bool oldmarked = m_data[index].marked;
		bool marked = data.marked;
		if (oldmarked && !marked && m_nMarkedBlocks > 0)
			m_nMarkedBlocks--;
		else if (!oldmarked && marked)
			m_nMarkedBlocks++;
		m_data[index] = data;
	}
	void			SetState(int index, DiffState state) {m_data[index].state = state;}
	void			SetLine(int index, const CString& sLine) {m_data[index].sLine = sLine;}
	void			SetLineNumber(int index, int linenumber) {m_data[index].linenumber = linenumber;}
	void			SetLineEnding(int index, EOL ending) {m_data[index].ending = ending;}
	void			SetMovedIndex(int index, int movedIndex, bool movedFrom) {m_data[index].movedIndex = movedIndex; m_data[index].movedFrom = movedFrom;}
	void			SetLineHideState(int index, HideState state) {m_data[index].hidestate = state;}
	void			SetMarked(int index, bool marked)
	{
		bool oldmarked = m_data[index].marked;
		if (oldmarked && !marked && m_nMarkedBlocks > 0)
			m_nMarkedBlocks--;
		else if (!oldmarked && marked)
			m_nMarkedBlocks++;
		m_data[index].marked = marked;
	}
	bool			HasMarkedBlocks() const { return m_nMarkedBlocks > 0; }

	void			Clear() { m_data.clear(); m_nMarkedBlocks = 0; }
	void			Reserve(int length) {m_data.reserve(length);}

protected:
	std::vector<viewdata>		m_data;
	int							m_nMarkedBlocks = 0;
};
