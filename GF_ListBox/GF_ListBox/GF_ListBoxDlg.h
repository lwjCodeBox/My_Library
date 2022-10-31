
// GF_ListBoxDlg.h : header file
//

#pragma once


// CGFListBoxDlg dialog
class CGFListBoxDlg : public CDialogEx
{
private:
	CListBox m_event_list;

	// ����Ʈ �ڽ��� �̺�Ʈ �α׸� �߰��ϴ� �Լ�
	inline void AddEventString(const wchar_t *const ap_stirng, ...)
	{
		wchar_t str[512];
		// ���� ���� ���� ���� ������ ó���ؼ� str �迭�� ���ڿ��� �����Ѵ�.
		va_list args;
		va_start(args, ap_stirng);
		vswprintf_s(str, 512, ap_stirng, args);
		va_end(args);

		// ������ ���ڿ��� ����Ʈ �ڽ��� �߰��Ѵ�.
		int index = m_event_list.InsertString(-1, str);
		// �߰��� ��ġ�� �����Ѵ�.
		m_event_list.SetCurSel(index);
	}

	int CopyTextToClipboard(const char *ap_string);
	void OnSelectChanged();
// Construction
public:
	CGFListBoxDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GF_LISTBOX_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();

	afx_msg void OnLbnDblclkEventList();
	afx_msg void OnBnClickedLbTotalCopy();
	virtual BOOL PreTranslateMessage(MSG *pMsg);
};
