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
	int m_caret_height;		// ���� ĳ���� ����
	int m_cur_caret_pos = 0;// ���� ĳ���� ��ġ
	int m_text_cy;			// ���� ���� ��ġ

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
		// �� �Լ��� \0 ���� ������ ũ�Ⱑ ��ȯ��
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

