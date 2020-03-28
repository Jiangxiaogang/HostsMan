#include "stdafx.h"
#include "HostsFile.h"
#include "MainApp.h"
#include "MainDlg.h"
#include "InputDlg.h"
#include "DialogHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_NOTIFYICON (WM_USER + 1)

CMainDlg::CMainDlg(CWnd* pParent)
: CDialog(CMainDlg::IDD, pParent)
{
	m_edit_changed = FALSE;
	m_edit_index   = 0;
}

void CMainDlg::CreateTrayIcon()
{
	memset(&m_nid, 0, sizeof(m_nid));
	m_nid.cbSize = sizeof(m_nid);
	m_nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	m_nid.hWnd = GetSafeHwnd();
	m_nid.uCallbackMessage = WM_NOTIFYICON;
	m_nid.hIcon = m_hIcon;
	strcpy(m_nid.szTip, "HostsMan");
	Shell_NotifyIcon(NIM_ADD, &m_nid);
}

void CMainDlg::DeleteTrayIcon()
{
	Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_STATUS, m_status);
}

static void EnumProfileProc(LPCTSTR name, LPVOID arg)
{
	int count;
	CListCtrl *list;
	list = (CListCtrl *)arg;
	if(strcmp(name, "系统默认") == 0)
	{
		count = 0;
	}
	else
	{
		count = list->GetItemCount();
	}
	list->InsertItem(0, name);
}

void CMainDlg::LoadProfile()
{
	EnumAppHosts(EnumProfileProc, &m_list1);
}

//初始化默认配置
void CMainDlg::InitProfile()
{
	UINT  size;
	size = GetSysHosts(m_text_buffer, APP_MAX_FILE_SIZE);
	SetAppHosts("系统默认", m_text_buffer, size);
}

//新建配置
int  CMainDlg::CreateProfile(LPCTSTR name)
{
	int count;
	if(CreateAppHosts(name))
	{
		count = m_list1.GetItemCount();
		m_list1.InsertItem(count, name);
		return count;
	}
	return -1;
}

//删除配置
BOOL CMainDlg::DeleteProfile(int index)
{
	CHAR name[APP_MAX_PATH];
	m_list1.GetItemText(index, 0, name, APP_MAX_PATH);
	TRACE("DeleteProfile: index=%d, name=%s\n", index, name);
	if(DeleteAppHosts(name))
	{
		m_list1.DeleteItem(index);
		return TRUE;
	}
	return FALSE;
}

//编辑配置
BOOL CMainDlg::EditProfile(int index)
{
	CHAR  name[APP_MAX_PATH];
	UINT  size;
	m_list1.GetItemText(index, 0, name, APP_MAX_PATH);
	size = GetAppHosts(name, m_text_buffer, APP_MAX_FILE_SIZE);
	SetDlgItemText(IDC_EDIT1, m_text_buffer);
	TRACE("EditProfile: index=%d, name=%s\n", index, name);
	return TRUE;
}

//修改配置
BOOL CMainDlg::SaveProfile(int index)
{
	TCHAR   name[APP_MAX_PATH];
	CString text;
	m_list1.GetItemText(index, 0, name, MAX_PATH);
	m_edit1.GetWindowText(text);
	TRACE("SaveProfile: index=%d, name=%s\n", index, name);
	return SetAppHosts(name, text, text.GetLength());
}

//应用配置
BOOL CMainDlg::ApplyProfile(void)
{
	int   i;
	UINT  size;
	CHAR  name[APP_MAX_PATH];
	size = 0;
	for(i=0; i<m_list1.GetItemCount(); i++)
	{
		if(m_list1.GetCheck(i))
		{
			m_list1.GetItemText(i, 0, name, APP_MAX_PATH);
			size += GetAppHosts(name, m_text_buffer + size, APP_MAX_FILE_SIZE - size);
			TRACE("ApplyProfile: %s, size=%d\n", name, size);
		}
	}
	SetSysHosts(m_text_buffer, size);
	FlushDNS();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_CONTEXTMENU()
	ON_MESSAGE(WM_NOTIFYICON, &CMainDlg::OnNotifyIcon)
	ON_COMMAND(IDM_NEW, &CMainDlg::OnCommandNew)
	ON_COMMAND(IDM_LOAD, &CMainDlg::OnCommandLoad)
	ON_COMMAND(IDM_RENAME, &CMainDlg::OnCommandRename)
	ON_COMMAND(IDM_DELETE, &CMainDlg::OnCommandDelete)
	ON_COMMAND(IDM_SHOW, &CMainDlg::OnCommandShow)
	ON_COMMAND(IDM_EXIT, &CMainDlg::OnCommandExit)
	ON_COMMAND(IDM_HELP, &CMainDlg::OnCommandHelp)
	ON_NOTIFY(NM_CLICK, IDC_STATUS, &CMainDlg::OnNMClickStatus)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMainDlg::OnNMDblclkList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMainDlg::OnNMRclickList1)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMainDlg::OnLvnItemChangedList1)
	ON_EN_CHANGE(IDC_EDIT1, &CMainDlg::OnEnChangeEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CMainDlg::OnEnKillfocusEdit1)

