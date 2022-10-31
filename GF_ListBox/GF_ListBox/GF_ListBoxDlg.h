
// GF_ListBoxDlg.h : header file
//

#pragma once


// CGFListBoxDlg dialog
class CGFListBoxDlg : public CDialogEx
{
private:
	CListBox m_event_list;

	// 리스트 박스에 이벤트 로그를 추가하는 함수
	inline void AddEventString(const wchar_t *const ap_stirng, ...)
	{
		wchar_t str[512];
		// 전달 받은 가변 인자 형식을 처리해서 str 배열에 문자열을 구성한다.
		va_list args;
		va_start(args, ap_stirng);
		vswprintf_s(str, 512, ap_stirng, args);
		va_end(args);

		// 구성된 문자열을 리스트 박스에 추가한다.
		int index = m_event_list.InsertString(-1, str);
		// 추가된 위치를 선택한다.
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
