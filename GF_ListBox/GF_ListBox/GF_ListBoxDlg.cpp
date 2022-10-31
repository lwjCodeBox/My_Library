
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
	// 리스트 박스에 포커스 잡혔을 떄
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
	// 저장할 문자열의 길이를 구한다. ('\0'까지 포함한 크기)
	int string_length = strlen(ap_string) + 1;
	// 클리보드로 문자열을 복사하기 위하여 메모리를 할당한다.
	// 클립보드에는 핸들을 넣는 형식이라서 HeapAlloc 함수 사용이 블가능하다.
	HANDLE h_data = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, string_length);
	// 할당된 메모리에 문자열을 복사하기 위해서 사용 가능한 주소를 얻는다.
	char *p_data = (char *)::GlobalLock(h_data);
	if (NULL != p_data) {
		// 할당된 메모리 영역에 삽입할 문자열을 복사한다.
		memcpy(p_data, ap_string, string_length);
		// 문자열을 복사하기 위해서 Lock 했던 메모리를 해제한다.
		::GlobalUnlock(h_data);
		if (::OpenClipboard(m_hWnd)) {           // 클립보드를 연다.
			::EmptyClipboard();                  // 클립보드에 저장된 기존 문자열을 삭제한다.
			::SetClipboardData(CF_TEXT, h_data); // 클립보드로 문자열을 복사한다.
			::CloseClipboard();                  // 클립보드를 닫는다.
		}
	}
	return 0;
}

void CGFListBoxDlg::OnSelectChanged()
{
	// 선택한 셀 클립보드에 복사하는 두 가지 방법.
#if(0) // https://tapito.tistory.com/577
	CStringA ansi_str; // ASCII 형식으로 문자열을 변경하기 위해서 사용
	CString str, total_str;

	/* 사용자가 선택한 선택지들의 index 개수와 각 값 */
	DWORD dwSelection = LB_ERR;
	HANDLE hSelection = NULL;
	LPINT rgSelection = NULL;

	/* 사용자가 선택지 몇 개를 선택했는지 확인 */
	dwSelection = m_event_list.GetSelCount();

	if (dwSelection == LB_ERR) return;
	else if (dwSelection == 0)
	{
		/* 아무것도 선택하지 않았을 경우 */
		MessageBox(L"선택지에서 하나 이상 선택하세요.", L"Error", 1);
	}
	else
	{
		/* 하나 이상 선택했을 경우, 그 개수만큼 동적 할당 */
		VERIFY((hSelection = LocalAlloc(LMEM_MOVEABLE | LMEM_ZEROINIT, dwSelection * sizeof(INT))) != NULL);

		if ((rgSelection = (LPINT)LocalLock(hSelection)) != NULL)
		{
			/* 구체적으로 몇 번 선택지들을 선택했는지 가져오기 */
			VERIFY(m_event_list.GetSelItems(dwSelection, rgSelection) == dwSelection);

			/* 사용자가 선택한 각 선택지에 부여된 정수 값을 가져와 더하기 */
			for (DWORD dwIndex = 0; dwIndex < dwSelection; dwIndex++) {
				// 리스트 박스의 각 항목을 가져온다.
				m_event_list.GetText(rgSelection[dwIndex], str);
				// 각 항목의 문자열을 하나의 문자열로 합친다.
				total_str += str;
				// 각 URL을 구분하기 위해서 개행문자를 추가한다.
				total_str += L"\r\n";

				// total_str에 합쳐진 URL 문자열은 유니코드 형식이기 때문에
				// ASCII 형식으로 변경해서 ansi_str에 넣습니다.
				ansi_str = total_str;
			}
		}
	}

	LocalUnlock(hSelection);
	LocalFree(hSelection);

	CopyTextToClipboard(ansi_str);
#else
	CStringA ansi_str; // ASCII 형식으로 문자열을 변경하기 위해서 사용
	CString str, total_str;
	// 리스트박스에 등록된 항목의 개수를 얻는다. 
	int count = m_event_list.GetCount();

	// 항목을 이동시키기 위해서 선택된 항목을 복사한 후, 선택항목을 삭제해야 
	// 하는데 위쪽에서 지우면 항목의 위치가 변경되면서 아래쪽 선택항목을 체크하는데 
	// 오차가 발생합니다. 따라서 리스트박스의 항목을 위에서 아래로 가면서 복사하는 것이 
	// 아니라 아래쪽에서 위쪽으로 올라오면서 복사하여 중간에 항목을 삭제하더라도 
	// 위치상의 오차가 발생하지 않도록 합니다. 
	for (int i = 0; i < count - 1; i++) {

		// 현재 항목이 선택되어있는지 체크합니다. 
		// GetCurSel은 활성화된 체크 항목만 찾을때 사용합니다. 
		if (m_event_list.GetSel(i)) {

			// 선택되어있는 항목의 문자열을 얻습니다. 
			m_event_list.GetText(i, str);

			// 각 항목의 문자열을 하나의 문자열로 합친다.
			total_str += str;
			// 각 라인을 구분하기 위해서 개행문자를 추가한다.
			total_str += L"\r\n";

			// total_str에 합쳐진 URL 문자열은 유니코드 형식이기 때문에
			// ASCII 형식으로 변경해서 ansi_str에 넣습니다.
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
	if (m_event_list.GetCurSel() != LB_ERR) // LB_ERR = -1의 의미는 어떤 문자열이 선택됐을 때만 실행
	{
		CStringA ansi_str; // ASCII 형식으로 문자열을 변경하기 위해서 사용
		CString str, total_str;
		// 리스트 박스에 추가된 항목의 갯수를 얻는다.
		int count = m_event_list.GetCount();
		for (int i = 0; i < count; i++) {
			// 리스트 박스의 각 항목을 가져온다.
			m_event_list.GetText(i, str);
			// 각 항목의 문자열을 하나의 문자열로 합친다.
			total_str += str;
			// 각 URL을 구분하기 위해서 개행문자를 추가한다.
			total_str += L"\r\n";
		}
		// total_str에 합쳐진 URL 문자열은 유니코드 형식이기 때문에
		// ASCII 형식으로 변경해서 ansi_str에 넣습니다.
		ansi_str = total_str;

		// 자신이 클립보드에 문자열을 넣어도 WM_DRAWCLIPBOARD 메시지가 발생해서
		// 지금 추가하는 클립보드 내용이 리스트 박스에 추가되기 때문에 
		// ansi_str에 저장된 URL 목록을 클립보드에 복사해준다.
		CopyTextToClipboard(ansi_str);
	}
}


