#include "stdafx.h"
#include "MainApp.h"
#include "InputDlg.h"

IMPLEMENT_DYNAMIC(CInputDlg, CDialog)

CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{

}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
END_MESSAGE_MAP()

void CInputDlg::OnOK()
{
	int ret;
	LPCH exclude="\\/:*?\"<>|";
	GetDlgItemText(IDC_EDIT1, m_input);
	ret = m_input.FindOneOf(exclude);
	if(ret >= 0)
	{
		CString text;
		text.Format("不能包含下列特殊符号！\r\n\\/:*?\"<>|");
		MessageBox(text, "无效输入", MB_ICONINFORMATION);
		return;
	}
	ret = m_input.GetLength();
	if(ret == 0)
	{
		MessageBox("请输入配置名称！", "无效输入", MB_ICONINFORMATION);
		return;
	}
	CDialog::OnOK();
}

BOOL CInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_title);
	SetDlgItemText(IDC_EDIT1, m_input);
	return TRUE;
}
