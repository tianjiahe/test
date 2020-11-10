#include "pch.h"
#include "PLC.h"
#include "test04Dlg.h"
#include "resource.h"
#include "test04.h"


CPLC::CPLC():m_nErr(0), m_nPort(0)
{
	m_pAddr = &m_oAddr;
	m_bIsRemoteMode = true;
	m_bIsUse   = false;
	m_IsUpdate = false;   // 是否可刷新变量显示
}
CPLC::~CPLC() {

}

long CPLC::Open() 
{
	long result = 0;
	
	CIniReader iniReader(TEXT(".\\RpDevicePara.ini"));
	LPTSTR szNetID = iniReader.ReadString(_T("TwinCat 通讯参数"), _T("AdsNetId"), _T("0.0.0.0.1.1"));
	
	strNetID = szNetID;           // 保存netid字符串
	//m_nPort = AdsPortOpenEx();
	m_nPort = AdsPortOpen();
	if(m_nPort==0)	return 0;    // 打开端口失败

	if (m_bIsRemoteMode) {       // 远程模式
		result = _createAddr(szNetID, 851) ? 1 : 0; // 返回非 0
		   
	}else{
		result = AdsGetLocalAddress(m_pAddr);        // 本地模式,返回 0
		if(result) cerr << "Error: AdsGetLocalAddress: " << result << '\n';
	}

	return result;
	
}

long CPLC::Close() 
{
	return AdsPortClose();
}

// 测试异步执行效果
long CPLC::testSetBool(LPCTSTR szDevice, bool bData)
{
	printf( "测试 布尔命令开始执行...,参数：%ls,状态= %d \n",szDevice,bData);
	for (int i = 0; i < 10000; i++)
		for (int j = 0; j < 160000; j++);
			
	printf("执行完成。。。\n");
	return 1;
}

long CPLC::testSetLong(LPCTSTR szDevice, long bData)
{
	printf("整数命令开始执行...,参数：%ls,整数值= %d\n", szDevice, bData);
	for (int i = 0; i < 10000; i++)
		for (int j = 0; j < 160000; j++)	;
		
	printf("执行完成。。。\n");
	return 1;
}

long CPLC::testSetFloat(LPCTSTR szDevice, float bData)
{
	printf("浮点命令开始执行...,参数：%ls 浮点值 = %5.2f\n", szDevice, bData);
	for (int i = 0; i < 10000; i++)
		for (int j = 0; j < 160000; j++);
			
	printf("执行完成。。。\n");
	return 1;
}

long CPLC::SetDeviceBool(LPCTSTR szDevice, bool bData) 
{
	// 写开关变量
	long result = 0;
	ULONG  lHdlVar;
	
	//得到句柄
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar), &lHdlVar, sizeof(szDevice), (void *)szDevice);
	//向ADS写值
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(bData), &bData);
	//释放句柄
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);

	if (result != 0)  printf("写布尔值，PLC错误：%d\n ",result);
	return result;
}

long CPLC::SetDeviceFloat(LPCTSTR szDevice, float fData)
{
	long result = 0;
	ULONG     lHdlVar;

	//得到句柄
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar), &lHdlVar, sizeof(szDevice), (void*)szDevice);
	//向ADS写值
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(fData), &fData);
	//释放句柄
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);

	if (result != 0)  printf("写浮点值，PLC错误：%d\n ", result);
	return result;
}

long CPLC::SetDeviceLong(LPCTSTR szDevice, long lData) 
{
	long result = 0;
	ULONG     lHdlVar;

	//得到句柄
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar), &lHdlVar, sizeof(szDevice), (void*)szDevice);
	//向ADS写值
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(lData), &lData);
	//释放句柄
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);

	if (result != 0)  printf("写整数值，PLC错误：%d\n ", result);
	return result;
}

long CPLC::GetDeviceLong(LPCTSTR szDevice, long* lplData) 
{
	long result = 0;
	ULONG     lHdlVar;

	//得到句柄
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar),&lHdlVar, sizeof(szDevice), (void*)szDevice);
	if (result) goto err;
	//读出值
	result = AdsSyncReadReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(lplData), lplData);
	if (result) goto err;
	//释放句柄
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);
	if (result) goto err;