END_MESSAGE_MAP()

void CMainDlg::InitStatusBar()
{
	UINT table[8]={0};
	m_status.SetIndicators(table, 3);
	m_status.SetPaneInfo(0, 0,  SBPS_NORMAL, 100);
	m_status.SetPaneInfo(1, 0,  SBPS_NORMAL, 400);
	m_status.SetPaneInfo(2, 0,  SBPS_STRETCH, 0);
	m_status.SetPaneText(0, _T("版本:V1.0.0"));
	m_status.SetPaneText(1, _T("源码:https://gitee.com/kerndev/HostsMan"));
}

void CMainDlg::OnOK()
{
}

void CMainDlg::OnCancel()
{
	ShowWindow(SW_HIDE);
}

void CMainDlg::PostNcDestroy()
{
	DeleteTrayIcon();
	free(m_text_buffer);
	CDialog::PostNcDestroy();
}

BOOL CMainDlg::OnInitDialog()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_text_buffer = (CHAR *)malloc(APP_MAX_FILE_SIZE);
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);
	GetClientRect(&m_client_rect);

	RECT rc;
	m_list1.GetClientRect(&rc);
	m_list1.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_list1.InsertColumn(1, _T("配置名称"), LVCFMT_LEFT, rc.right);
	m_edit1.SetLimitText(APP_MAX_FILE_SIZE);

	CreateTrayIcon();
	InitStatusBar();
	InitProfile();
	LoadProfile();
	EditProfile(0);
	return TRUE; 
}

//托盘消息
LRESULT CMainDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	CMenu menu;
	CMenu *sub_menu;
	if(lParam == WM_LBUTTONDOWN)
	{
		ShowWindow(SW_SHOW);
		return 0;
	}
	if(lParam == WM_RBUTTONDOWN)
	{
		GetCursorPos(&pt);
		menu.LoadMenu(IDR_MENU);
		sub_menu = menu.GetSubMenu(1);
		sub_menu->TrackPopupMenu(TPM_LEFTALIGN, pt.x, pt.y, this);
		return 0;
	}
	return 0;
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	int  dx;
	int  dy;
	if(nType != SIZE_MINIMIZED)
	{
		dx = cx - m_client_rect.right;
		dy = cy - m_client_rect.bottom;
		m_client_rect.right = cx;
		m_client_rect.bottom= cy;

		LockWindowUpdate();
		MoveDlgItem(m_hWnd, IDC_LIST1, dx, dy, MDIF_RESIZE_H);
		MoveDlgItem(m_hWnd, IDC_EDIT1, dx, dy, MDIF_RESIZE_H|MDIF_RESIZE_W);
		MoveDlgItem(m_hWnd, IDC_STATUS,dx, dy, MDIF_RESIZE_W|MDIF_OFFSET_Y);
		UnlockWindowUpdate();
	}
}

void CMainDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 600;
	lpMMI->ptMinTrackSize.y = 400;
}

//单击选中某项
void CMainDlg::OnLvnItemChangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	//勾选复选框
	if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)))
	{ 
		ApplyProfile();
	}

	//取消复选框
	if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1))) 
	{ 
		ApplyProfile();
	}

	//变更高亮行
	if(!(pNMLV->uOldState & LVIS_SELECTED) && (pNMLV->uNewState & LVIS_SELECTED)) 
	{ 
		if(m_edit_changed)
		{
			m_edit_changed = FALSE;
			SaveProfile(m_edit_index);
		}
		m_edit_index = pNMLV->iItem;
		EditProfile(pNMLV->iItem);
	}
	*pResult = 0;
}

