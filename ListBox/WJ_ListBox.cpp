// WJ_ListBox.cpp: 구현 파일
#include "pch.h"
#include "WJ_ListBox.h"

// WJ_ListBox
IMPLEMENT_DYNAMIC(WJ_ListBox, CListBox)

// 각 항목의 배경을 그리기 위해 사용할 함수
void SampleBaseDraw(DRAWITEMSTRUCT *ap_dis, ListBoxColorAttr *ap_attr)
{
	// 전체 갱신, 포커스드 갱신, 선택 갱신인 경우에 처리한다.
	if (ap_dis->itemAction & 0x07) { // 0x07 = ODA_DRAWENTIRE|ODA_FOCUS|ODA_SELECT
		// 색상만 변경해서 사용하는 펜과 브러시 객체를 선택한다.
		::SelectObject(ap_dis->hDC, ::GetStockObject(DC_BRUSH));
		::SelectObject(ap_dis->hDC, ::GetStockObject(DC_PEN));
		// 현재 선택된 상태인지 체크한다.
		if (ap_dis->itemState & ODS_SELECTED) {
			// 선택된 상태의 펜 색상을 설정한다.
			::SetDCPenColor(ap_dis->hDC, ap_attr->select_pen_color);
			// 포커스 상태인지 체크하고 포커스 상태라면 조금더 
			// 밝은 배경색을 사용하고 아니라면 배경색과 동일한 색상을 사용한다.
			if (ap_dis->itemState & ODS_FOCUS) {
				::SetDCBrushColor(ap_dis->hDC, ap_attr->focus_brush_color);
			} else ::SetDCBrushColor(ap_dis->hDC, ap_attr->select_brush_color);
		} else {
			// 선택된 상태가 아니라면 배경색과 같은 브러시와 펜을 사용한다.
			::SetDCPenColor(ap_dis->hDC, ap_attr->bk_pen_color);
			::SetDCBrushColor(ap_dis->hDC, ap_attr->bk_brush_color);
		}
		// 항목의 배경을 그린다.
		::Rectangle(ap_dis->hDC, ap_dis->rcItem.left, ap_dis->rcItem.top,
			ap_dis->rcItem.right, ap_dis->rcItem.bottom);
	}
}

// 각 항목에 출력될 정보를 그리는 함수
void SampleUserDraw(DRAWITEMSTRUCT *ap_dis, ListBoxColorAttr *ap_attr)
{

}


WJ_ListBox::WJ_ListBox()
{
	// 리스트 박스가 사용할 색상 정보를 구성한다.
	m_attr.bk_brush_color = RGB(58, 62, 80);
	m_attr.focus_brush_color = RGB(82, 97, 124);

	m_attr.select_brush_color = m_attr.focus_pen_color = m_attr.select_pen_color = RGB(82, 97, 124);
	m_attr.bk_pen_color = m_attr.bk_brush_color;
	mp_base_draw = SampleBaseDraw;
	mp_user_draw = SampleUserDraw;
}

WJ_ListBox::~WJ_ListBox()
{
}

BEGIN_MESSAGE_MAP(WJ_ListBox, CListBox)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_DRAWITEM_REFLECT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT(LBN_DBLCLK, &WJ_ListBox::OnLbnDblclk)
END_MESSAGE_MAP()

// WJ_ListBox 메시지 처리기
HBRUSH WJ_ListBox::CtlColor(CDC * /*pDC*/, UINT /*nCtlColor*/)
{
	return m_attr.h_list_bk_brush;
}

void WJ_ListBox::DrawItem(LPDRAWITEMSTRUCT lpds)
{
	// 각 함수의 인자로 넘어갈 정보를 멤버로 백업해서 사용한다.
	mp_last_lpds = lpds;

	// 유효한 항목의 인덱스인지 체크한다.
	if ((UINT)lpds->itemID < (UINT)GetCount()) {
		// 각 항목의 배경을 그린다.
		(*mp_base_draw)(lpds, &m_attr);
		// 선택된 항목의 정보를 출력한다.
		(*mp_user_draw)(lpds, &m_attr);
	}
}

