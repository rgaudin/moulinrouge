// moulin-autorunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "moulin-autorun.h"
#include "moulin-autorunDlg.h"
#include "lang_en.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmoulinautorunDlg dialog




CmoulinautorunDlg::CmoulinautorunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CmoulinautorunDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmoulinautorunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LAUNCH, launch_btn);
	DDX_Control(pDX, IDC_INSTALL, install_btn);
	DDX_Control(pDX, IDC_READ, help_btn);
	DDX_Control(pDX, IDC_QUIT, quit_btn);
}

BEGIN_MESSAGE_MAP(CmoulinautorunDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LAUNCH, &CmoulinautorunDlg::OnBnClickedLaunch)
	ON_BN_CLICKED(IDC_INSTALL, &CmoulinautorunDlg::OnBnClickedInstall)
	ON_BN_CLICKED(IDC_READ, &CmoulinautorunDlg::OnBnClickedHelp)
	ON_BN_CLICKED(IDC_QUIT, &CmoulinautorunDlg::OnBnClickedQuit)
END_MESSAGE_MAP()


// CmoulinautorunDlg message handlers

BOOL CmoulinautorunDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// change UI lang strings
	this->SetWindowTextW(main_win_title);
	launch_btn.SetWindowTextW(launch_btn_title);
	install_btn.SetWindowTextW(install_btn_title);
	help_btn.SetWindowTextW(help_btn_title);
	quit_btn.SetWindowTextW(quit_btn_title);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CmoulinautorunDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CmoulinautorunDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CmoulinautorunDlg::OnBnClickedLaunch()
{
	WinExec("..\\moulin\\moulin.exe", SW_SHOWMAXIMIZED);
}

void CmoulinautorunDlg::OnBnClickedInstall()
{
	WinExec("..\\installer\\moulin-setup.exe", SW_SHOWNORMAL);
}

void CmoulinautorunDlg::OnBnClickedHelp()
{
	LPCTSTR url = _T("..\\README.html");
	CString args;
	args.Format(_T("url.dll,FileProtocolHandler %s"), url);
	ShellExecute(NULL, _T("open"), _T("rundll32.exe"), args, NULL, SW_SHOWNORMAL);
}

void CmoulinautorunDlg::OnBnClickedQuit()
{
	this->DestroyWindow();
}
