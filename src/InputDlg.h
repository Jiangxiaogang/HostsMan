#pragma once

class CInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDlg)
	DECLARE_MESSAGE_MAP()

public:
	CInputDlg(CWnd* pParent = NULL); 
	enum { IDD = IDD_INPUT };

public:
	CString m_title;
	CString m_input;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Ö§³Ö
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
