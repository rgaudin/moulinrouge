// autorunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "autorun.h"
#include "autorunDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CautorunDlg dialog




CautorunDlg::CautorunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CautorunDlg::IDD, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CautorunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CautorunDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDLAUNCH, &CautorunDlg::OnBnClickedLaunch)
	ON_BN_CLICKED(IDINSTALL, &CautorunDlg::OnBnClickedInstall)
	ON_BN_CLICKED(IDHELP, &CautorunDlg::OnBnClickedHelp)
END_MESSAGE_MAP()


// CautorunDlg message handlers

BOOL CautorunDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CautorunDlg::OnPaint()
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
HCURSOR CautorunDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CautorunDlg::OnBnClickedLaunch()
{
	// Launch the moulin browser
	WinExec("..\\moulin\\moulin.exe", SW_SHOWMAXIMIZED);
}

void CautorunDlg::OnBnClickedInstall()
{
	WinExec("..\\installer\\moulin-setup.exe", SW_SHOWNORMAL);
}

void CautorunDlg::OnBnClickedHelp()
{
	LPCTSTR url = _T("..\\README.html");
	CString args;
	args.Format(_T("url.dll,FileProtocolHandler %s"), url);
	ShellExecute(NULL, _T("open"), _T("rundll32.exe"), args, NULL, SW_SHOWNORMAL);
}
