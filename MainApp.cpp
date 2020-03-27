// EasyHost.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MainApp.h"
#include "MainDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEasyHostApp
BEGIN_MESSAGE_MAP(CMainApp, CWinApp)
	ON_COMMAND(ID_HELP, OnHelp)
END_MESSAGE_MAP()


// CEasyHostApp ����
CMainApp::CMainApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CEasyHostApp ����
CMainApp theApp;


// CEasyHostApp ��ʼ��
BOOL CMainApp::InitInstance()
{
	CWinApp::InitInstance();

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	InitContex();
	CMainDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}

void CMainApp::InitContex()
{
	TCHAR file_name[1024];
	GetModuleFileName(NULL, file_name, 1024);
	PathRemoveFileSpec(file_name);
	SetCurrentDirectory(file_name);
	CreateDirectory(_T("etc"), NULL);
}

void CMainApp::OnHelp()
{

}
