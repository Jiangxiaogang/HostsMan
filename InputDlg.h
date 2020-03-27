#pragma once


// CNewDialog �Ի���

class CInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CInputDlg();
	void SetInput(LPCTSTR text);
	void SetTitle(LPCTSTR text);
	enum { IDD = IDD_NEW };

public:
	CString m_title;
	CString m_input;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
