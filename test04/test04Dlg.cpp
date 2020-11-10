
// test04Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "test04.h"
#include "test04Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Ctest04Dlg 对话框



Ctest04Dlg::Ctest04Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST04_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


}

void Ctest04Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CMD_LIST, m_cmdList);
}

BEGIN_MESSAGE_MAP(Ctest04Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Ctest04Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Ctest04Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Ctest04Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &Ctest04Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &Ctest04Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &Ctest04Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &Ctest04Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &Ctest04Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &Ctest04Dlg::OnBnClickedButton9)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Ctest04Dlg 消息处理程序

BOOL Ctest04Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CWinThread* pThread = AfxBeginThread(thePLC.ThreadProcPLC, this);  // 创建PLC线程，tjh


	if (thePLC.m_bIsUse)  SetTimer(1000, 3000, NULL);   // 

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Ctest04Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Ctest04Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Ctest04Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Ctest04Dlg::OnBnClickedButton1()
{

	long result  = thePLC.Open();
	if (result == 0) 
	{
		thePLC.m_bIsUse = false;
		printf("\n 链接PLC错误,错误号：%d \n", result);
		MessageBox(L"链接PLC错误！"); 
	    return;
	}
	thePLC.m_bIsUse = true;
	
	//IDC_STATIC_NETID
	SetDlgItemText(IDC_STATIC_NETID, thePLC.strNetID);  


}



void Ctest04Dlg::OnBnClickedButton2()
{
	//thePLC.SetDeviceBool(RESET_PLC_SWITCH, true);// PLC 复位命令

	PLCCMD  cmd = thePLC.BuildCmd(PLC_RESET, RESET_PLC_SWITCH, CMD_BOOL, true);
	thePLC.ExecCmd(cmd);

}

void Ctest04Dlg::OnBnClickedButton3()
{
	PLCCMD  cmd = thePLC.BuildCmd(PLC_LIGHT, LIGHTING_PLC_SWITCH, CMD_BOOL, true);
	thePLC.ExecCmd(cmd, true,false);
}

void Ctest04Dlg::OnBnClickedButton4()
{
	PLCCMD  cmd = thePLC.BuildCmd(PLC_FILTER_BACKBLOW, START_FILTER_BACK_BLOW, CMD_BOOL,true);
	thePLC.ExecCmd(cmd);
}



void Ctest04Dlg::OnBnClickedButton5()
{
	//thePLC.test(START_FILTER_BACK_BLOW, true);
	//thePLC.m_bIsUse = false;
	//PLCCMD  cmd = thePLC.BuildCmd(PLC_RESET, RESET_PLC_SWITCH, CMD_BOOL,true, NULL, NULL);
	PLCCMD  cmd = thePLC.BuildCmd(PLC_RESET, RESET_PLC_SWITCH, CMD_BOOL, true);
	thePLC.ExecCmd(cmd);
}


void Ctest04Dlg::OnBnClickedButton6()
{
	//static int count = 1000;
	PLCCMD  cmd = thePLC.BuildCmd(PLC_LIGHT, LIGHTING_PLC_SWITCH,CMD_BOOL,true);
	thePLC.ExecCmd(cmd);
}


void Ctest04Dlg::OnBnClickedButton7()
{

}


void Ctest04Dlg::OnBnClickedButton8()
{
	// 滤芯反吹命令
	PLCCMD  cmd = thePLC.BuildCmd(PLC_FILTER_BACKBLOW, START_FILTER_BACK_BLOW, CMD_BOOL, true);
	// 清空命令队列，强制执行当前命令。
	thePLC.ExecCmd(cmd, true, true);
}


void Ctest04Dlg::OnBnClickedButton9()
{
	// 批量返回数值
	if (!thePLC.m_bIsUse) {
		MessageBox(_T("没有连接PLC"));
		return;
	}
	PLCCMD cmd = thePLC.BuildCmd( PLC_MUTIL_STATE, MACHINESTATE, CMD_MUTIL, &thePLC.m_SysVals );
	thePLC.ExecCmd(cmd);

	
}


void Ctest04Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == 1000) {



		// 请求变量
		PLCCMD cmd = thePLC.BuildCmd(PLC_MUTIL_STATE, MACHINESTATE, CMD_MUTIL, &thePLC.m_SysVals);
		thePLC.ExecCmd(cmd);

		
		if (thePLC.m_bIsUse && thePLC.m_IsUpdate)
		{
			//IDC_CHECK1 ~22 1014--1035

			((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(thePLC.m_SysVals.m_bLaser ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(thePLC.m_SysVals.m_bHeater ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(thePLC.m_SysVals.m_bMotor ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(thePLC.m_SysVals.m_bAeration ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(thePLC.m_SysVals.m_bIndicator ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(thePLC.m_SysVals.m_bLighting ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(thePLC.m_SysVals.m_bDoor1 ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(thePLC.m_SysVals.m_bLock1 ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(thePLC.m_SysVals.m_bPLCError ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(thePLC.m_SysVals.m_bLaserError ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(thePLC.m_SysVals.m_bArGasError ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK12))->SetCheck(thePLC.m_SysVals.m_bAirGasError ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK13))->SetCheck(thePLC.m_SysVals.m_bWindMotorError ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK14))->SetCheck(thePLC.m_SysVals.m_bSpare1 ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK15))->SetCheck(thePLC.m_SysVals.m_bSpare2 ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK16))->SetCheck(thePLC.m_SysVals.m_bSpare5 ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK17))->SetCheck(thePLC.m_SysVals.m_bMotorLimit[0] ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK18))->SetCheck(thePLC.m_SysVals.m_bMotorLimit[1] ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK19))->SetCheck(thePLC.m_SysVals.m_bMotorLimit[2] ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK20))->SetCheck(thePLC.m_SysVals.m_bMotorLimit[3] ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK21))->SetCheck(thePLC.m_SysVals.m_bMotorLimit[4] ? BST_CHECKED : BST_UNCHECKED);
			((CButton*)GetDlgItem(IDC_CHECK22))->SetCheck(thePLC.m_SysVals.m_bMotorLimit[5] ? BST_CHECKED : BST_UNCHECKED);

			CString str;
			str.Format(_T("%5.2f"), thePLC.m_SysVals.m_fPartPosition); SetDlgItemTextW(IDC_EDIT1, str);
			str.Format(_T("%5.2f"), thePLC.m_SysVals.m_fFeedPosition); SetDlgItemTextW(IDC_EDIT2, str);
			str.Format(_T("%5.2f"), thePLC.m_SysVals.m_fCarrierPosition); SetDlgItemTextW(IDC_EDIT3, str);
			str.Format(_T("%5.2f"), thePLC.m_SysVals.m_fCurrentTem); SetDlgItemTextW(IDC_EDIT4, str);
			str.Format(_T("%5.2f"), thePLC.m_SysVals.m_fBaseTem); SetDlgItemTextW(IDC_EDIT5, str);
			str.Format(_T("%5.2f"), thePLC.m_SysVals.m_fPressure); SetDlgItemTextW(IDC_EDIT6, str);
			str.Format(_T("%5.2f"), thePLC.m_SysVals.m_fOxygenCont); SetDlgItemTextW(IDC_EDIT7, str);
			str.Format(_T("%5.2f"), thePLC.m_SysVals.m_filterPressure); SetDlgItemTextW(IDC_EDIT8, str);
			//thePLC.m_IsUpdate = false;
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}
