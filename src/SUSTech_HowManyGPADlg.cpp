// SUSTech_HowManyGPADlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "SUSTech_HowManyGPA.h"
#include "SUSTech_HowManyGPADlg.h"
#include "afxdialogex.h"
#include "afxinet.h"
#include <regex>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSUSTechHowManyGPADlg 对话框
CSUSTechHowManyGPADlg::CSUSTechHowManyGPADlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SUSTECH_HOWMANYGPA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSUSTechHowManyGPADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Score, studentScore);
}

BEGIN_MESSAGE_MAP(CSUSTechHowManyGPADlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER_FRESH_SCORE, FreshScore)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSEALL, &CSUSTechHowManyGPADlg::OnBnClickedButtonChooseall)
	ON_BN_CLICKED(IDC_BUTTON_CLEARALL, &CSUSTechHowManyGPADlg::OnBnClickedButtonClearall)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CSUSTechHowManyGPADlg::OnBnClickedButtonDefault)
	ON_NOTIFY(NM_CLICK, IDC_LIST_Score, &CSUSTechHowManyGPADlg::OnNMClickListScore)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_Score, &CSUSTechHowManyGPADlg::OnNMClickListScore)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_Score, &CSUSTechHowManyGPADlg::OnCustomdrawListScore)
	ON_WM_DRAWITEM()
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_Score, &CSUSTechHowManyGPADlg::OnLvnKeydownListScore)
END_MESSAGE_MAP()


// CSUSTechHowManyGPADlg 消息处理程序
BOOL CSUSTechHowManyGPADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, TRUE);			// 设置小图标

	// TODO: 在此添加额外的初始化代码
	dLogin.Create(IDD_DIALOG_LOGIN, this);

	CRect rect;
	studentScore.GetClientRect(&rect);
	studentScore.SetExtendedStyle(studentScore.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_DOUBLEBUFFER);
	studentScore.InsertColumn(0, "开课学期", LVCFMT_LEFT, rect.Width() * 1 / 5, 0);
	studentScore.InsertColumn(1, "课程编号", LVCFMT_LEFT, rect.Width() * 1 / 5, 1);
	studentScore.InsertColumn(2, "课程名称", LVCFMT_LEFT, rect.Width() * 1 / 5, 2);
	studentScore.InsertColumn(3, "等级制", LVCFMT_LEFT, rect.Width() * 2 / 15, 3);
	studentScore.InsertColumn(4, "百分制", LVCFMT_LEFT, rect.Width() * 2 / 15, 4);
	studentScore.InsertColumn(5, "学分", LVCFMT_LEFT, rect.Width() * 2 / 15, 5);

	char cListCtrlBackgroundPath[MAX_PATH] = { 0 };
	::GetModuleFileName(AfxGetInstanceHandle(), cListCtrlBackgroundPath, MAX_PATH);
	CString sPath;
	sPath.Format("res://%s/#2/#139", cListCtrlBackgroundPath);
	studentScore.SetBkImage(sPath.GetBuffer(sPath.GetLength()), TRUE);

	GetDlgItem(IDC_EDIT_GPA)->SetWindowText("0.00");

	dLogin.ShowWindow(SW_SHOW);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSUSTechHowManyGPADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSUSTechHowManyGPADlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CRect myrect;
		GetClientRect(&myrect);
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP_BACKGROUND);
		BITMAP bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap* pbmpOld = dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(0, 0, myrect.Width(), myrect.Height(), &dcMem, 0, 0,
			bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSUSTechHowManyGPADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CSUSTechHowManyGPADlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_GPA)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CSUSTechHowManyGPADlg::OnCustomdrawListScore(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;


	if (CDDS_PREPAINT == pNMCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pNMCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pNMCD->nmcd.dwDrawStage)
	{
		COLORREF  clrNewBkColor;
		int nItem = static_cast<int>(pNMCD->nmcd.dwItemSpec);
		if (nItem % 2)
		{
			clrNewBkColor = RGB(225, 225, 225);
		}
		else
		{
			clrNewBkColor = RGB(255, 255, 255);
		}

		pNMCD->clrTextBk = clrNewBkColor;
		*pResult = CDRF_DODEFAULT;
	}
}

