// DIALOG_LOGIN.cpp: 实现文件
//

#include "pch.h"
#include "SUSTech_HowManyGPA.h"
#include "DIALOG_LOGIN.h"
#include "afxinet.h"
#include <regex>

// DIALOG_LOGIN 对话框

IMPLEMENT_DYNAMIC(DIALOG_LOGIN, CDialogEx)

DIALOG_LOGIN::DIALOG_LOGIN(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LOGIN, pParent)
	, inputAccount(_T(""))
	, inputPassword(_T(""))
{

}

DIALOG_LOGIN::~DIALOG_LOGIN()
{
}

void DIALOG_LOGIN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ACCOUNT, inputAccount);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, inputPassword);
}


BEGIN_MESSAGE_MAP(DIALOG_LOGIN, CDialogEx)
	ON_BN_CLICKED(IDOK, &DIALOG_LOGIN::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DIALOG_LOGIN::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// DIALOG_LOGIN 消息处理程序
BOOL DIALOG_LOGIN::OnInitDialog()
{
	newFont.CreatePointFont(120, "等线");
	GetDlgItem(IDC_STATIC_ACCOUNT)->SetFont(&newFont);
	GetDlgItem(IDC_STATIC_PASSWORD)->SetFont(&newFont);
	GetDlgItem(IDOK)->SetFont(&newFont);
	GetDlgItem(IDCANCEL)->SetFont(&newFont);
	return TRUE;
}


wchar_t* DIALOG_LOGIN::UTF8toWide(CString& str)
{
	char* pStr = (char*)str.GetBuffer(str.GetLength());
	int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0);
	wchar_t* pBuffer = (wchar_t*)malloc(nBufferSize * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, pStr, -1, pBuffer, nBufferSize);
	return pBuffer;
}


char* DIALOG_LOGIN::WidetoUTF8(CString& str)
{
	// CString to Unicode
	char* pStr = (char*)str.GetBuffer(str.GetLength());
	int nBufferSize = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, pStr, -1, NULL, 0);
	wchar_t* wszUTF8 = new wchar_t[nBufferSize + 1];
	memset(wszUTF8, 0, nBufferSize * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, pStr, -1, wszUTF8, nBufferSize);

	// Unicode to UTF-8
	nBufferSize = WideCharToMultiByte(CP_UTF8, 0, wszUTF8, -1, NULL, 0, NULL, NULL);
	char* szUTF8 = new char[nBufferSize + 1];
	memset(szUTF8, 0, nBufferSize + 1);
	WideCharToMultiByte(CP_UTF8, 0, wszUTF8, -1, szUTF8, nBufferSize, NULL, NULL);
	delete[] wszUTF8;
	return szUTF8;
}


void DIALOG_LOGIN::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (inputAccount.GetLength() != 8) {
		AfxMessageBox("请输入正确长度的学号！");
		return;
	}

	CString loginURL = "cas.sustech.edu.cn";
	CHttpConnection* pServer = session.GetHttpConnection(loginURL, INTERNET_FLAG_SECURE, (INTERNET_PORT)443, NULL, NULL);
	CHttpFile* pfile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, "/cas/login", NULL, 1,
		NULL, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_DONT_CACHE);
	pfile->SendRequest();
	DWORD dwRet;
	pfile->QueryInfoStatusCode(dwRet);
	if (dwRet != HTTP_STATUS_OK) {
		AfxMessageBox("网络异常！");
		return;
	}

	CString getCAS = "", strLine;
	while (pfile->ReadString(strLine))
		getCAS += strLine;
	wchar_t* pBuffer = UTF8toWide(getCAS);
	std::wstring wstr = pBuffer;

	CString postInformation;
	std::wregex regCASInformation(L"on\" value=\"(.+?)\"");
	std::wsmatch wsm;
	std::regex_search(wstr, wsm, regCASInformation);
	postInformation = (CString)("username=" + inputAccount + "&password=" + inputPassword + "&_eventId=submit&execution=");
	postInformation += ((std::wstring)wsm[1]).c_str();
	free(pBuffer);

	pfile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, "/cas/login?service=https%3A%2F%2Fjwxt.sustech.edu.cn%2Fjsxsd%2F", NULL, 1,
		NULL, NULL, INTERNET_FLAG_SECURE | INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_DONT_CACHE);
	char* wait_send = WidetoUTF8(postInformation);
	CString strHeaders = "Content-Type: application/x-www-form-urlencoded\r\n";
	pfile->SendRequest(strHeaders, wait_send, strlen(wait_send));
	pfile->QueryInfoStatusCode(dwRet);
	delete[] wait_send;

	getCAS = "", strLine;
	while (pfile->ReadString(strLine))
		getCAS += strLine;
	pBuffer = UTF8toWide(getCAS);
	wstr = pBuffer;

	if (dwRet == HTTP_STATUS_DENIED || (dwRet == HTTP_STATUS_OK && wstr.find(L"登录成功") == wstr.npos)) {
		AfxMessageBox("密码错误！");
		return;
	}
	else if (dwRet != HTTP_STATUS_OK) {
		AfxMessageBox("网络异常！");
		return;
	}
	CDialogEx::OnOK();
	::SendMessage(GetParent()->m_hWnd, WM_USER_FRESH_SCORE, (WPARAM)0, (LPARAM)0);
}


void DIALOG_LOGIN::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	::SendMessage(GetParent()->m_hWnd, WM_CLOSE, 0, 0);
}


void DIALOG_LOGIN::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CRect myrect;
	GetClientRect(&myrect);
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpBackground;
	bmpBackground.LoadBitmap(IDB_BITMAP_LOGIN);
	BITMAP bitmap;
	bmpBackground.GetBitmap(&bitmap);
	CBitmap* pbmpOld = dcMem.SelectObject(&bmpBackground);
	dc.StretchBlt(0, 0, myrect.Width(), myrect.Height(), &dcMem, 0, 0,
		bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	CDialogEx::OnPaint();
}


HBRUSH DIALOG_LOGIN::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_ACCOUNT || pWnd->GetDlgCtrlID() == IDC_STATIC_PASSWORD)
	{
		pDC->SetTextColor(RGB(20, 60, 80));
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void DIALOG_LOGIN::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDCtl == IDOK || nIDCtl == IDCANCEL) {
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
