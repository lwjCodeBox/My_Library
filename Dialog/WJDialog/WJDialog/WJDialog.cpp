
// WJDialog.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "WJDialog.h"
#include "WJDialogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWJDialogApp

BEGIN_MESSAGE_MAP(CWJDialogApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CWJDialogApp construction

CWJDialogApp::CWJDialogApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CWJDialogApp object

CWJDialogApp theApp;


// CWJDialogApp initialization

BOOL CWJDialogApp::InitInstance()
{
	CWinApp::InitInstance();

	// get the function pointer for SetLayeredWindowAttributes in User32.dll
	HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
	SetLayeredWindowAttributes = (lpfn)GetProcAddress(hUser32, "SetLayeredWindowAttributes");
	if (SetLayeredWindowAttributes == NULL)
		AfxMessageBox(L"Layering is not supported in this version of Windows", MB_ICONEXCLAMATION);

	CWJDialogDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	return FALSE;
}