int WJ_ListBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListBox::OnCreate(lpCreateStruct) == -1) return -1;

	// 리스트 박스가 배경 채우기용으로 사용할 브러시를 생성한다.
	m_attr.h_list_bk_brush = ::CreateSolidBrush(m_attr.bk_brush_color);
	// 상속된 클래스에서 리스트 박스 생성시에 함께 작업할 내용이
	// 있다면 이 함수를 오버라이딩해서 사용!
	CreateUserObject();
	return 0;
}

void WJ_ListBox::OnDestroy()
{
	CListBox::OnDestroy();

	// 상속된 클래스에서 리스트 박스 제거시에 함께 작업할 내용이
	// 있다면 이 함수를 오버라이딩해서 사용!
	DeleteUserObject();
	// 리스트 박스가 배경 채우기용으로 사용할 브러시를 제거한다.
	::DeleteObject(m_attr.h_list_bk_brush);
}

void WJ_ListBox::PreSubclassWindow()
{
	CListBox::PreSubclassWindow();

	// 리스트 박스가 배경 채우기용으로 사용할 브러시를 생성한다.
	m_attr.h_list_bk_brush = ::CreateSolidBrush(m_attr.bk_brush_color);
	// 상속된 클래스에서 리스트 박스 생성시에 함께 작업할 내용이
	// 있다면 이 함수를 오버라이딩해서 사용!
	CreateUserObject();
}

// 리스트 박스의 특정 항목 위치만 갱신하는 함수
void WJ_ListBox::UpdateItemInfo(int a_index, int a_count)
{
	CRect r;
	if (a_count == 1) GetItemRect(a_index, r);  // 선택한 항목의 좌표를 얻는다.
	else {
		CRect temp_rect;
		// 원하는 항목 개수만큼 반복하면서 영역을 합친다.
		for (int i = 0; i < a_index; i++) {
			GetItemRect(a_index + i, temp_rect);  // 선택한 항목의 좌표를 얻는다.
			r.UnionRect(r, temp_rect);  // r 영역에 temp_rect영역을 합친다.
		}
	}
	InvalidateRect(r, 0);  // r영역만 갱신한다.
}

void WJ_ListBox::OnLbnDblclk()
{
	POINT pos;
	// 현재 마우스 위치를 얻는다.
	GetCursorPos(&pos);
	// 마우스 좌표를 리스트 박스 기준으로 변경한다.
	ScreenToClient(&pos);

	BOOL out_side_flag = 0;
	// 마우스가 위치한 곳에 있는 항목의 위치 값을 구한다.
	// 항목이 없는 곳이면 항목의 갯수에 해당하는 값이 반환한다.
	// 예를 들어, 항목이 6개면 유효 인덱스는 0~5까지인데 빈곳을
	// 클릭하면 6이 나옵니다. 그리고 이때는 out_side_flag 값이 
	// 1이 되기 때문에 이 값을 함께 체크하는 것이 안전합니다.
	UINT index = ItemFromPoint(pos, out_side_flag);
	UINT check_index = GetCurSel();
	if (!out_side_flag && index == check_index) {
		// index 위치가 유효한 경우에만 더블 클릭을 수행!
		ProcessDoubleClicked(check_index);
	}
}

// 리스트 박스의 각 항목을 출력하는데 사용할 함수의 주소를 설정하는 함수
void WJ_ListBox::SetDrawFunc(PFN_LIST_DRAW ap_base_draw, PFN_LIST_DRAW ap_user_draw)
{
	if(ap_base_draw != NULL) mp_base_draw = ap_base_draw;
	if(ap_user_draw != NULL) mp_user_draw = ap_user_draw;
}
