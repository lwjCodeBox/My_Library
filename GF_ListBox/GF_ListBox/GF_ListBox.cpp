
// GF_ListBox.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "GF_ListBox.h"
#include "GF_ListBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGFListBoxApp

BEGIN_MESSAGE_MAP(CGFListBoxApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGFListBoxApp construction

CGFListBoxApp::CGFListBoxApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CGFListBoxApp object

CGFListBoxApp theApp;


// CGFListBoxApp initialization

BOOL CGFListBoxApp::InitInstance()
{
	CWinApp::InitInstance();	

	CGFListBoxDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	return FALSE;
}