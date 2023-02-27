// WJ_EditControl.cpp: 구현 파일
#include "pch.h"
#include "WJ_EditControl.h"

// WJ_EditControl
IMPLEMENT_DYNAMIC(WJ_EditControl, CWnd)

WJ_EditControl::WJ_EditControl()
{

}

WJ_EditControl::~WJ_EditControl()
{
}

BEGIN_MESSAGE_MAP(WJ_EditControl, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//http://www.tipssoft.com/bulletin/board.php?bo_table=update&wr_id=94
BOOL WJ_EditControl::GetCurrentInputMode()
{
	DWORD conv, sentence;
	HIMC h_imc = ImmGetContext(m_hWnd);

	// IME(input method editor) 상태를 얻는다.
	ImmGetConversionStatus(h_imc, &conv, &sentence);

	ImmReleaseContext(m_hWnd, h_imc);
	if (conv & IME_CMODE_LANGUAGE) return TRUE;  // 한글 상태임...

	return FALSE;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::SetCurrentInputMode(BOOL parm_mode)
{
	DWORD conv, sentence;

	HIMC h_imc = ImmGetContext(m_hWnd);
	// IME(input method editor) 상태를 얻는다.
	ImmGetConversionStatus(h_imc, &conv, &sentence);

	if (parm_mode == TRUE) {
		// 영문 입력 상태였다면 상태를 제거한다.
		conv = conv & (~IME_CMODE_CHARCODE);
		// 한글 입력 상태로 설정한다.
		conv = conv | IME_CMODE_LANGUAGE;
	}
	else {
		// 한글 입력 상태였다면 상태를 제거한다.
		conv = conv & (~IME_CMODE_LANGUAGE);
		// 영문 입력 상태로 설정한다.
		conv = conv | IME_CMODE_CHARCODE;
	}

	// IME 상태를 설정한다.
	ImmSetConversionStatus(h_imc, conv, sentence);
	ImmReleaseContext(m_hWnd, h_imc);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// WJ_EditControl 메시지 처리기
int WJ_EditControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) return -1;

	/*
	CreateFont(
		15,                  // 문자 폭
		0,                   // 문자 높이
		0,                   // 문자 기울기
		0,                   // 문자 방향
		FW_NORMAL,           // 문자 굵기
		FALSE,               // 기울기
		FALSE,               // 밑줄
		0,                   // 취소선
		DEFAULT_CHARSET,     // 문자셋
		OUT_DEFAULT_PRECIS,  // 출력 정확도
		CLIP_DEFAULT_PRECIS, // 클리핑 정확도
		DEFAULT_QUALITY,     // 출력의 질
		DEFAULT_PITCH   |   FF_SWISS, "Arial "); // 자간과 폰트
	*/

	// 문자열 출력에 사용할 글꼴을 생성한다. 0xFFFFFFEB
	mh_font = ::CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_SWISS, L"Consolas");

	CRect r;
	GetClientRect(r);  // 현재 윈도우의 클라이언트 영역의 좌표를 얻는다.
	// 화면 표시에 사용할 이미지 객체를 생성한다.
	m_mem_image.Create(r.Width(), r.Height(), 32);
	// 이미지 객체에 그림을 그릴때 사용할 DC 핸들값을 얻는다.
	mh_mem_dc = m_mem_image.GetDC();
	// 배경 색상을 투명하게 처리한다.
	SetBkMode(mh_mem_dc, TRANSPARENT);
	// 펜과 브러시를 색상만 변경해서 사용할수 있는 객체로 지정한다.
	SelectObject(mh_mem_dc, GetStockObject(DC_BRUSH));
	SelectObject(mh_mem_dc, GetStockObject(DC_PEN));
	// 브러시 기본색상을 검은색으로 지정한다.
	SetDCBrushColor(mh_mem_dc, RGB(56, 56, 56));
	// 배경을 검은색으로 채워서 이전 내용을 지운다.
	Rectangle(mh_mem_dc, -2, -1, m_mem_image.GetWidth() + 1, m_mem_image.GetHeight() + 1); //Rectangle(mh_mem_dc, 0, 0, m_mem_image.GetWidth(), m_mem_image.GetHeight());
	// 생성된 글꼴을 지정한다.
	SelectObject(mh_mem_dc, mh_font);

	SIZE temp_size;
	::GetTextExtentPoint(mh_mem_dc, L" ", 1, &temp_size);
	m_caret_height = temp_size.cy + 2;

	m_caret_y = (int)temp_size.cy / 5;
	m_text_cy = m_caret_y + 1;
	
	// 캐럿 위치 정보를 저장 하기 위해 노드 생성.
	caret_pos_list *m_cp_newNode = SLL_CreateNode(m_caret_x);
	SLL_AppendNode(&m_cp_list, m_cp_newNode);
	
	return 0;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::OnDestroy()
{
	CWnd::OnDestroy();

	// 캐럿 위치 정보를 담고 있는 노드를 파괴한다.
	SLL_DestroyAllNode(m_cp_list);

	// 이미지 객체에 그림 그리기에 사용하던 DC 핸들값을 반납한다.
	m_mem_image.ReleaseDC();
	// 이미지 객체를 제거한다.
	m_mem_image.Destroy();
	// 생성된 글꼴을 제거한다.
	DeleteObject(mh_font);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::OnPaint()
{
	CPaintDC dc(this);

	// 이미지 객체에 그려진 그림을 화면에 출력한다.
	m_mem_image.Draw(dc, 0, 0);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::OnSetFocus(CWnd *pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	Activity_EditControl_Color();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::OnKillFocus(CWnd *pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	Disabled_EditControl_Color();

	// 캐럿을 화면에서 숨긴다.
	::HideCaret(m_hWnd);
	// 캐럿을 제거한다.
	::DestroyCaret();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 포커스를 가진 윈도우가 현재 윈도우가 아니라면 현재 윈도우를 포커스 상태로 변경!
	if (::GetFocus() != m_hWnd) ::SetFocus(m_hWnd);

	CWnd::OnLButtonDown(nFlags, point);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::OnNcPaint()
{
	if (::GetFocus() != m_hWnd) Disabled_EditControl_Color();
	else						Activity_EditControl_Color();
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::Activity_EditControl_Color()
{
	CWindowDC dc(this);

	CRect r, client_r;
	GetWindowRect(r);
	GetClientRect(client_r);  // 클라이언트 좌표를 추가로 구한다.
	int border_thick = (r.Width() - client_r.Width()) / 2; // 에디트 컨트롤의 테두리 굵기를 구한다.
	ScreenToClient(r);
	r += CPoint(border_thick, border_thick); // 테두리 굵기를 반영해서 위치 보정을 한다.

	dc.SelectStockObject(DC_PEN);
	dc.SelectStockObject(NULL_BRUSH);
	dc.SetDCPenColor(RGB(86, 156, 214));
	dc.Rectangle(r.left, r.top, r.right, r.bottom);
	if (border_thick > 1) {  // 테두리 굵기가 2이면 선을 하나 더 그린다.
		dc.SetDCPenColor(RGB(54, 73, 245));  // 분홍색 선으로 설정한다.
		dc.Rectangle(r.left + 1, r.top + 1, r.right - 1, r.bottom - 1);  // 사각형을 그린다.
	}

	SIZE temp_size;
	// 문자열이 출력되었을 때 폭과 높이 정보를 얻는다.
	::GetTextExtentPoint(mh_mem_dc, m_str.c_str(), m_str.length(), &temp_size);
	// 캐럿을 출력된 문자열 뒤에 위치하도록 위치를 조정한다.
	m_caret_x = 5 + temp_size.cx;

	// 캐럿 위치 저장
	m_caret_index = m_str.length();
	// 폭이 1이고 높이가 18인 캐럿을 생성한다.
	::CreateCaret(m_hWnd, NULL, 1, m_caret_height);
	// 생성된 캐럿의 위치를 지정한다.
	::SetCaretPos(m_caret_x, m_caret_y);
	// 캐럿을 화면에 표시한다.
	::ShowCaret(m_hWnd);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::Disabled_EditControl_Color()
{
	CWindowDC dc(this);
	CRect r, client_r;
	GetWindowRect(r);
	GetClientRect(client_r);  // 클라이언트 좌표를 추가로 구한다.
	int border_thick = (r.Width() - client_r.Width()) / 2; // 에디트 컨트롤의 테두리 굵기를 구한다.
	ScreenToClient(r);
	r += CPoint(border_thick, border_thick); // 테두리 굵기를 반영해서 위치 보정을 한다.

	dc.SelectStockObject(DC_PEN);
	dc.SelectStockObject(NULL_BRUSH);
	dc.SetDCPenColor(RGB(100, 100, 0));
	dc.Rectangle(r.left, r.top, r.right, r.bottom);
	if (border_thick > 1) {  // 테두리 굵기가 2이면 선을 하나 더 그린다.
		dc.SetDCPenColor(RGB(0, 100, 100));  // 분홍색 선으로 설정한다.
		dc.Rectangle(r.left + 1, r.top + 1, r.right - 1, r.bottom - 1);  // 사각형을 그린다.
	}
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::CopyTextW(std::wstring a_str)
{
	int len = a_str.length() << 1;
	char *aStr = (char *)malloc(len);
	memset(aStr, 0x00, len);
	
	UnicodeToAscii(a_str.c_str(), aStr);

	HGLOBAL ClipData = ::GlobalAlloc(GHND, len);
	char *lpClip = (char *)::GlobalLock(ClipData);
	memcpy(lpClip, aStr, len);
	
	::GlobalUnlock(ClipData);
	::OpenClipboard(GetSafeHwnd());
	::EmptyClipboard();
	::SetClipboardData(CF_TEXT, ClipData);
	::CloseClipboard();

	free(aStr);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
int WJ_EditControl::PasteTextW(std::wstring *ap_string)
{
	unsigned int priority_list = CF_TEXT;
	HANDLE h_clipboard_data;
	char *p_clipboard_data;
	int format_info = ::GetPriorityClipboardFormat(&priority_list, 1), len = 0;

	if (format_info == CF_TEXT) {  // 클립보드에 저장된 정보가 텍스트 인경우에만 처리한다.
		// 클립보드에 있는 비트맵 정보를 얻기 위해서 클립보드를 연다.
		if (::OpenClipboard(m_hWnd)) {  // 클립보드를 연다.
			// 클립보드에 저장된 정보의 핸들을 얻는다.
			h_clipboard_data = ::GetClipboardData(format_info);
			if (h_clipboard_data != NULL) {  // 정보를 정상적으로 얻었다면
				// 클립보드에 저장된 문자열를 복사하기 위해 클립보드 핸들을 사용하여 
				// 문자열이 저장된 메모리의 주소를 얻는다.
				p_clipboard_data = (char *)::GlobalLock(h_clipboard_data);
				// 클립보드에 저장된 문자열의 길이를 얻는다.
				len = strlen(p_clipboard_data);

				// <--- 원본(https://lwj789.tistory.com/123)에서 3줄 추가. 
				wchar_t *p_wStr = new wchar_t[len + 1];
				memset(p_wStr, 0, (len + 1) << 1);				
				AsciiToUnicode(p_clipboard_data, p_wStr);
				// --->
				*ap_string = p_wStr;// p_clipboard_data;
				
				delete[] p_wStr;
				
				// 클립보드에 저장된 데이터 핸들을 다른 프로세스가 사용할 수 있도록 반환한다.
				::GlobalUnlock(h_clipboard_data);
			}
			::CloseClipboard();  // 클립보드를 닫는다.
		}
	}
	return len;  // 복사한 데이터의 크기를 반환한다.
}
 
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::WriteToEditCtrl(const wchar_t *ap_string)
{
	int len = wcslen(ap_string);

	// 입력된 문자는 노란색으로 출력한다.
	SetTextColor(mh_mem_dc, RGB(255, 255, 0));
	// 입력된 문자를 화면에 출력한다.
	TextOut(mh_mem_dc, TEXT_X_POS, m_text_cy, ap_string, len);
	// 화면을 갱신하여 입력된 문자가 화면에 표시되게 한다.
	Invalidate(0);

	SIZE temp_size;
	// 문자열이 출력되었을 때 폭과 높이 정보를 얻는다.
	::GetTextExtentPoint(mh_mem_dc, ap_string, len, &temp_size);
	// 캐럿을 출력된 문자열 뒤에 위치하도록 위치를 조정한다.
	m_caret_x = 5 + temp_size.cx;
	// 계산된 캐럿의 위치에 캐럿이 이동하게 한다.
	::SetCaretPos(m_caret_x, m_caret_y);

	DbgLogW(L"OnKeyDown() - %c (w: %d) (h: %d) (cur pos: %d)\n", m_str.at(m_caret_index), temp_size.cx, temp_size.cy, m_caret_x);
				 
	// 캐럿 위치 정보를 저장 하기 위해 노드 생성.
	caret_pos_list *m_cp_newNode = SLL_CreateNode(m_caret_x);
	SLL_AppendNode(&m_cp_list, m_cp_newNode);
	SLL_ShowAllData(m_cp_list);

	m_caret_index = len;
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::WriteHangeulToEditCtrl(const wchar_t *ap_string, LPARAM lParam)
{
	int len = wcslen(ap_string);

	// 입력된 문자는 노란색으로 출력한다.
	SetTextColor(mh_mem_dc, RGB(255, 255, 0));
	// 입력된 문자를 화면에 출력한다.
	TextOut(mh_mem_dc, TEXT_X_POS, m_text_cy, ap_string, len);
	// 화면을 갱신하여 입력된 문자가 화면에 표시되게 한다.
	Invalidate(0);
	
	SIZE temp_size;
	// 문자열이 출력되었을 때 폭과 높이 정보를 얻는다.
	::GetTextExtentPoint(mh_mem_dc, ap_string, len, &temp_size);
	// 캐럿을 출력된 문자열 뒤에 위치하도록 위치를 조정한다.
	m_caret_x = 5 + temp_size.cx;
	// 계산된 캐럿의 위치에 캐럿이 이동하게 한다.
	::SetCaretPos(m_caret_x, m_caret_y);

	m_caret_index = len;

	// 글자 완성
	if (lParam & GCS_RESULTSTR)
	{
		DbgLogW(L"글자 완성\n");
		DbgLogW(L"OnKeyDown() - %c (w: %d) (h: %d) (cur pos: %d)\n", m_str.at(m_caret_index - 1), temp_size.cx, temp_size.cy, m_caret_x);
		// 캐럿 위치 정보를 저장 하기 위해 노드 생성.
		caret_pos_list *m_cp_newNode = SLL_CreateNode(m_caret_x);
		SLL_AppendNode(&m_cp_list, m_cp_newNode);
		SLL_ShowAllData(m_cp_list);
		
	}
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::EraseStrFromEditCtrl(const wchar_t *ap_string, int a_start_idx/* = 0*/, int a_end_idx/* = 0*/)
{
	if (a_start_idx == 0) return;
	
	SetDCBrushColor(mh_mem_dc, RGB(56, 56, 56));
	Rectangle(mh_mem_dc, -2, -1, m_mem_image.GetWidth() + 1, m_mem_image.GetHeight() + 1);

	int len = m_str.length();

	// 캐럿 갯수 줄임.
	m_caret_index--;

	// 캐럿 정보 삭제
	DbgLogW(L"Erase before\n\n");
	SLL_ShowAllData(m_cp_list);
	caret_pos_list *p = NULL;
	// TODO: 삭제
	for (int i = len; i > a_start_idx; --i)
	{
		int n = 0;
		n = SLL_GetNodeAt(m_cp_list, i - 1)->Data;
		SLL_GetNodeAt(m_cp_list, i)->Data = n;
	}
	DbgLogW(L"Erase after\n\n");
	SLL_RemoveNode(&m_cp_list, a_start_idx);
	SLL_ShowAllData(m_cp_list);

	


	// 맨 뒤에 있는 문자열 삭제
	m_str.erase(m_caret_index, 1);

	// 입력된 문자는 노란색으로 출력한다.
	SetTextColor(mh_mem_dc, RGB(255, 255, 0));
	// 입력된 문자를 화면에 출력한다.
	TextOut(mh_mem_dc, TEXT_X_POS, m_text_cy, m_str.c_str(), len);
	// 화면을 갱신하여 입력된 문자가 화면에 표시되게 한다.
	Invalidate(0);

	SIZE temp_size;
	// 문자열이 출력되었을 때 폭과 높이 정보를 얻는다.
	::GetTextExtentPoint(mh_mem_dc, m_str.c_str(), len, &temp_size);
	// 캐럿을 출력된 문자열 뒤에 위치하도록 위치를 조정한다.
	m_caret_x = SLL_GetNodeAt(m_cp_list, m_caret_index)->Data;
	// 계산된 캐럿의 위치에 캐럿이 이동하게 한다.
	::SetCaretPos(m_caret_x, m_caret_y);

	


	DbgLogW(L"EraseStrFromEditCtrl() - (cur pos: %d)\n", m_caret_x);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
void WJ_EditControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 방향키 (<-)
	// 방향키 (<-)
	if (nChar == VK_LEFT)
	{
		SetDCBrushColor(mh_mem_dc, RGB(56, 56, 56));
		Rectangle(mh_mem_dc, -2, -1, m_mem_image.GetWidth() + 1, m_mem_image.GetHeight() + 1);

		int len = m_str.length();

		// 입력된 문자는 노란색으로 출력한다.
		SetTextColor(mh_mem_dc, RGB(255, 255, 0));
		// 입력된 문자를 화면에 출력한다.
		TextOut(mh_mem_dc, TEXT_X_POS, m_text_cy, m_str.c_str(), len);
		// 화면을 갱신하여 입력된 문자가 화면에 표시되게 한다.
		Invalidate(0);

		if (m_caret_index != 0)
		{
			if (!(::GetKeyState(VK_LSHIFT) < 0 || ::GetKeyState(VK_RSHIFT) < 0))
			{
				DbgLogW(L"left key\n");

				SIZE temp_size;
				wchar_t b[3] = { 0 };
				wsprintf(b, L"%c", m_str.at(m_caret_index - 1));
				::GetTextExtentPoint(mh_mem_dc, b, 1, &temp_size);

				m_caret_x -= temp_size.cx;
#if(_DEBUG)
				DbgLogW(L"VK_LSHIFT() - %c (w: %d) (h: %d) (cur pos: %d)\n", m_str.at(m_caret_index - 1), temp_size.cx, temp_size.cy, m_caret_x);
#endif

				::SetCaretPos(m_caret_x, m_caret_y);
				::ShowCaret(m_hWnd);
				m_caret_index--;
			}
			else // shift + left key
			{
				DbgLogW(L"shift + left key\n");

				caret_pos_list *node = SLL_GetNodeAt(m_cp_list, m_caret_index);
				
				DbgLogW(L"%d \n", node->Data);

				SetDCBrushColor(mh_mem_dc, RGB(150, 100, 150));
				Rectangle(mh_mem_dc, -2, -1, m_mem_image.GetWidth() + 1, m_mem_image.GetHeight() + 1);

				
				
				int len = m_str.length();

				// 입력된 문자는 노란색으로 출력한다.
				SetTextColor(mh_mem_dc, RGB(255, 255, 0));
				// 입력된 문자를 화면에 출력한다.
				TextOut(mh_mem_dc, TEXT_X_POS, m_text_cy, m_str.c_str(), len);
				Invalidate(0);
				
				SIZE temp_size;
				wchar_t b[3] = { 0 };
				wsprintf(b, L"%c", m_str.at(m_caret_index - 1));
				::GetTextExtentPoint(mh_mem_dc, b, 1, &temp_size);

				//if (1)
				//{
				//	SetDCBrushColor(mh_mem_dc, RGB(150, 100, 150)); // 핑크색
				//	Rectangle(mh_mem_dc, -2, -1, temp_size.cx, m_caret_height);		
				//}

				m_caret_x -= temp_size.cx;
				::SetCaretPos(m_caret_x, m_caret_y);
				::HideCaret(m_hWnd);
				m_caret_index--;
			}
		}
	}
	// TODO: 방향키 (->)
	// 방향test키 (->)
	else if (nChar == VK_RIGHT)
	{
		SetDCBrushColor(mh_mem_dc, RGB(56, 56, 56));
		Rectangle(mh_mem_dc, -2, -1, m_mem_image.GetWidth() + 1, m_mem_image.GetHeight() + 1);

		int len = m_str.length();

		// 입력된 문자는 노란색으로 출력한다.
		SetTextColor(mh_mem_dc, RGB(255, 255, 0));
		// 입력된 문자를 화면에 출력한다.
		TextOut(mh_mem_dc, TEXT_X_POS, m_text_cy, m_str.c_str(), len);
		// 화면을 갱신하여 입력된 문자가 화면에 표시되게 한다.
		Invalidate(0);
		
		if (/*m_caret_index < m_str.GetLength()*/SLL_GetNodeAt(m_cp_list, m_caret_index)->NextNode)
		{
			SIZE temp_size;
			wchar_t b[3] = { 0 };
			wsprintf(b, L"%c", m_str.at(m_caret_index)); // TODO: 여기서 부터 보면 됨.
			::GetTextExtentPoint(mh_mem_dc, b, 1, &temp_size);

			m_caret_x += temp_size.cx;
#if(_DEBUG)
			DbgLogW(L"VK_RIGHT - %c (w: %d) (h: %d) (cur pos: %d)\n", m_str.at(m_caret_index), temp_size.cx, temp_size.cy, m_caret_x);
#endif

			::SetCaretPos(m_caret_x, m_caret_y);
			::ShowCaret(m_hWnd);
			m_caret_index++;
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
LRESULT WJ_EditControl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_IME_STARTCOMPOSITION)
		DbgLogW(L"WM_IME_STARTCOMPOSITION\n");
	else if (message == WM_IME_ENDCOMPOSITION)
		DbgLogW(L"WM_IME_ENDCOMPOSITION\n");
	else if (message == WM_IME_COMPOSITION) {
		DbgLogW(L"WM_IME_COMPOSITION - [hwnd: 0x%X] [message: 0x%X] [wParam: 0x%X] [lParam: 0x%X]\n", m_hWnd, message, wParam, lParam);

		int len = 0;
		wchar_t uniStr[4] = { 0 };
		HIMC hIMC = ImmGetContext(m_hWnd);
		if (lParam & GCS_COMPSTR) {
			len = ImmGetCompositionString(hIMC, GCS_COMPSTR, NULL, 0);
			ImmGetCompositionString(hIMC, GCS_COMPSTR, uniStr, len);

			// 배경을 검은색으로 채워서 이전 내용을 지운다.
			Rectangle(mh_mem_dc, -2, -1, m_mem_image.GetWidth() + 1, m_mem_image.GetHeight() + 1);

			// 조합중인 글자 출력..
			uniStr[len] = '\0';
			m_str += uniStr;

			WriteHangeulToEditCtrl(m_str.c_str(), lParam);

			m_str.erase(m_str.length() - 1, 1);
		}
		if (lParam & GCS_RESULTSTR) {
			len = ImmGetCompositionString(hIMC, GCS_RESULTSTR, NULL, 0);
			ImmGetCompositionString(hIMC, GCS_RESULTSTR, uniStr, len);

			// 배경을 검은색으로 채워서 이전 내용을 지운다.
			Rectangle(mh_mem_dc, -2, -1, m_mem_image.GetWidth() + 1, m_mem_image.GetHeight() + 1);

			// 완성된 글자 출력..
			uniStr[len] = '\0';
			m_str += uniStr;

			WriteHangeulToEditCtrl(m_str.c_str(), lParam);			
		}

		ImmReleaseContext(m_hWnd, hIMC);
	}

	else if (message == WM_KEYDOWN) 
	{
		WORD key_data = 0;
		// 현재 키보드의 키 상태를 얻는다.
		GetKeyboardState(m_key_state);
		// 현재 키 상태를 기반으로 입력된 문자 값을 얻는다.
		ToAscii(wParam, lParam, m_key_state, &key_data, 0);

		DbgLogW(L"DefWindowProc - [hwnd: 0x%X] [message: 0x%X] [wParam: 0x%X] [lParam: 0x%X]\n", m_hWnd, message, wParam, lParam);

		if (::GetKeyState(VK_CONTROL) < 0)
		{
			if (wParam == 67)		// [Paste] Control + C
				CopyTextW(m_str);
			else if (wParam == 86)	// [Paste] Control + V
			{
				std::wstring temp_str;
				PasteTextW(&temp_str);	// 클립보드에 복사된 문자열을 가져온다.
				m_str += temp_str;		// 입력된 문자를 문자열에 추가한다.
				WriteToEditCtrl(m_str.c_str());
			}
		}
		
		// 입력된 문자가 있는 지 확인, Shift 키 입력시에는 0으로 처리됨 
		// push ESC key(27)
		else if (key_data != 0 && key_data != 27 && key_data != VK_BACK)
		{
			// 배경을 검은색으로 채워서 이전 내용을 지운다.
			Rectangle(mh_mem_dc, -2, -1, m_mem_image.GetWidth() + 1, m_mem_image.GetHeight() + 1); //Rectangle(mh_mem_dc, 0, 0, m_mem_image.GetWidth(), m_mem_image.GetHeight());

			m_str += (char)key_data;  // 입력된 문자를 문자열에 추가한다.				
			WriteToEditCtrl(m_str.c_str());
		}
		// TODO: push Backspace key
		else if (key_data == VK_BACK && m_caret_index != 0)
		{
			EraseStrFromEditCtrl(m_str.c_str(), m_caret_index);
		}
	}


	else if (message == WM_KEYUP) 
	{

	}

	return CWnd::DefWindowProc(message, wParam, lParam);
}