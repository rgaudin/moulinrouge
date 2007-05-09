// moulin-autorunDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CmoulinautorunDlg dialog
class CmoulinautorunDlg : public CDialog
{
// Construction
public:
	CmoulinautorunDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MOULINAUTORUN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton launch_btn;
public:
	CButton install_btn;
public:
	CButton help_btn;
public:
	CButton quit_btn;
public:
	afx_msg void OnBnClickedLaunch();
public:
	afx_msg void OnBnClickedInstall();
public:
	afx_msg void OnBnClickedHelp();
public:
	afx_msg void OnBnClickedQuit();
};
