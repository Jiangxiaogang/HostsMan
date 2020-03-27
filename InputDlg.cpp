// NewDialog.cpp : 实现文件
#include "stdafx.h"
#include "MainApp.h"
#include "InputDlg.h"

// CNewDialog 对话框
IMPLEMENT_DYNAMIC(CInputDlg, CDialog)

CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDlg::IDD, pParent)
{

}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CInputDlg::SetTitle(LPCTSTR text)
{
	m_title = text;
}

void CInputDlg::SetInput(LPCTSTR text)
{
	m_input = text;
}

BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
END_MESSAGE_MAP()

void CInputDlg::OnOK()
{
	int ret;
	GetDlgItemText(IDC_EDIT1, m_input);
	ret = m_input.FindOneOf("/*^=\\");
	if(ret >= 0)
	{
		MessageBox(_T("名称内不能包含特殊符号！"), _T("名称无效"));
		return;
	}
	ret = m_input.GetLength();
	if(ret == 0)
	{
		MessageBox(_T("请输入配置名称！"), _T("名称无效"));
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
