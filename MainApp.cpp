// EasyHost.cpp : 定义应用程序的类行为。
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


// CEasyHostApp 构造
CMainApp::CMainApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CEasyHostApp 对象
CMainApp theApp;


// CEasyHostApp 初始化
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
