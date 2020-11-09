#pragma once



#include <Windows.h>
#include "TcAdsDef.h"
#include "TcAdsAPI.h"

#include <queue>                                           //

#pragma comment(lib,"TcAdsDll.lib")                        // 加载倍福的通讯驱动库

enum CMD_TYPE  { CMD_BOOL=0, CMD_LONG, CMD_FLOAT };       // PLC 命令参数类型

// PLC 命令结构体
typedef struct {
	bool     boolVal;
	long     longVal;
	float    floatVal;
	CMD_TYPE varType;             // 命令参数类型
	CHAR     name[64];            // PLC标签名称
	int      cmdID;               // 命令id

} PLCCMD, * PPLCCMD;


class CPLC
{
public:
	CPLC();
	virtual ~CPLC();

	long Open();
	long Close();
	long SetDeviceBool(LPCTSTR szDevice, bool bData);      // 写开关变量
	long SetDeviceFloat(LPCTSTR szDevice, float fData);    // 写浮点数
	long SetDeviceLong(LPCTSTR szDevice, long lData);      // 写整数
	long GetDeviceLong(LPCTSTR szDevice, long* lplData);   // 读long型变量
	long GetDeviceFloat(LPCTSTR szDevice, float* fpfData); // 读入浮点数
	long GetDeviceBool(LPCTSTR szDevice, bool* bpbData);   // 读入开关变量


	long ReadMachineState();             // 批量读入数据到指定的结构体,
	long WriteMotorPara();               // 写电机参数到PLC，（仅铺粉电机参数批量写入PLC），tjh
	
	CString  strNetID;                      // netid，用于显示

	long testSetBool(LPCTSTR szDevice, bool bData);
	long testSetLong(LPCTSTR szDevice, long bData);
	long testSetFloat(LPCTSTR szDevice, float bData);

// PLC命令在线程中执行，定义线程

	CWinThread* pThread;   
	static UINT _cdecl ThreadProcPLC(LPVOID lpParameter);         // PLC的命令线程,必须静态

	std::queue < PLCCMD > m_cmdQueue;                             // 命令队列
	
	PLCCMD  BuildCmd(int cmdID, LPCTSTR pName,CMD_TYPE cmdType, bool bval,long lval,float fval);  // 构造一个命令结构
	long    ExecCmd( PPLCCMD );                    // PLC 执行分支判断

private:
	bool _createAddr(LPTSTR  szNetId, int port);

private:

	long		 m_nErr;
	long		 m_nPort;
	AmsAddr      m_oAddr;                  // PLC设备的地址结构  netID 和port，tjh    
	PAmsAddr     m_pAddr;
	bool         m_bIsRemoteMode;		  // Ads通信模式:true：远程模式，false：本地模式

	//MachineState m_MachineState;
	//MotorPara	 m_MotorPara;           // 电机相关参数（仅铺粉电机用tjh），用PLC控制
	
public:
	// PLC启动，禁用
	bool m_bIsUse;
	

};

#define PLC_RESET               (1)
#define PLC_LIGHT               (2)
#define PLC_FILTER_BACKBLOW     (3)


#define RESET_PLC_SWITCH		_T("GVL.m_bReset")				 // PLC软复位开关
#define	LIGHTING_PLC_SWITCH		_T("MAIN.m_bSwitchOnLighting")	 // 照明开关
#define	START_FILTER_BACK_BLOW	_T("CYCLE.m_bFilter_Back_Blow")  // 启动滤芯反吹 1：启动，

