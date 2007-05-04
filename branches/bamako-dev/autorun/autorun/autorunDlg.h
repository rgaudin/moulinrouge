// autorunDlg.h : header file
//

#pragma once


// CautorunDlg dialog
class CautorunDlg : public CDialog
{
// Construction
public:
	CautorunDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AUTORUN_DIALOG };

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
	afx_msg void OnBnClickedLaunch();
public:
	afx_msg void OnBnClickedInstall();
public:
	afx_msg void OnBnClickedHelp();
};
