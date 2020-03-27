// EasyHost.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CEasyHostApp:
// 有关此类的实现，请参阅 EasyHost.cpp
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