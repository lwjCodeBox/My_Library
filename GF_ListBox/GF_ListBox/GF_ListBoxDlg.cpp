
// GF_ListBoxDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "GF_ListBox.h"
#include "GF_ListBoxDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGFListBoxDlg dialog



CGFListBoxDlg::CGFListBoxDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GF_LISTBOX_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGFListBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EVENT_LIST, m_event_list);
}

BEGIN_MESSAGE_MAP(CGFListBoxDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGFListBoxDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGFListBoxDlg::OnBnClickedCancel)
	ON_WM_DESTROY()

	//ON_LBN_SELCHANGE(IDC_EVENT_LIST, &CGFListBoxDlg::OnSelectChanged)
	ON_LBN_DBLCLK(IDC_EVENT_LIST, &CGFListBoxDlg::OnLbnDblclkEventList)

	ON_BN_CLICKED(IDC_LB_TOTAL_COPY, &CGFListBoxDlg::OnBnClickedLbTotalCopy)
END_MESSAGE_MAP()


// CGFListBoxDlg message handlers

BOOL CGFListBoxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Selection -> Extented
	wchar_t ws[32];
	memset(ws, 0, 32 << 1);
	
	for (int i = 0; i < 10; i++) {
		swprintf_s(ws, L"%d. TEST", i);
		m_event_list.InsertString(i, ws);
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGFListBoxDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGFListBoxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGFListBoxDlg::OnBnClickedOk() { /*CDialogEx::OnOK();*/ }
void CGFListBoxDlg::OnBnClickedCancel() { CDialogEx::OnCancel(); }


void CGFListBoxDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}

