
// WJDialogDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "WJDialog.h"
#include "WJDialogDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWJDialogDlg dialog



CWJDialogDlg::CWJDialogDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WJDIALOG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bDraging = FALSE;
}

void CWJDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWJDialogDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CWJDialogDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWJDialogDlg::OnBnClickedCancel)
	ON_WM_ENTERSIZEMOVE()
	ON_WM_EXITSIZEMOVE()
END_MESSAGE_MAP()


// CWJDialogDlg message handlers

BOOL CWJDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWJDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// 클라이언트 DC를 선언한다.
		CClientDC dc(this);
		// 클라이언트 DC로 사각형을 그린다.
		dc.Rectangle(10, 10, 100, 100);		
		
		//CDialogEx::OnPaint(); //http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=700
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWJDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWJDialogDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	// CDialogEx::OnOK();
}


void CWJDialogDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CWJDialogDlg::OnEnterSizeMove()
{
	if (!m_bDraging)
	{
		m_bDraging = TRUE;
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) ^ WS_EX_LAYERED);
		theApp.SetLayeredWindowAttributes(m_hWnd, 0, 176, LWA_ALPHA);
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	}
	//Invalidate(true);
	//CDialogEx::OnEnterSizeMove();
}


void CWJDialogDlg::OnExitSizeMove()
{
	if (m_bDraging)
	{
		m_bDraging = FALSE;
		::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);
		::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
	}
	//Invalidate(true);
	//CDialogEx::OnExitSizeMove();
}