LRESULT CSUSTechHowManyGPADlg::FreshScore(WPARAM iParam1, LPARAM iParam2)
{
	CString jwxtURL = "jwxt.sustech.edu.cn";
	CHttpConnection* pServer = dLogin.session.GetHttpConnection(jwxtURL, INTERNET_FLAG_SECURE, (INTERNET_PORT)443, NULL, NULL);
	CHttpFile* pfile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, "/jsxsd/kscj/cjcx_query", NULL, 1,
		NULL, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_DONT_CACHE);
	pfile->SendRequest();

	std::wregex regScore(L"<td align=\"left\">(.+?)</td>.*?<td align=\"left\">(.+?)</td>.*?<td>(.+?)</td>.*?<td>(.+?)</td>.*?<td>(.+?)</td>");
	std::wsmatch wsm;
	std::wstring::const_iterator iter;
	std::wstring::const_iterator iterEnd;
	CTime now = CTime::GetCurrentTime();

	for (int i = 2010 + (dLogin.inputAccount[3] - '0'), j = i + 1; j <= now.GetYear() + 1; i++, j++)
	{
		for (int k = 1; k <= 3; k++)
		{
			CString termNo;
			termNo.Format("%d-%d-%d", i, j, k);
			pfile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, "/jsxsd/kscj/cjcx_list?kksj=" + termNo, NULL, 1,
				NULL, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_DONT_CACHE);
			pfile->SendRequest();

			CString getCAS = "", strLine;
			while (pfile->ReadString(strLine))
				getCAS += strLine;
			wchar_t* pBuffer = dLogin.UTF8toWide(getCAS);
			std::wstring wstr = pBuffer;
			iter = wstr.begin();
			iterEnd = wstr.end();

			if (wstr.find(L"未查询到数据") != wstr.npos)
				continue;
			else
			{
				while (std::regex_search(iter, iterEnd, wsm, regScore))
				{
					studentScore.SetRedraw(FALSE);
					studentScore.InsertItem(0, termNo);
					studentScore.SetItemText(0, 1, (CString)((std::wstring)wsm[1]).c_str());
					studentScore.SetItemText(0, 2, (CString)((std::wstring)wsm[2]).c_str());
					studentScore.SetItemText(0, 3, (CString)((std::wstring)wsm[3]).c_str());
					studentScore.SetItemText(0, 4, (CString)((std::wstring)wsm[4]).c_str());
					studentScore.SetItemText(0, 5, (CString)((std::wstring)wsm[5]).c_str());
					studentScore.SetRedraw(TRUE);
					iter = wsm[0].second;
				}
			}
		}
	}
	return 0;
}

void CSUSTechHowManyGPADlg::SetStatus(int nItem, bool addOrSub)
{
	CString str = studentScore.GetItemText(nItem, 2);
	int aCredit = _ttoi(studentScore.GetItemText(nItem, 5));
	CString aRank = studentScore.GetItemText(nItem, 3);
	if (aRank != "P" && aRank != "W")
	{
		for (int i = 0; i < studentScore.GetItemCount(); i++)
		{
			if (str == studentScore.GetItemText(i, 2) && i != nItem && studentScore.GetCheck(i) &&
				studentScore.GetItemText(i, 3) != "P" && studentScore.GetItemText(i, 3) != "W")
			{
				credit += aCredit * (addOrSub ? -1 : 1);
				break;
			}
		}
		credit += aCredit * (addOrSub ? 1 : -1);
		GPASUM += aCredit * mapGPA[aRank] * (addOrSub ? 1 : -1);
		if (credit == 0)
			GetDlgItem(IDC_EDIT_GPA)->SetWindowText("0.00");
		else {
			CString str;
			str.Format("%.2lf", GPASUM / credit);
			GetDlgItem(IDC_EDIT_GPA)->SetWindowText(str);
		}
		UpdateData(FALSE);
	}
}

