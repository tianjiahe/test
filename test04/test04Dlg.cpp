
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
		printf("\n 链接PLC错误,错误号：%d \n", result);
		MessageBox(L"链接PLC错误！");  
	    return;
	}
	
	//IDC_STATIC_NETID
	SetDlgItemText(IDC_STATIC_NETID, thePLC.strNetID);

}



void Ctest04Dlg::OnBnClickedButton2()
{
	//thePLC.SetDeviceBool(RESET_PLC_SWITCH, true);// PLC 复位命令

	PLCCMD  cmd = thePLC.BuildCmd(PLC_RESET, RESET_PLC_SWITCH, CMD_BOOL, true, 0, 0.0);
	thePLC.m_cmdQueue.push(cmd);

}

void Ctest04Dlg::OnBnClickedButton3()
{
	//thePLC.SetDeviceBool(LIGHTING_PLC_SWITCH, true); // PLC 开灯命令

	PLCCMD  cmd = thePLC.BuildCmd(PLC_LIGHT, LIGHTING_PLC_SWITCH, CMD_BOOL, true, 0, 0.0);
	thePLC.m_cmdQueue.push(cmd);
}

void Ctest04Dlg::OnBnClickedButton4()
{
	//thePLC.SetDeviceBool( START_FILTER_BACK_BLOW, true);  // 滤芯反吹
	PLCCMD  cmd = thePLC.BuildCmd(PLC_FILTER_BACKBLOW, START_FILTER_BACK_BLOW, CMD_BOOL, true, 0, 0.0);
	thePLC.m_cmdQueue.push(cmd);
}



void Ctest04Dlg::OnBnClickedButton5()
{
	//thePLC.test(START_FILTER_BACK_BLOW, true);
	//thePLC.m_bIsUse = false;

	PLCCMD  cmd = thePLC.BuildCmd(PLC_RESET, RESET_PLC_SWITCH, CMD_BOOL,true, 0, 0.0);
	thePLC.m_cmdQueue.push(cmd);

}


void Ctest04Dlg::OnBnClickedButton6()
{
	//static int count = 1000;
	PLCCMD  cmd = thePLC.BuildCmd(PLC_LIGHT, LIGHTING_PLC_SWITCH,CMD_BOOL,true, 0, 0.0);
	
	thePLC.m_cmdQueue.push(cmd);

}


void Ctest04Dlg::OnBnClickedButton7()
{

}


void Ctest04Dlg::OnBnClickedButton8()
{
	// 滤芯反吹命令
	PLCCMD  cmd = thePLC.BuildCmd(PLC_FILTER_BACKBLOW, START_FILTER_BACK_BLOW, CMD_BOOL, true, 0, 0.0);

	// 清空命令队列，强制执行当前命令。
	while (!thePLC.m_cmdQueue.empty())
	{
		thePLC.m_cmdQueue.pop();
	}
	thePLC.m_cmdQueue.push(cmd);
}
