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
	void InitContex();
	virtual BOOL InitInstance();
	afx_msg void OnHelp();
	DECLARE_MESSAGE_MAP()
};

extern CMainApp theApp;
