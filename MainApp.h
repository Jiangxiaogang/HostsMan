// EasyHost.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEasyHostApp:
// �йش����ʵ�֣������ EasyHost.cpp
class CMainApp : public CWinApp
{
public:
	CMainApp();

private:
	void InitContex();
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnHelp();
};

extern CMainApp theApp;