﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2023 - TortoiseGit
// Copyright (C) 2020-2021 - TortoiseSVN

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
#include "registry.h"
#include <unordered_map>
#include <functional>
#include <optional>
#pragma warning(push)
#pragma warning(disable : 4458) // declaration of 'xxx' hides class member
#include <gdiplus.h>
#pragma warning(pop)

using ThemeChangeCallback = std::function<void()>;

/**
 * Singleton to handle Theme related methods.
 * provides callbacks to allow different parts of the app
 * to receive notifications when the theme changes.
 */
class CTheme
{
private:
	CTheme();
	~CTheme();

public:
	static CTheme& Instance();

	// cf. src/TortoiseUDiff/UDiffColors.h, src/SshAskPass/SshAskPass.cpp and src/TortoiseGitBlame/BlameIndexColors.h
	static const COLORREF darkBkColor = 0x202020;
	static const COLORREF darkBkHotColor = 0x404040;
	static const COLORREF darkTextColor = 0xDDDDDD;
	static const COLORREF darkDisabledTextColor = 0x808080;

	/// call this on every WM_SYSCOLORCHANGED message
	void OnSysColorChanged();
	/// returns true if dark mode is even allowed. We only allow dark mode on Win10 1809 or later.
	bool IsDarkModeAllowed();
	/// sets the theme and calls all registered callbacks.
	/// if \force is true, all Callback functions are called whether the mode changed or not
	void SetDarkTheme(bool b = true, bool force = false);
	/// returns true if dark theme is enabled. If false, then the normal theme is active.
	bool IsDarkTheme() const;
	/// returns true if high contrast mode is on
	bool IsHighContrastMode() const;
	/// returns true if high contrast mode is on, and the color scheme is dark
	bool IsHighContrastModeDark() const;
	/// converts a color to the theme color. For normal theme the \b clr is returned unchanged.
	/// for dark theme, the color is adjusted in brightness.
	COLORREF GetThemeColor(COLORREF clr, bool fixed = false) const;
	Gdiplus::Color GetThemeColor(Gdiplus::Color clr, bool fixed = false) const;
	/// registers a callback function that's called for every theme change.
	/// returns an id that can be used to unregister the callback function.
	int RegisterThemeChangeCallback(ThemeChangeCallback&& cb);
	/// unregisters a callback function.
	bool RemoveRegisteredCallback(int id);

	/// sets the theme for a whole dialog. For dark mode, the
	/// windows are subclassed if necessary. For normal mode,
	/// subclassing is removed to ensure the behavior is
	/// identical to the original.
	bool SetThemeForDialog(HWND hWnd, bool bDark);

	static void RGBToHSB(COLORREF rgb, BYTE& hue, BYTE& saturation, BYTE& brightness);
	static void RGBtoHSL(COLORREF color, float& h, float& s, float& l);
	static COLORREF HSLtoRGB(float h, float s, float l);

	static std::optional<LRESULT> HandleMenuBar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
	void Load();
	static BOOL CALLBACK AdjustThemeForChildrenProc(HWND hwnd, LPARAM lParam);
	static LRESULT CALLBACK ListViewSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static LRESULT CALLBACK ComboBoxSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static LRESULT CALLBACK MainSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static LRESULT CALLBACK ButtonSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	static LRESULT CALLBACK AutoSuggestSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);

private:
	bool m_bLoaded = false;
	bool m_dark = false;
	bool m_isHighContrastMode = false;
	bool m_isHighContrastModeDark = false;
	bool m_bDarkModeIsAllowed = false;
	std::unordered_map<int, ThemeChangeCallback> m_themeChangeCallbacks;
	int m_lastThemeChangeCallbackId = 0;
	CRegStdDWORD m_regDarkTheme;
	static HBRUSH s_backBrush;
	static HBRUSH s_backHotBrush;
};
