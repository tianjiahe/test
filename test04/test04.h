
// test04.h: PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"		// 主符号


// Ctest04App:
// 有关此类的实现，请参阅 test04.cpp
//

class Ctest04App : public CWinApp
{
public:
	Ctest04App();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	// 使用控制台窗口调试
	bool m_bUseConsole;
	virtual int ExitInstance();

// 

};

extern Ctest04App theApp;

extern CPLC  thePLC;     // 加载 PLC 通讯模块
extern CTool theTool;    // 加载 工具类