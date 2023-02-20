#ifndef _WJ_EDIT_CONTROLH_
#define _WJ_EDIT_CONTROLH_

/*
	* Version : 1.0
	* 속성 -> 추가 디렉토리
		프로젝트 소스 위치 추가. (#include "pch.h" 때문에 함.)
		ex) C:\Users\lwj79\Desktop\My_Workspace\_MFC\Raon_Helper\Raon_Helper;
*/

#include <imm.h>
#pragma comment(lib, "imm32.lib")
#define TEXT_X_POS	3

// WJ_EditControl
class WJ_EditControl : public CWnd
{
	DECLARE_DYNAMIC(WJ_EditControl)
protected:
	CImage m_mem_image;		// 자연스로운 출력을 위해 사용할 이미지 객체
	HDC mh_mem_dc;			// m_mem_image 객체에 그림을 그릴때 사용할 DC 핸들값

	BYTE m_key_state[256];	// 현재 키 상태를 기억할 변수
	HFONT mh_font;			// 출력할 문자열에 사용할 글꼴
	CString m_str;			// 입력 문자열을 저장할 문자열 객체
	int m_caret_x = 5;		// 현재 캐럿의 x축 위치
	int m_caret_y;			// 현재 캐럿의 y축 위치
	int m_caret_height;		// 현재 캐럿의 높이
	int m_cur_caret_pos = 0;// 현재 캐럿의 위치
	int m_text_cy;			// 문자 높이 위치

	bool m_capital = false, m_numLock = false, m_shift = false;
public:
	WJ_EditControl();
	virtual ~WJ_EditControl();

protected:
	DECLARE_MESSAGE_MAP()

	BOOL GetCurrentInputMode();
	void SetCurrentInputMode(BOOL parm_mode);
	void Activity_EditControl_Color();
	void Disabled_EditControl_Color();

	inline int UnicodeToAscii(wchar_t *ap_src_str, char *ap_dest_str)
	{
		int len = WideCharToMultiByte(CP_ACP, 0, ap_src_str, -1, NULL, 0, NULL, NULL);
		if (ap_dest_str != NULL) {
			WideCharToMultiByte(CP_ACP, 0, ap_src_str, -1, ap_dest_str, len, NULL, NULL);
		}
		return len - 1;
	}

	inline int AsciiToUnicode(char *ap_src_str, wchar_t *ap_dest_str)
	{
		// 이 함수는 \0 까지 포함한 크기가 반환됨
		int len = MultiByteToWideChar(CP_ACP, 0, ap_src_str, -1, NULL, 0);
		if (ap_dest_str != NULL) MultiByteToWideChar(CP_ACP, 0, ap_src_str, -1, ap_dest_str, len);
		return len - 1;
	}

	void CopyTextW(CString a_str);
	int PasteTextW(CString *ap_string);
	void WriteToEditCtrl(CString ap_string);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#endif

