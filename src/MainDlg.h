#pragma once
#include "afxwin.h"
#include "afxext.h"

class CMainDlg : public CDialog
{
public:
	CMainDlg(CWnd* pParent = NULL);
	enum { IDD = IDD_MAIN };
	DECLARE_MESSAGE_MAP()

private:
	HICON          m_hIcon;
	CEdit          m_edit1;
	CListCtrl      m_list1;
	CStatusBar     m_status;
	NOTIFYICONDATA m_nid;

private:
	CHAR *m_text_buffer;
	RECT  m_client_rect;
	INT   m_edit_index;
	BOOL  m_edit_changed;

private:
	void CreateTrayIcon();
	void DeleteTrayIcon();
	void InitStatusBar();
	void InitProfile();
	void LoadProfile();
	int  CreateProfile(LPCTSTR name);
	BOOL DeleteProfile(int index);
	BOOL SaveProfile(int index);
	BOOL EditProfile(int index);
	BOOL ApplyProfile();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	afx_msg void OnCommandNew();
	afx_msg void OnCommandLoad();
	afx_msg void OnCommandRename();
	afx_msg void OnCommandDelete();
	afx_msg void OnCommandShow();
	afx_msg void OnCommandExit();
	afx_msg void OnCommandHelp();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNMClickStatus(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemChangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnKillfocusEdit1();
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
};