err:
	if (result != 0)  printf("读整数值，PLC错误：%d\n ", result);
	return result;
}

long CPLC::GetDeviceFloat(LPCTSTR szDevice, float* fpfData) 
{
	// 读入开关变量
	long result = 0;
	ULONG  lHdlVar;

	//得到句柄
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar),&lHdlVar, sizeof(szDevice), (void*)szDevice);
	if (result) goto err;
	//读出值
	result = AdsSyncReadReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(fpfData), fpfData);
	if (result) goto err;
	//释放句柄
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);
	if (result) goto err;
err:
	if (result != 0)  printf("读浮点值，PLC错误：%d\n ", result);
	return result;
}

long CPLC::GetDeviceBool(LPCTSTR szDevice, bool* bpbData)
{
	// 读入开关变量
	long result = 0;
	ULONG  lHdlVar;

	//得到句柄
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar),&lHdlVar, sizeof(szDevice), (void*)szDevice);
	if (result) goto err;
	//读出值
	//result = AdsSyncReadReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(bpbData), bpbData);
	result = AdsSyncReadReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(bool), bpbData); //?
	if (result) goto err;
	//释放句柄
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);
	if (result) goto err;
err:
	if (result != 0)  printf("读布尔值，PLC错误：%d\n ", result);
	return result;
}


// 批量读入数据到指定的结构体,从模板参数返回
template<typename T> 
long CPLC::GetMutilState(LPCTSTR szDevice, T *pMutilState )
{
	long result = 0;
	ULONG     lHdlVar;   // PLC 变量访问句柄

	//得到句柄
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar), &lHdlVar, sizeof(szDevice), (void *)szDevice);
	if (result) goto err;
	//读出值
	result = AdsSyncReadReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(*pMutilState), pMutilState);
	if (result)	goto err;
	//释放句柄
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);
	if (result)	goto err;
err:
	if (result != 0)  printf("读批量数值，PLC错误：%d\n ", result);
	
	return result;
}

long CPLC::WriteMotorPara()
{
	return 0;        // 写电机参数到PLC，（仅铺粉电机参数批量写入PLC），tjh
}

// 有 netid字符串，生产变量地址
bool CPLC::_createAddr( LPTSTR  szNetId, int port)
{
	CString cstr;
	int i = 0;
	for (i = 0; i < 6; i++)
	{
		AfxExtractSubString(cstr, (LPCTSTR)szNetId, i, '.');  //
		if (cstr == "")  	return false;

		m_oAddr.netId.b[i] = (UCHAR)(_ttoi(cstr));
	}
	m_oAddr.port = port; 

	return true;
}

// PLC 命令处理线程
UINT _cdecl CPLC::ThreadProcPLC(LPVOID lpParameter)
{
	Ctest04Dlg * pDlg = (Ctest04Dlg*)lpParameter; //仍然需要传递对象指针，因为static关键词没有this指针
	//int a = pDlg->m_nPort;
	CString str;
	pDlg->GetDlgItemTextW(IDC_STATIC_NETID,str);
	//AfxMessageBox(str);
	printf("PLC 命令线程启动...\n");

	while (thePLC.m_bIsUse) {
        
		Sleep(300);
		//thePLC.test(START_FILTER_BACK_BLOW, true);
		
		if (thePLC._cmdQueue.empty())  continue;
		
		PLCCMD cmd = thePLC._cmdQueue.front();   // 从命令队列中取命令对象

		pDlg->m_cmdList.AddString(str = cmd.name);          // 当前命令在列表上显示


		thePLC._execCmd( &cmd );                  // 执行命令


		thePLC._cmdQueue.pop();                   // 删除命令
		
	}
	printf("PLC 命令线程关闭...\n");              // 线程运行到结束，自动关闭线程OK
	return 0;
}

