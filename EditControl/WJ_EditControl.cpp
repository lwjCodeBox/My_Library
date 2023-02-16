﻿// WJ_EditControl.cpp: 구현 파일
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

	// 문자열 출력에 사용할 글꼴을 생성한다.
	mh_font = ::CreateFont(0xFFFFFFEB, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
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

	return 0;
}

void WJ_EditControl::OnDestroy()
{
	CWnd::OnDestroy();

	// 이미지 객체에 그림 그리기에 사용하던 DC 핸들값을 반납한다.
	m_mem_image.ReleaseDC();
	// 이미지 객체를 제거한다.
	m_mem_image.Destroy();
	// 생성된 글꼴을 제거한다.
	DeleteObject(mh_font);
}

void WJ_EditControl::OnPaint()
{
	CPaintDC dc(this);

	// 이미지 객체에 그려진 그림을 화면에 출력한다.
	m_mem_image.Draw(dc, 0, 0);
}

void WJ_EditControl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	WORD key_data = 0;
	// 현재 키보드의 키 상태를 얻는다.
	GetKeyboardState(m_key_state);
	// 현재 키 상태를 기반으로 입력된 문자 값을 얻는다.
	ToAscii(nChar, MAKELPARAM(nRepCnt, nFlags), m_key_state, &key_data, 0);

	if (key_data) // 입력된 문자가 있는 지 확인, Shift 키 입력시에는 0으로 처리됨
	{  
		// 배경을 검은색으로 채워서 이전 내용을 지운다.
		Rectangle(mh_mem_dc, -2, -1, m_mem_image.GetWidth() + 1, m_mem_image.GetHeight() + 1); //Rectangle(mh_mem_dc, 0, 0, m_mem_image.GetWidth(), m_mem_image.GetHeight());

		if (key_data == 8) // push Backspace key
			m_str.Delete(m_str.GetLength() - 1);
		else if(key_data != 0)
		{
			CString str;
			// 입력된 문자 값을 문자로 변경한다.
			str.Format(L"%c", (int)key_data);
			m_str += str;  // 입력된 문자를 문자열에 추가한다.
		}

		if (key_data) {
			// 입력된 문자는 노란색으로 출력한다.
			SetTextColor(mh_mem_dc, RGB(255, 255, 0));
			// 입력된 문자를 화면에 출력한다.
			TextOut(mh_mem_dc, 3, 2, m_str, m_str.GetLength());
			// 화면을 갱신하여 입력된 문자가 화면에 표시되게 한다.
			Invalidate(0);

			SIZE temp_size;
			// 문자열이 출력되었을 때 폭과 높이 정보를 얻는다.
			::GetTextExtentPoint(mh_mem_dc, m_str, m_str.GetLength(), &temp_size);
			// 캐럿을 출력된 문자열 뒤에 위치하도록 위치를 조정한다.
			m_caret_x = 5 + temp_size.cx;
			// 계산된 캐럿의 위치에 캐럿이 이동하게 한다.
			::SetCaretPos(m_caret_x, 4);
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void WJ_EditControl::OnSetFocus(CWnd *pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);

	Activity_EditControl_Color();
}

void WJ_EditControl::OnKillFocus(CWnd *pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);

	Disabled_EditControl_Color();

	// 캐럿을 화면에서 숨긴다.
	::HideCaret(m_hWnd);
	// 캐럿을 제거한다.
	::DestroyCaret();
}


void WJ_EditControl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 포커스를 가진 윈도우가 현재 윈도우가 아니라면 현재 윈도우를 포커스 상태로 변경!
	if (::GetFocus() != m_hWnd) ::SetFocus(m_hWnd);

	CWnd::OnLButtonDown(nFlags, point);
}


void WJ_EditControl::OnNcPaint()
{
	Activity_EditControl_Color();
}

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

	// 폭이 1이고 높이가 18인 캐럿을 생성한다.
	::CreateCaret(m_hWnd, NULL, 1, 18);
	// 생성된 캐럿의 위치를 지정한다.
	::SetCaretPos(m_caret_x, 4);
	// 캐럿을 화면에 표시한다.
	::ShowCaret(m_hWnd);
}

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