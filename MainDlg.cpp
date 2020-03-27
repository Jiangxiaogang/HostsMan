// EasyHostDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HostsFile.h"
#include "MainApp.h"
#include "MainDlg.h"
#include "InputDlg.h"
#include "DialogUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEasyHostDlg �Ի���
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_cur_edit = 0;
	m_cur_apply = 0;
	m_edit_changed = FALSE;
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
	if(strcmp(name, "ϵͳĬ��") == 0)
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

//��ʼ��Ĭ������
void CMainDlg::InitProfile()
{
	UINT  size;
	size = GetSysHosts(m_text_buffer, APP_MAX_FILE_SIZE);
	SetAppHosts("ϵͳĬ��", m_text_buffer, size);
}

//�½�����
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

//ɾ������
BOOL CMainDlg::DeleteProfile(int index)
{
	CHAR  name[APP_MAX_PATH];
	m_list1.GetItemText(index, 0, name, APP_MAX_PATH);
	TRACE("DeleteProfile: index=%d, name=%s\n", index, name);
	if(DeleteAppHosts(name))
	{
		m_list1.DeleteItem(index);
		return TRUE;
	}
	return FALSE;
}

//չʾ����
void CMainDlg::DisplayProfile(int index)
{
	CHAR  name[APP_MAX_PATH];
	UINT  size;
	m_list1.GetItemText(index, 0, name, APP_MAX_PATH);
	size = GetAppHosts(name, m_text_buffer, APP_MAX_FILE_SIZE);
	SetDlgItemText(IDC_EDIT1, m_text_buffer);
	TRACE("DisplayProfile: index=%d, name=%s\n", index, name);
}

//�޸�����
BOOL CMainDlg::UpdateProfile(int index)
{
	TCHAR   name[APP_MAX_PATH];
	CString text;

	m_list1.GetItemText(index, 0, name, MAX_PATH);
	m_edit1.GetWindowText(text);
	TRACE("UpdateProfile: index=%d, name=%s\n", index, name);
	return SetAppHosts(name, text, text.GetLength());
}


//Ӧ������
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
	ON_COMMAND(IDM_NEW, &CMainDlg::OnCommandNew)
	ON_COMMAND(IDM_LOAD, &CMainDlg::OnCommandLoad)
	ON_COMMAND(IDM_RENAME, &CMainDlg::OnCommandRename)
	ON_COMMAND(IDM_DELETE, &CMainDlg::OnCommandDelete)
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
	m_status.SetPaneInfo(1, 0,  SBPS_NORMAL, 100);
	m_status.SetPaneInfo(2, 0,  SBPS_STRETCH, 0);
	m_status.SetPaneText(0, _T("�汾:V1.0.0"));
	m_status.SetPaneText(1, _T("����:�ǳ��ض�"));
}

// CEasyHostDlg ��Ϣ�������
BOOL CMainDlg::OnInitDialog()
{
	m_text_buffer = (CHAR *)malloc(APP_MAX_FILE_SIZE);
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	GetClientRect(&m_client_rect);

	RECT rc;
	m_list1.GetClientRect(&rc);
	m_list1.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);
	m_list1.InsertColumn(1, _T("��������"), LVCFMT_LEFT, rc.right);
	m_edit1.SetLimitText(65536);

	InitStatusBar();
	InitProfile();
	LoadProfile();
	m_list1.SetSelectedColumn(0);
	DisplayProfile(0);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

//����ѡ��ĳ��
void CMainDlg::OnLvnItemChangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	//��ѡ��ѡ��
	if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)))
	{ 
		TRACE("Item %d is checked\n", pNMLV->iItem);
		ApplyProfile();
	}

	//ȡ����ѡ��
	if((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) && (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1))) 
	{ 
		TRACE("Item %d is unchecked\n", pNMLV->iItem);
		ApplyProfile();
	}

	//���������
	if(!(pNMLV->uOldState & LVIS_SELECTED) && (pNMLV->uNewState & LVIS_SELECTED)) 
	{ 
		TRACE("Item %d is selected\n", pNMLV->iItem); 
		DisplayProfile(pNMLV->iItem);
	}
	*pResult = 0;
}

//������˫���¼�
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


//����Ҽ������¼�
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

//�½�
void CMainDlg::OnCommandNew()
{
	int index;
	CInputDlg dlg;
	dlg.SetTitle(_T("�½������ļ�"));
	dlg.SetInput(_T("�½������ļ�"));
	if(dlg.DoModal() == IDOK)
	{
		index = CreateProfile(dlg.m_input);
		if(index < 0)
		{
			MessageBox(_T("���������ļ�ʧ�ܣ������ļ��Ƿ��Ѵ��ڣ�"), _T("����ʧ��"), MB_ICONWARNING);
		}
		else
		{
			DisplayProfile(index);
		}
	}
}

//����
void CMainDlg::OnCommandLoad()
{
	int  index;
	index = m_list1.GetSelectionMark();
	m_list1.SetCheck(index);
	ApplyProfile();
}

//������
void CMainDlg::OnCommandRename()
{
	int  index;
	char name[MAX_PATH];
	CInputDlg dlg;
	index = m_list1.GetSelectionMark();
	if(index >= 0)
	{
		m_list1.GetItemText(index, 0, name, MAX_PATH);
		dlg.SetTitle(_T("������"));
		dlg.SetInput(name);
		dlg.DoModal();
		if(RenameAppHosts(name, dlg.m_input))
		{
			m_list1.SetItemText(index, 0, dlg.m_input);
		}
		else
		{
			MessageBox(_T("����������ʧ�ܣ����������ļ��Ƿ�ռ�ã�"), _T("����ʧ��"), MB_ICONWARNING);
		}
	}
}

//ɾ��
void CMainDlg::OnCommandDelete()
{
	int  ret;
	int  index;
	char name[MAX_PATH];
	index = m_list1.GetSelectionMark();
	if(index >= 0)
	{
		ret = MessageBox(_T("��ȷ��Ҫɾ����������ļ���"), _T("ȷ��ɾ��"), MB_ICONQUESTION|MB_OKCANCEL);
		if(ret != IDOK)
		{
			return;
		}
		m_list1.GetItemText(index, 0, name, MAX_PATH);
		if(DeleteAppHosts(name))
		{
			m_list1.DeleteItem(index);
			if(m_cur_edit == index)
			{
				m_list1.SetSelectionMark(0);
			}
		}
		else
		{
			MessageBox(_T("ɾ������ʧ�ܣ����������ļ��Ƿ�ռ�ã�"), _T("����ʧ��"), MB_ICONWARNING);
		}
	}
}

//�ı�����
void CMainDlg::OnEnChangeEdit1()
{
	m_edit_changed = TRUE;
	OutputDebugString("Edit\n");
}

void CMainDlg::OnEnKillfocusEdit1()
{
	int index;
	if(m_edit_changed)
	{
		m_edit_changed = FALSE;
		index = m_list1.GetSelectionMark();
		if(!UpdateProfile(index))
		{
			MessageBox(_T("��������ʧ�ܣ����������ļ��Ƿ�ռ�ã�"), _T("����ʧ��"), MB_ICONWARNING);
		}
	}
}

void CMainDlg::OnOK()
{
}
