#ifndef _WJ_EDIT_CONTROLH_
#define _WJ_EDIT_CONTROLH_

/*
	* Version : 1.0
	* �Ӽ� -> �߰� ���丮
		������Ʈ �ҽ� ��ġ �߰�. (#include "pch.h" ������ ��.)
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
	CImage m_mem_image;		// �ڿ����ο� ����� ���� ����� �̹��� ��ü
	HDC mh_mem_dc;			// m_mem_image ��ü�� �׸��� �׸��� ����� DC �ڵ鰪

	BYTE m_key_state[256];	// ���� Ű ���¸� ����� ����
	HFONT mh_font;			// ����� ���ڿ��� ����� �۲�
	CString m_str;			// �Է� ���ڿ��� ������ ���ڿ� ��ü
	int m_caret_x = 5;		// ���� ĳ���� x�� ��ġ
	int m_caret_y;			// ���� ĳ���� y�� ��ġ
	int m_font_width;		// ���� �ʺ�
	int m_font_height;		// ���� ����
	int m_text_cy;			// ���� ���� ��ġ

public:
	WJ_EditControl();
	virtual ~WJ_EditControl();

protected:
	DECLARE_MESSAGE_MAP()

	BOOL GetCurrentInputMode();
	void SetCurrentInputMode(BOOL parm_mode);
	void Activity_EditControl_Color();
	void Disabled_EditControl_Color();

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
};

#endif

