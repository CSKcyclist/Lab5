// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2009, 2011-2012, 2016-2017, 2021, 2023 - TortoiseGit
// Copyright (C) 2003-2011, 2015 - TortoiseSVN

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

/**
 * \ingroup Utils
 * A wrapper class for the IProgressDialog interface. Thats
 * the dialog used by the shell/IE to show progress in e.g.
 * copying, downloading, ...
 *
 * \remark you need to call AfxOleInit() before using this class, preferably in
 * your app's InitInistance() method.
 */
class CSysProgressDlg
{
public:
	CSysProgressDlg();
	~CSysProgressDlg();

	/**
	 * sets the title of the progress dialog box.
	 * \param szTitle pointer to a nullptr-terminated string that contains the dialog box title
	 */
	void SetTitle(LPCWSTR szTitle);
	/**
	* sets the title of the progress dialog box to a string resource value.
	*/
	void SetTitle ( UINT idTitle);

	/**
	 * Displays a message.
	 * \param dwLine line number on which the text is to be displayed. There are three lines possible, two lines if SetCalculateTime() is set to true.
	 * \param szText nullptr-terminated string that contains the line text.
	 * \param bCompactPath set to true if you want the text to be compacted (if it is a path) to fit on the line.
	 *
	 * \remark This call should be made *after* the dialog has been shown - this allows
	 * the system to measure the space available for the text, and do path compaction properly
	 */
	void SetLine(DWORD dwLine, LPCWSTR szText, bool bCompactPath = false);

#ifdef _MFC_VER
	/**
	* Wrappers around set line, to do a CString::Format type operation
	* See SetLine for more details
	*
	* \remark These calls should be made *after* the dialog has been shown - this allows
	* the system to measure the space available for the text, and do path compaction properly
	*/
	void FormatPathLine ( DWORD dwLine, UINT idFormatText, ...);
	void FormatPathLine(DWORD dwLine, LPCWSTR FormatText, ...);
	void FormatNonPathLine ( DWORD dwLine, UINT idFormatText, ...);
	void FormatNonPathLine(DWORD dwLine, LPCWSTR FormatText, ...);
#endif
	/**
	 * Sets a message to be displayed if the user clicks the cancel button.
	 * \param szMessage pointer to a nullptr-terminated string that contains the message.
	 * \remark Even though the user clicks Cancel, the application cannot immediately
	 * call Stop() to close the dialog box. The application
	 * must wait until the next time it calls HasUserCancelled() to
	 * discover that the user has canceled the operation. Since this delay might be
	 * significant, the progress dialog box provides the user with immediate feedback
	 * by clearing text lines 1 and 2 and displaying the cancel message on line 3.
	 * The message is intended to let the user know that the delay is normal and that
	 * the progress dialog box will be closed shortly. It is typically is set to
	 * something like "Please wait while ...".
	 */
	void SetCancelMsg(LPCWSTR szMessage);
#ifdef _MFC_VER
	void SetCancelMsg ( UINT idMessage );
#endif

	/**
	 * Specifies that the progress dialog should have a line indicating the time remaining to complete.
	 * \param bCalculate false to deactivate the time remaining line.
	 */
	void SetTime ( bool bTime = true );

	/**
	 * Specifies if the progress bar should be shown or not.
	 */
	void SetShowProgressBar ( bool bShow = true );

	/**
	 * Resets the progress dialog box timer to zero.
	 * \remark the timer is used to estimate the remaining time. It is started when your
	 * application calls ShowModal() or ShowModeless(). Unless your application will
	 * start immediately, it should call ResetTimer() just before starting the operation.
	 * This practice ensures that the time estimates will be as accurate as possible.
	 * This method should not be called after the first call to UpdateProgress().
	 */
	void ResetTimer();

	/**
	 * Shows the progress dialog box modal.
	 */
#ifdef _MFC_VER
	HRESULT ShowModal ( CWnd* pwndParent, BOOL immediately = true );
#endif
	HRESULT ShowModal ( HWND hWndParent, BOOL immediately = true );
	/**
	 * Shows the progress dialog box modeless.
	 */
#ifdef _MFC_VER
	HRESULT ShowModeless ( CWnd* pwndParent, BOOL immediately = true );
#endif
	HRESULT ShowModeless ( HWND hWndParent, BOOL immediately = true );

	/**
	 * Stops the progress dialog box and removes it from the screen.
	 */
	void Stop();

	/**
	 * Updates the progress dialog box with the current state of the operation.
	 * \param dwProgress Application-defined value that indicates what proportion of the operation has been completed at the time the method was called
	 * \param dwMax Application-defined value that specifies what value dwCompleted will have when the operation is complete
	 */
	void SetProgress ( DWORD dwProgress, DWORD dwMax );
	/**
	 * Updates the progress dialog box with the current state of the operation.
	 * \param u64Progress Application-defined value that indicates what proportion of the operation has been completed at the time the method was called
	 * \param u64ProgressMax Application-defined value that specifies what value dwCompleted will have when the operation is complete
	 */
	void SetProgress64 ( ULONGLONG u64Progress, ULONGLONG u64ProgressMax );

	/**
	 * Checks whether the user has canceled the operation.
	 */
	bool HasUserCancelled();

	/**
	 * Checks whether this object was created successfully. If the return value is false then
	 * you MUST NOT use the current instance of this class.
	 */
	bool IsValid() const { return m_pIDlg != nullptr; }

	/**
	 * Checks whether the window is shown.
	 */
	bool IsVisible() const { return m_isVisible; }

	/**
	 * After a call to Stop() to hide the progress dialog,
	 * call EnsureValid() to recreate the dialog and fill in the
	 * data again.
	 */
	bool EnsureValid();

protected:
	ATL::CComPtr<IProgressDialog> m_pIDlg;
	bool				m_isVisible = false;
	DWORD				m_dwDlgFlags = PROGDLG_NORMAL;
	HWND m_hWndProgDlg = nullptr;
	HWND m_hWndParent = nullptr;
	HWND m_hWndFocus = nullptr;
};