void CSUSTechHowManyGPADlg::OnNMClickListScore(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	studentScore.ScreenToClient(&point);

	UINT* nFlag = new UINT;
	int nItem = studentScore.HitTest(point, nFlag);
	if (*nFlag == LVHT_ONITEMSTATEICON) {
		BOOL bCheckState = studentScore.GetCheck(nItem);
		/*old state : checked && new state : unchecked */
		if (bCheckState)
		{
			SetStatus(nItem, FALSE);
		}
		/* old state : unchecked && new state : checked*/
		else
		{
			SetStatus(nItem, TRUE);
		}
	}
	delete nFlag;

	*pResult = 0;
}

void CSUSTechHowManyGPADlg::OnLvnKeydownListScore(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pLVKeyDow->wVKey == VK_SPACE)
	{
		POSITION p = studentScore.GetFirstSelectedItemPosition();
		for (unsigned int i = 0; i < studentScore.GetSelectedCount(); i++)
		{
			int nItem = studentScore.GetNextSelectedItem(p);

			BOOL bCheckState = studentScore.GetCheck(nItem);
			/*old state : checked && new state : unchecked */
			if (bCheckState)
			{
				studentScore.SetCheck(nItem, FALSE);
				SetStatus(nItem, FALSE);
			}
			/* old state : unchecked && new state : checked*/
			else
			{
				studentScore.SetCheck(nItem, TRUE);
				SetStatus(nItem, TRUE);
			}
		}
		*pResult = 1;
	}
	else
		*pResult = 0;
}

void CSUSTechHowManyGPADlg::OnBnClickedButtonChooseall()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < studentScore.GetItemCount(); i++)
	{
		if (!studentScore.GetCheck(i)) {
			studentScore.SetCheck(i, TRUE);
			SetStatus(i, TRUE);
		}
	}
}

void CSUSTechHowManyGPADlg::OnBnClickedButtonClearall()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < studentScore.GetItemCount(); i++)
	{
		if (studentScore.GetCheck(i)) {
			studentScore.SetCheck(i, FALSE);
			SetStatus(i, FALSE);
		}
	}
}

void CSUSTechHowManyGPADlg::OnBnClickedButtonDefault()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION p = studentScore.GetFirstSelectedItemPosition();
	BOOL allChecked = TRUE;
	for (unsigned int i = 0; i < studentScore.GetSelectedCount(); i++)
	{
		int nItem = studentScore.GetNextSelectedItem(p);
		allChecked = allChecked && studentScore.GetCheck(nItem);
	}
	p = studentScore.GetFirstSelectedItemPosition();
	for (unsigned int i = 0; i < studentScore.GetSelectedCount(); i++)
	{
		int nItem = studentScore.GetNextSelectedItem(p);
		if (allChecked) {
			studentScore.SetCheck(nItem, FALSE);
			SetStatus(nItem, FALSE);
		}
		else if (!studentScore.GetCheck(nItem))
		{
			studentScore.SetCheck(nItem, TRUE);
			SetStatus(nItem, TRUE);
		}
	}

}

void CSUSTechHowManyGPADlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDCtl == IDC_BUTTON_CHOOSEALL || nIDCtl == IDC_BUTTON_CLEARALL) {
		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);

		CString strText;
		((CButton*)GetDlgItem(nIDCtl))->GetWindowText(strText);
		dc.Draw3dRect(&(lpDrawItemStruct->rcItem), RGB(0, 0, 0), RGB(255, 255, 255));
		dc.FillSolidRect(&(lpDrawItemStruct->rcItem), RGB(50, 170, 165));
		DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),
			&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		dc.Detach();
	}
	else
		CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
