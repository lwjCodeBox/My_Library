
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

	CWJDialogDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	return FALSE;
}

