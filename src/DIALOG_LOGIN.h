#pragma once
#include "afxinet.h"
#define WM_USER_FRESH_SCORE WM_USER+1
// DIALOG_LOGIN 对话框
class DIALOG_LOGIN : public CDialogEx
{
	DECLARE_DYNAMIC(DIALOG_LOGIN)

public:
	DIALOG_LOGIN(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~DIALOG_LOGIN();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString inputAccount;
	CString inputPassword;
	CInternetSession session;
	CFont newFont;

	static wchar_t* DIALOG_LOGIN::UTF8toWide(CString& str);
	static char* DIALOG_LOGIN::WidetoUTF8(CString& str);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
