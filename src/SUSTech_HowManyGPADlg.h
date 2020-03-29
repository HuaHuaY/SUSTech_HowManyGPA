
// SUSTech_HowManyGPADlg.h: 头文件
//
#pragma once
#include "DIALOG_LOGIN.h"
#include <map>
#define WM_USER_FRESH_SCORE WM_USER+1

// CSUSTechHowManyGPADlg 对话框
class CSUSTechHowManyGPADlg : public CDialogEx
{
	// 构造
public:
	CSUSTechHowManyGPADlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUSTECH_HOWMANYGPA_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
public:
	DIALOG_LOGIN dLogin;
	CListCtrl studentScore;
	int credit = 0;
	double GPASUM = 0.0;
	std::map<CString, double> mapGPA = { {"A+",4.00}, {"A",3.94}, {"A-",3.85},
										{"B+",3.73}, {"B",3.55}, {"B-",3.32},
										{"C+",3.09}, {"C",2.78}, {"C-",2.42},
										{"D+",2.08}, {"D",1.63}, {"D-",1.15}, {"F",0.0} };
	afx_msg LRESULT FreshScore(WPARAM iParam1, LPARAM iParam2);
	afx_msg void OnBnClickedButtonChooseall();
	afx_msg void OnBnClickedButtonClearall();
	void CSUSTechHowManyGPADlg::SetStatus(int nItem, bool addOrSub);
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnNMClickListScore(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCustomdrawListScore(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLvnKeydownListScore(NMHDR* pNMHDR, LRESULT* pResult);
};
