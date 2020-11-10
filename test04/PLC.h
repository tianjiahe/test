#pragma once



#include <Windows.h>
#include "TcAdsDef.h"
#include "TcAdsAPI.h"

#include <queue>                                           //

#pragma comment(lib,"TcAdsDll.lib")                        // 加载倍福的通讯驱动库


// * 字节（byte）数需为4的倍数,用于保存从PLC批量返回的数值，需要兼容PLC的对变量空间排布的要求 4字节对其，
typedef struct  tagMutilState
{
	bool m_bLaser;						// 激光器开关状态
	bool m_bHeater;						// 加热器开关状态
	bool m_bMotor;						// 电机开关状态
	bool m_bAeration;					// 通风开关状态

	bool m_bIndicator;					// 指示器开关状态
	bool m_bLighting;					// 照明开关状态
	bool m_bDoor1;						// 工作仓门状态 1：关闭，0：打开
	bool m_bLock1;						// 工作仓门锁开关状态 1：锁闭，0：打开

	bool m_bPLCError;					// PLC 错误
	bool m_bLaserError;					// 激光器报警
	bool m_bArGasError;					// 工作气报警
	bool m_bAirGasError;				// 压缩空气报警

	bool m_bWindMotorError;				// 变频器报警
	bool m_bSpare1;						// 舱体进气关反馈
	bool m_bSpare2;						// 舱体出气关反馈
	bool m_bSpare3;						// 备用

	bool m_bSpare4;						// 备用
	bool m_bSpare5;						// 滤芯反吹完成
	bool m_bMotorLimit[6];				// 三个电机的限位标志，值为1表示到达限位
										// 0-成型缸正限位，1-成型缸负限位
										// 2-料缸正限位，  3-料缸负限位
										// 4-铺粉正限位，  5-铺粉负限位
	int	m_nErrorCode;					// PLC错误代码

	float m_fPartPosition;          	// 成型缸的位置，
	float m_fFeedPosition;				// 料缸的位置
	float m_fCarrierPosition;			// 铺粉电机位置

										// 以下为新增的变量
	float m_fCurrentTem;				// 工作仓温度测量值
	float m_fBaseTem;					// 基板温度1
	float m_fPressure;					// 工作仓微压传感器
	float m_fOxygenCont;				// 工作仓氧含量传感器
	float m_filterPressure;				// 滤芯压力

} MutilState, pMutilState;


enum CMD_TYPE  { CMD_BOOL=0, CMD_LONG=1, CMD_FLOAT=2,CMD_MUTIL=3 };       // PLC 命令参数类型
typedef struct tagPLCCMD
{         // PLC 命令结构体

	bool     boolVal;             // 下发命令bool值
	long     longVal;             // 下发命令long值
	float    floatVal;            // 下发命令float值
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


	template<typename T> 
	long GetMutilState(LPCTSTR szDevice, T *pMutilState); // 批量读入数据到指定的结构体,
	
	long WriteMotorPara();                   // 写电机参数到PLC，（仅铺粉电机参数批量写入PLC），tjh
	
	CString  strNetID;                      // netid，用于显示

	long testSetBool(LPCTSTR szDevice, bool bData);
	long testSetLong(LPCTSTR szDevice, long bData);
	long testSetFloat(LPCTSTR szDevice, float bData);

// PLC命令在线程中执行，定义线程

	CWinThread* pThread;   
	static UINT _cdecl ThreadProcPLC(LPVOID lpParameter);         // PLC的命令线程,必须静态


	
	//PLCCMD  BuildCmd(int cmdID, LPCTSTR pName,CMD_TYPE cmdType,bool bval=true,long lval=0,float fval=0.0);  // 构造一个命令结构

	PLCCMD  BuildCmd(int cmdID, LPCTSTR pName, CMD_TYPE cmdType, bool  bval);  // 
	PLCCMD  BuildCmd(int cmdID, LPCTSTR pName, CMD_TYPE cmdType, long  lval);  // 构造一个命令结构,参数类型long
	PLCCMD  BuildCmd(int cmdID, LPCTSTR pName, CMD_TYPE cmdType, float fval);  // 构造一个命令结构，参数类型float
	PLCCMD  BuildCmd(int cmdID, LPCTSTR pName, CMD_TYPE cmdType );  // 构造一个命令结构，参数类型T

	long    ExecCmd(PLCCMD cmd, bool isAsync=true, bool isForce=false);         // 执行一个命令对象，参数：是否异步,是否强制

private:

	std::queue < PLCCMD > _cmdQueue;					// 命令队列
	long _execCmd( PPLCCMD );							// PLC 执行分支判断

	bool _createAddr(LPTSTR  szNetId, int port);


	long		 m_nErr;
	long		 m_nPort;
	AmsAddr      m_oAddr;                  // PLC设备的地址结构  netID 和port，tjh    
	PAmsAddr     m_pAddr;
	bool         m_bIsRemoteMode;		  // Ads通信模式:true：远程模式，false：本地模式

	
	//MotorPara	 m_MotorPara;           // 电机相关参数（仅铺粉电机用tjh），用PLC控制
	
public:
	// PLC启动，禁用
	bool         m_bIsUse;
	MutilState   m_SysVals;            // 批量返回对多值
	bool         m_IsUpdate;           // 是否已经更新到最新数据值，构造命令时=flase，执行命令后=true
};


















#define PLC_RESET               (1)                             // PLC复位
#define PLC_LIGHT               (2)                             // 舱内开灯
#define PLC_FILTER_BACKBLOW     (3)                             // 滤芯反吹
#define PLC_MUTIL_STATE         (4)                             // 读入多个状态值


#define RESET_PLC_SWITCH		_T("GVL.m_bReset")				 // PLC软复位开关
#define	LIGHTING_PLC_SWITCH		_T("MAIN.m_bSwitchOnLighting")	 // 照明开关
#define	START_FILTER_BACK_BLOW	_T("CYCLE.m_bFilter_Back_Blow")  // 启动滤芯反吹 1：启动，
#define	MACHINESTATE			_T("GVL.M_State")				 // PLC标签，读入多个参数返回



