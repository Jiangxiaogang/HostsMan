// EasyHostDlg.h : 头文件
#pragma once
#include "afxwin.h"
#include "afxext.h"

// CEasyHostDlg 对话框
class CMainDlg : public CDialog
{
// 构造
public:
	CMainDlg(CWnd* pParent = NULL);	// 标准构造函数
	enum { IDD = IDD_MAIN };

protected:


private:
	HICON m_hIcon;
	CEdit m_edit1;
	CListCtrl m_list1;
	CStatusBar m_status;
	CHAR *m_text_buffer;
	RECT m_client_rect;
	INT m_cur_apply;
	INT m_cur_edit;
	BOOL m_edit_changed;

public:
	void InitStatusBar();
	void InitProfile();
	void LoadProfile();
	BOOL UpdateProfile(int index);
	int  CreateProfile(LPCTSTR name);
	BOOL DeleteProfile(int index);
	void DisplayProfile(int index);
	BOOL ApplyProfile();

public:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCommandNew();
	afx_msg void OnCommandLoad();
	afx_msg void OnCommandRename();
	afx_msg void OnCommandDelete();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemChangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnKillfocusEdit1();
};
