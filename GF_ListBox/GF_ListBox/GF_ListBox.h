
// GF_ListBox.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CGFListBoxApp:
// See GF_ListBox.cpp for the implementation of this class
//

class CGFListBoxApp : public CWinApp
{
public:
	CGFListBoxApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()		
};

extern CGFListBoxApp theApp;
