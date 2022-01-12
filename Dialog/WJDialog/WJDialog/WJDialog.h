
// WJDialog.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CWJDialogApp:
// See WJDialog.cpp for the implementation of this class
//
typedef BOOL(WINAPI *lpfn) (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);
class CWJDialogApp : public CWinApp
{
public:
	lpfn			SetLayeredWindowAttributes;

public:	
	CWJDialogApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CWJDialogApp theApp;
