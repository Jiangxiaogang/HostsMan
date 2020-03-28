#include "StdAfx.h"
#include "HostsFile.h"

static BOOL GetSysHostsPath(LPTSTR path, DWORD size)
{
	GetSystemDirectory(path, size);
	PathAppend(path, "drivers\\etc\\hosts");
	return TRUE;
}

static BOOL GetAppHostsPath(LPCTSTR name, LPTSTR path, DWORD size)
{
	GetCurrentDirectory(size, path);
	PathAppend(path, "etc");
	PathAppend(path, name);
	strcat(path, ".hosts");
	return TRUE;
}

static BOOL SetHostsFileText(LPCTSTR path, LPCTSTR text, DWORD size)
{
	CFile file;
	BOOL  ret;
	ret = file.Open(path, CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone);
	if(!ret)
	{
		return FALSE;
	}
	file.Write(text, size);
	file.Close();
	return TRUE;
}

static UINT GetHostsFileText(LPCTSTR path, LPTSTR text, DWORD size)
{
	CFile file;
	UINT  ret;
	ret = file.Open(path, CFile::modeRead|CFile::shareDenyNone);
	if(!ret)
	{
		return 0;
	}
	ret = file.Read(text, size - 1);
	text[ret] = 0x00;
	file.Close();
	return ret;
}

//创建用户配置
BOOL CreateAppHosts(LPCTSTR name)
{
	TCHAR path[1024];
	CFile file;
	GetAppHostsPath(name, path, 1024);
	if(PathFileExists(path))
	{
		return FALSE;
	}
	return SetHostsFileText(path, "#", 1);
}

BOOL DeleteAppHosts(LPCTSTR name)
{
	TCHAR path[1024];
	GetAppHostsPath(name, path, 1024);
	return DeleteFile(path);
}

BOOL RenameAppHosts(LPCTSTR name, LPCTSTR new_name)
{
	TCHAR path[1024];
	TCHAR new_path[1024];
	GetAppHostsPath(name, path, 1024);
	GetAppHostsPath(new_name, new_path, 1024);
	return MoveFile(path, new_path);
}

BOOL EnumAppHosts(void(*EnumProc)(LPCTSTR, LPVOID), LPVOID arg)
{
	TCHAR           path[1024];
	HANDLE          find;
	WIN32_FIND_DATA wfd;
	GetAppHostsPath("*", path, 1024);
	find = FindFirstFile(path, &wfd);
	if(find == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	PathRemoveExtension(wfd.cFileName);
	EnumProc(wfd.cFileName, arg);
	while(FindNextFile(find, &wfd))
	{
		PathRemoveExtension(wfd.cFileName);
		EnumProc(wfd.cFileName, arg);
	}
	FindClose(find);
	return TRUE;
}

BOOL SetAppHosts(LPCTSTR name, LPCTSTR text, UINT size)
{
	TCHAR path[1024];
	GetAppHostsPath(name, path, 1024);
	return SetHostsFileText(path, text, size);
}

UINT GetAppHosts(LPCTSTR name,LPTSTR text, UINT size)
{
	TCHAR path[1024];
	GetAppHostsPath(name, path, 1024);
	return GetHostsFileText(path, text, size);
}

BOOL SetSysHosts(LPCTSTR text, UINT size)
{
	TCHAR path[1024];
	GetSysHostsPath(path, 1024);
	return SetHostsFileText(path, text, size);
}

UINT GetSysHosts(LPTSTR text, UINT size)
{
	TCHAR path[1024];
	GetSysHostsPath(path, 1024);
	return GetHostsFileText(path, text, size);
}

BOOL FlushDNS(void)
{
	WinExec("ipconfig.exe /flushdns", SW_HIDE);
	return TRUE;
}
