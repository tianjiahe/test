#pragma once



#include <Windows.h>
#include "TcAdsDef.h"
#include "TcAdsAPI.h"

#include <queue>                                           //

#pragma comment(lib,"TcAdsDll.lib")                        // ���ر�����ͨѶ������

enum CMD_TYPE  { CMD_BOOL=0, CMD_LONG, CMD_FLOAT };       // PLC �����������

// PLC ����ṹ��
typedef struct {
	bool     boolVal;
	long     longVal;
	float    floatVal;
	CMD_TYPE varType;             // �����������
	CHAR     name[64];            // PLC��ǩ����
	int      cmdID;               // ����id

} PLCCMD, * PPLCCMD;


class CPLC
{
public:
	CPLC();
	virtual ~CPLC();

	long Open();
	long Close();
	long SetDeviceBool(LPCTSTR szDevice, bool bData);      // д���ر���
	long SetDeviceFloat(LPCTSTR szDevice, float fData);    // д������
	long SetDeviceLong(LPCTSTR szDevice, long lData);      // д����
	long GetDeviceLong(LPCTSTR szDevice, long* lplData);   // ��long�ͱ���
	long GetDeviceFloat(LPCTSTR szDevice, float* fpfData); // ���븡����
	long GetDeviceBool(LPCTSTR szDevice, bool* bpbData);   // ���뿪�ر���


	long ReadMachineState();             // �����������ݵ�ָ���Ľṹ��,
	long WriteMotorPara();               // д���������PLC�������̷۵����������д��PLC����tjh
	
	CString  strNetID;                      // netid��������ʾ

	long testSetBool(LPCTSTR szDevice, bool bData);
	long testSetLong(LPCTSTR szDevice, long bData);
	long testSetFloat(LPCTSTR szDevice, float bData);

// PLC�������߳���ִ�У������߳�

	CWinThread* pThread;   
	static UINT _cdecl ThreadProcPLC(LPVOID lpParameter);         // PLC�������߳�,���뾲̬

	std::queue < PLCCMD > m_cmdQueue;                             // �������
	
	PLCCMD  BuildCmd(int cmdID, LPCTSTR pName,CMD_TYPE cmdType, bool bval,long lval,float fval);  // ����һ������ṹ
	long    ExecCmd( PPLCCMD );                    // PLC ִ�з�֧�ж�

private:
	bool _createAddr(LPTSTR  szNetId, int port);

private:

	long		 m_nErr;
	long		 m_nPort;
	AmsAddr      m_oAddr;                  // PLC�豸�ĵ�ַ�ṹ  netID ��port��tjh    
	PAmsAddr     m_pAddr;
	bool         m_bIsRemoteMode;		  // Adsͨ��ģʽ:true��Զ��ģʽ��false������ģʽ

	//MachineState m_MachineState;
	//MotorPara	 m_MotorPara;           // �����ز��������̷۵����tjh������PLC����
	
public:
	// PLC����������
	bool m_bIsUse;
	

};

#define PLC_RESET               (1)
#define PLC_LIGHT               (2)
#define PLC_FILTER_BACKBLOW     (3)


#define RESET_PLC_SWITCH		_T("GVL.m_bReset")				 // PLC��λ����
#define	LIGHTING_PLC_SWITCH		_T("MAIN.m_bSwitchOnLighting")	 // ��������
#define	START_FILTER_BACK_BLOW	_T("CYCLE.m_bFilter_Back_Blow")  // ������о���� 1��������

