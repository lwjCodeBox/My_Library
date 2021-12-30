// 클래스 제작 : 김성엽 (tipsware@naver.com)
// 2021년 4월 11일 일요일
// 서브 클래스시에도 초기화가 잘 진행되도록 PreSubclassWindow 함수 추가
#pragma once

// WJ_ListBox가 사용하는 색상과 관련된 구조체
struct ListBoxColorAttr
{
	// 리스트 박스 배경색으로 사용할 브러시 객체
	HBRUSH h_list_bk_brush;
	// 상황별 브러시 색상을 기억할 변수들
	COLORREF select_brush_color, focus_brush_color, bk_brush_color;
	// 상황별 펜 색상을 기억할 변수들
	COLORREF select_pen_color, focus_pen_color, bk_pen_color;
};

// 리스트 박스의 각 항목 출력을 위한 함수 원형을 함수 포인터 형식으로 선언
typedef void (*PFN_LIST_DRAW)(DRAWITEMSTRUCT *ap_dis, ListBoxColorAttr *ap_attr);

// WJ_ListBox
class WJ_ListBox : public CListBox
{
	DECLARE_DYNAMIC(WJ_ListBox)

protected:
	ListBoxColorAttr m_attr;
	// 현재 처리중인 Owner-Draw 구조체 정보, 매개 변수로 계속 안넘기려고 
	// 구조체 주소를 보관해서 사용. 나중에 사용하면 주소가 유효하지 
	// 않을수 있으니 주의해야 함!
	LPDRAWITEMSTRUCT mp_last_lpds;

	// 각 항목의 배경을 그리기 위해 사용할 함수 포인터
	PFN_LIST_DRAW mp_base_draw;
	// 각 항목에 출력될 정보를 그리는 함수 포인터
	PFN_LIST_DRAW mp_user_draw;

public:
	WJ_ListBox();
	virtual ~WJ_ListBox();

	// 리스트 박스의 각 항목을 출력하는데 사용할 함수의 주소를 설정하는 함수
	void SetDrawFunc(PFN_LIST_DRAW ap_base_draw, PFN_LIST_DRAW ap_user_draw);

	// WM_CREATE를 사용하지 않고 리스트 박스가 생성될때 함께 작업해야 할
	// 내용을 추가하기 위해서 이 함수를 제공함
	virtual void CreateUserObject() {}
	// WM_DESTROY를 사용하지 않고 리스트 박스가 제거될때 함께 작업해야 할
	// 내용을 추가하기 위해서 이 함수를 제공함
	virtual void DeleteUserObject() {}
	// 리스트 박스의 특정 항목 위치만 갱신하는 함수
	void UpdateItemInfo(int a_index, int a_count);
	// 항목을 더블 클릭했을 때 호출되는 함수
	virtual void ProcessDoubleClicked(int a_index) { }

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH CtlColor(CDC * /*pDC*/, UINT /*nCtlColor*/);
	afx_msg void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual void PreSubclassWindow();
	afx_msg void OnLbnDblclk();
};