BOOL CGFListBoxDlg::PreTranslateMessage(MSG *pMsg)
{
	// ����Ʈ �ڽ��� ��Ŀ�� ������ ��
	if (pMsg->hwnd == m_event_list.m_hWnd)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			if (::GetKeyState(VK_CONTROL) < 0 && (pMsg->wParam == 'c' || pMsg->wParam == 'C'))
				OnSelectChanged();
			else if (::GetKeyState(VK_CONTROL) < 0 && (pMsg->wParam == 'a' || pMsg->wParam == 'A')) {
				int count = m_event_list.GetCount();
				m_event_list.SelItemRange(TRUE, 0, count);
				OnSelectChanged();
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

int CGFListBoxDlg::CopyTextToClipboard(const char *ap_string)
{
	// ������ ���ڿ��� ���̸� ���Ѵ�. ('\0'���� ������ ũ��)
	int string_length = strlen(ap_string) + 1;
	// Ŭ������� ���ڿ��� �����ϱ� ���Ͽ� �޸𸮸� �Ҵ��Ѵ�.
	// Ŭ�����忡�� �ڵ��� �ִ� �����̶� HeapAlloc �Լ� ����� �����ϴ�.
	HANDLE h_data = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, string_length);
	// �Ҵ�� �޸𸮿� ���ڿ��� �����ϱ� ���ؼ� ��� ������ �ּҸ� ��´�.
	char *p_data = (char *)::GlobalLock(h_data);
	if (NULL != p_data) {
		// �Ҵ�� �޸� ������ ������ ���ڿ��� �����Ѵ�.
		memcpy(p_data, ap_string, string_length);
		// ���ڿ��� �����ϱ� ���ؼ� Lock �ߴ� �޸𸮸� �����Ѵ�.
		::GlobalUnlock(h_data);
		if (::OpenClipboard(m_hWnd)) {           // Ŭ�����带 ����.
			::EmptyClipboard();                  // Ŭ�����忡 ����� ���� ���ڿ��� �����Ѵ�.
			::SetClipboardData(CF_TEXT, h_data); // Ŭ������� ���ڿ��� �����Ѵ�.
			::CloseClipboard();                  // Ŭ�����带 �ݴ´�.
		}
	}
	return 0;
}

void CGFListBoxDlg::OnSelectChanged()
{
	// ������ �� Ŭ�����忡 �����ϴ� �� ���� ���.
#if(0) // https://tapito.tistory.com/577
	CStringA ansi_str; // ASCII �������� ���ڿ��� �����ϱ� ���ؼ� ���
	CString str, total_str;

	/* ����ڰ� ������ ���������� index ������ �� �� */
	DWORD dwSelection = LB_ERR;
	HANDLE hSelection = NULL;
	LPINT rgSelection = NULL;

	/* ����ڰ� ������ �� ���� �����ߴ��� Ȯ�� */
	dwSelection = m_event_list.GetSelCount();

	if (dwSelection == LB_ERR) return;
	else if (dwSelection == 0)
	{
		/* �ƹ��͵� �������� �ʾ��� ��� */
		MessageBox(L"���������� �ϳ� �̻� �����ϼ���.", L"Error", 1);
	}
	else
	{
		/* �ϳ� �̻� �������� ���, �� ������ŭ ���� �Ҵ� */
		VERIFY((hSelection = LocalAlloc(LMEM_MOVEABLE | LMEM_ZEROINIT, dwSelection * sizeof(INT))) != NULL);

		if ((rgSelection = (LPINT)LocalLock(hSelection)) != NULL)
		{
			/* ��ü������ �� �� ���������� �����ߴ��� �������� */
			VERIFY(m_event_list.GetSelItems(dwSelection, rgSelection) == dwSelection);

			/* ����ڰ� ������ �� �������� �ο��� ���� ���� ������ ���ϱ� */
			for (DWORD dwIndex = 0; dwIndex < dwSelection; dwIndex++) {
				// ����Ʈ �ڽ��� �� �׸��� �����´�.
				m_event_list.GetText(rgSelection[dwIndex], str);
				// �� �׸��� ���ڿ��� �ϳ��� ���ڿ��� ��ģ��.
				total_str += str;
				// �� URL�� �����ϱ� ���ؼ� ���๮�ڸ� �߰��Ѵ�.
				total_str += L"\r\n";

				// total_str�� ������ URL ���ڿ��� �����ڵ� �����̱� ������
				// ASCII �������� �����ؼ� ansi_str�� �ֽ��ϴ�.
				ansi_str = total_str;
			}
		}
	}

	LocalUnlock(hSelection);
	LocalFree(hSelection);

	CopyTextToClipboard(ansi_str);
#else
	CStringA ansi_str; // ASCII �������� ���ڿ��� �����ϱ� ���ؼ� ���
	CString str, total_str;
	// ����Ʈ�ڽ��� ��ϵ� �׸��� ������ ��´�. 
	int count = m_event_list.GetCount();

	// �׸��� �̵���Ű�� ���ؼ� ���õ� �׸��� ������ ��, �����׸��� �����ؾ� 
	// �ϴµ� ���ʿ��� ����� �׸��� ��ġ�� ����Ǹ鼭 �Ʒ��� �����׸��� üũ�ϴµ� 
	// ������ �߻��մϴ�. ���� ����Ʈ�ڽ��� �׸��� ������ �Ʒ��� ���鼭 �����ϴ� ���� 
	// �ƴ϶� �Ʒ��ʿ��� �������� �ö���鼭 �����Ͽ� �߰��� �׸��� �����ϴ��� 
	// ��ġ���� ������ �߻����� �ʵ��� �մϴ�. 
	for (int i = 0; i < count - 1; i++) {

		// ���� �׸��� ���õǾ��ִ��� üũ�մϴ�. 
		// GetCurSel�� Ȱ��ȭ�� üũ �׸� ã���� ����մϴ�. 
		if (m_event_list.GetSel(i)) {

			// ���õǾ��ִ� �׸��� ���ڿ��� ����ϴ�. 
			m_event_list.GetText(i, str);

			// �� �׸��� ���ڿ��� �ϳ��� ���ڿ��� ��ģ��.
			total_str += str;
			// �� ������ �����ϱ� ���ؼ� ���๮�ڸ� �߰��Ѵ�.
			total_str += L"\r\n";

			// total_str�� ������ URL ���ڿ��� �����ڵ� �����̱� ������
			// ASCII �������� �����ؼ� ansi_str�� �ֽ��ϴ�.
			ansi_str = total_str;
		}
	}
	CopyTextToClipboard(ansi_str);
#endif
}

void CGFListBoxDlg::OnLbnDblclkEventList()
{
	CString str;
	int i = m_event_list.GetCurSel();

	m_event_list.GetText(i, str);
	MessageBox(str, L"Exam", 1);
}

void CGFListBoxDlg::OnBnClickedLbTotalCopy()
{
	if (m_event_list.GetCurSel() != LB_ERR) // LB_ERR = -1�� �ǹ̴� � ���ڿ��� ���õ��� ���� ����
	{
		CStringA ansi_str; // ASCII �������� ���ڿ��� �����ϱ� ���ؼ� ���
		CString str, total_str;
		// ����Ʈ �ڽ��� �߰��� �׸��� ������ ��´�.
		int count = m_event_list.GetCount();
		for (int i = 0; i < count; i++) {
			// ����Ʈ �ڽ��� �� �׸��� �����´�.
			m_event_list.GetText(i, str);
			// �� �׸��� ���ڿ��� �ϳ��� ���ڿ��� ��ģ��.
			total_str += str;
			// �� URL�� �����ϱ� ���ؼ� ���๮�ڸ� �߰��Ѵ�.
			total_str += L"\r\n";
		}
		// total_str�� ������ URL ���ڿ��� �����ڵ� �����̱� ������
		// ASCII �������� �����ؼ� ansi_str�� �ֽ��ϴ�.
		ansi_str = total_str;

		// �ڽ��� Ŭ�����忡 ���ڿ��� �־ WM_DRAWCLIPBOARD �޽����� �߻��ؼ�
		// ���� �߰��ϴ� Ŭ������ ������ ����Ʈ �ڽ��� �߰��Ǳ� ������ 
		// ansi_str�� ����� URL ����� Ŭ�����忡 �������ش�.
		CopyTextToClipboard(ansi_str);
	}
}


