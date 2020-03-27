#pragma once

BOOL CreateAppHosts(LPCTSTR name);
BOOL DeleteAppHosts(LPCTSTR name);
BOOL RenameAppHosts(LPCTSTR name, LPCTSTR new_name);
BOOL EnumAppHosts(void(*EnumProc)(LPCTSTR, LPVOID), LPVOID arg);
BOOL SetAppHosts(LPCTSTR name, LPCTSTR text, UINT size);
UINT GetAppHosts(LPCTSTR name, LPTSTR text, UINT size);
BOOL SetSysHosts(LPCTSTR text, UINT size);
UINT GetSysHosts(LPTSTR text, UINT size);
BOOL FlushDNS(void);