//鼠标左键单击状态栏
void CMainDlg::OnNMClickStatus(NMHDR *pNMHDR, LRESULT *pResult)
{
	theApp.OnHelp();
	*pResult = 0;
}

//鼠标左键双击列表
void CMainDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int   id;
	POINT pt;
	GetCursorPos(&pt);
	m_list1.ScreenToClient(&pt);
	id = m_list1.HitTest(CPoint(pt));
	if(id < 0)
	{
		OnCommandNew();
	}
	*pResult = 0;
}


//鼠标右键单击列表
void CMainDlg::OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int   id;
	POINT pt;
	POINT menu_pt;
	CMenu menu;
	CMenu *sub_menu;
	GetCursorPos(&pt);
	GetCursorPos(&menu_pt);
	menu.LoadMenu(IDR_MENU);
	sub_menu = menu.GetSubMenu(0);
	m_list1.ScreenToClient(&pt);
	id = m_list1.HitTest(CPoint(pt));
	if(id < 0)
	{
		sub_menu->EnableMenuItem(IDM_LOAD, MF_GRAYED);
		sub_menu->EnableMenuItem(IDM_RENAME, MF_GRAYED);
		sub_menu->EnableMenuItem(IDM_DELETE, MF_GRAYED);
	}
	sub_menu->TrackPopupMenu(TPM_LEFTALIGN, menu_pt.x, menu_pt.y, this);
	*pResult = 0;
}

//EDIT文本更新
void CMainDlg::OnEnChangeEdit1()
{
	m_edit_changed = TRUE;
}

//EDIT丢失焦点
void CMainDlg::OnEnKillfocusEdit1()
{
	if(m_edit_changed)
	{
		m_edit_changed = FALSE;
		if(!SaveProfile(m_edit_index))
		{
			MessageBox(_T("保存配置失败！请检查配置文件是否被占用！"), _T("操作失败"), MB_ICONWARNING);
		}
	}
}

//新建
void CMainDlg::OnCommandNew()
{
	int index;
	CInputDlg dlg;
	dlg.m_title = "新建配置文件";
	dlg.m_input = "新建配置文件";
	if(dlg.DoModal() == IDOK)
	{
		index = CreateProfile(dlg.m_input);
		if(index < 0)
		{
			MessageBox(_T("创建配置文件失败！请检查文件是否已存在！"), _T("操作失败"), MB_ICONWARNING);
		}
		else
		{
			m_edit_index = index;
			EditProfile(index);
		}
	}
}

//加载
void CMainDlg::OnCommandLoad()
{
	int  index;
	index = m_list1.GetSelectionMark();
	m_list1.SetCheck(index);
	ApplyProfile();
}

//重命名
void CMainDlg::OnCommandRename()
{
	int  index;
	char name[MAX_PATH];
	CInputDlg dlg;
	index = m_list1.GetSelectionMark();
	if(index >= 0)
	{
		m_list1.GetItemText(index, 0, name, MAX_PATH);
		dlg.m_title = "重命名";
		dlg.m_input = name;
		dlg.DoModal();
		if(RenameAppHosts(name, dlg.m_input))
		{
			m_list1.SetItemText(index, 0, dlg.m_input);
		}
		else
		{
			MessageBox(_T("重命名配置失败！请检查配置文件是否被占用！"), _T("操作失败"), MB_ICONWARNING);
		}
	}
}

//删除
void CMainDlg::OnCommandDelete()
{
	int  ret;
	int  index;
	index = m_list1.GetSelectionMark();
	if(m_edit_index == index)
	{
		MessageBox(_T("您无法删除正在编辑的配置！"), _T("操作无效"), MB_ICONWARNING);
		return;
	}
	if(index >= 0)
	{
		ret = MessageBox(_T("您确定要删除这个配置文件吗？"), _T("确认删除"), MB_ICONQUESTION|MB_OKCANCEL);
		if(ret != IDOK)
		{
			return;
		}
		if(!DeleteProfile(index))
		{
			MessageBox(_T("删除配置失败！请检查配置文件是否被占用！"), _T("操作失败"), MB_ICONWARNING);
			return;
		}
	}
}

//显示
void CMainDlg::OnCommandShow()
{
	ShowWindow(SW_SHOW);
}

//退出
void CMainDlg::OnCommandExit()
{
	EndDialog(IDOK);
}

//帮助
void CMainDlg::OnCommandHelp()
{
	theApp.OnHelp();
}
