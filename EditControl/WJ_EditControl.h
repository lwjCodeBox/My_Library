#ifndef _WJ_EDIT_CONTROLH_
#define _WJ_EDIT_CONTROLH_

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

public:
	WJ_EditControl();
	virtual ~WJ_EditControl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnKillFocus(CWnd *pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#endif