// 执行一个命令对象，参数：是否异步,是否立即执行
long  CPLC::ExecCmd(PLCCMD cmd, bool isAsync, bool isForce)
{   
	
	if (isForce) 
	{       // 强制执行，清空命令队列
		while (!_cmdQueue.empty())
			_cmdQueue.pop();
	}
	if (isAsync)	      _cmdQueue.push(cmd);     // 线程中异步执行命令对象，
	else   		 return   _execCmd( &cmd );        // 或直接执行命令对象
	return 0;
}

// 可在线程中执行命令，仿命令模式，接受命令对象参数，解析后执行，与发命令端解耦,
long CPLC::_execCmd( PPLCCMD pcmd ) 
{
	long result = 0;
	
	CString str(pcmd->name);   // 包装CString ，否则转LPCTSTR乱码

	switch ( pcmd->varType )
	{
	case CMD_BOOL:	result = SetDeviceBool((LPCTSTR)str, pcmd->boolVal); break;
	case CMD_LONG:	result = SetDeviceLong(  (LPCTSTR)str, pcmd->longVal); break;
	case CMD_FLOAT:	result = SetDeviceFloat((LPCTSTR)str, pcmd->floatVal); break;
	//case CMD_FLOAT:	result = testSetFloat( (LPCTSTR)str, pcmd->floatVal);break;
	case CMD_MUTIL: 
		result = GetMutilState((LPCTSTR)str, &m_SysVals);
		m_IsUpdate = true; // 数据已刷新，通知界面
		break;   // 返回的多个参数到内部结构中

	default:break;
	}
	return result;
}

// 构造一个命名结构体，仿命令模式，命令对象与命令发出端解耦
//PLCCMD  CPLC::BuildCmd(int cmdID, const LPCTSTR pName, CMD_TYPE cmdType, bool bval, long lval, float fval)
//{
//	PLCCMD  cmd;
//
//	cmd.cmdID = cmdID;                       // 命令ID号
//	sprintf_s( cmd.name,"%ls", pName);       // PLC 标签号
//	cmd.varType = cmdType;                   // 命令参数类型
//	// 不同的命令参数类型
//	if(     cmd.varType==CMD_BOOL)	cmd.boolVal  = bval;
//	else if(cmd.varType==CMD_LONG)  cmd.longVal  = lval;
//	else if(cmd.varType==CMD_FLOAT) cmd.floatVal = fval;
//	return cmd;
//}

PLCCMD  CPLC::BuildCmd(int cmdID, LPCTSTR pName, CMD_TYPE cmdType, bool  bval)
{
	PLCCMD  cmd;

	cmd.cmdID = cmdID;                       // 命令ID号
	sprintf_s(cmd.name, "%ls", pName);       // PLC 标签号
	cmd.varType = cmdType;                   // 命令参数类型
	cmd.boolVal = bval;
	return cmd;
}

PLCCMD  CPLC::BuildCmd(int cmdID, LPCTSTR pName, CMD_TYPE cmdType, long  bval)
{
	PLCCMD  cmd;

	cmd.cmdID = cmdID;                       // 命令ID号
	sprintf_s(cmd.name, "%ls", pName);       // PLC 标签号
	cmd.varType = cmdType;                   // 命令参数类型
	cmd.boolVal = bval;
	return cmd;
}

PLCCMD  CPLC::BuildCmd(int cmdID, LPCTSTR pName, CMD_TYPE cmdType, float  bval)
{
	PLCCMD  cmd;

	cmd.cmdID = cmdID;                       // 命令ID号
	sprintf_s(cmd.name, "%ls", pName);       // PLC 标签号
	cmd.varType = cmdType;                   // 命令参数类型
	cmd.boolVal = bval;
	return cmd;
}

PLCCMD  CPLC::BuildCmd(int cmdID, LPCTSTR pName, CMD_TYPE cmdType )
{
	PLCCMD  cmd;

	cmd.cmdID = cmdID;                       // 命令ID号
	sprintf_s(cmd.name, "%ls", pName);       // PLC 标签号
	cmd.varType = cmdType;                   // 命令参数类型

	m_IsUpdate = false;            // 返回数值集合当前内容无效，等异步执行完成后true 

	return cmd;
}