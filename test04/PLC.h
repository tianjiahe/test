#pragma once



#include <Windows.h>
#include "TcAdsDef.h"
#include "TcAdsAPI.h"

#include <queue>                                           //

#pragma comment(lib,"TcAdsDll.lib")                        // ���ر�����ͨѶ������


// * �ֽڣ�byte������Ϊ4�ı���,���ڱ����PLC�������ص���ֵ����Ҫ����PLC�ĶԱ����ռ��Ų���Ҫ�� 4�ֽڶ��䣬
typedef struct  tagMutilState
{
	bool m_bLaser;						// ����������״̬
	bool m_bHeater;						// ����������״̬
	bool m_bMotor;						// �������״̬
	bool m_bAeration;					// ͨ�翪��״̬

	bool m_bIndicator;					// ָʾ������״̬
	bool m_bLighting;					// ��������״̬
	bool m_bDoor1;						// ��������״̬ 1���رգ�0����
	bool m_bLock1;						// ��������������״̬ 1�����գ�0����

	bool m_bPLCError;					// PLC ����
	bool m_bLaserError;					// ����������
	bool m_bArGasError;					// ����������
	bool m_bAirGasError;				// ѹ����������

	bool m_bWindMotorError;				// ��Ƶ������
	bool m_bSpare1;						// ��������ط���
	bool m_bSpare2;						// ��������ط���
	bool m_bSpare3;						// ����

	bool m_bSpare4;						// ����
	bool m_bSpare5;						// ��о�������
	bool m_bMotorLimit[6];				// �����������λ��־��ֵΪ1��ʾ������λ
										// 0-���͸�����λ��1-���͸׸���λ
										// 2-�ϸ�����λ��  3-�ϸ׸���λ
										// 4-�̷�����λ��  5-�̷۸���λ
	int	m_nErrorCode;					// PLC�������

	float m_fPartPosition;          	// ���͸׵�λ�ã�
	float m_fFeedPosition;				// �ϸ׵�λ��
	float m_fCarrierPosition;			// �̷۵��λ��

										// ����Ϊ�����ı���
	float m_fCurrentTem;				// �������¶Ȳ���ֵ
	float m_fBaseTem;					// �����¶�1
	float m_fPressure;					// ������΢ѹ������
	float m_fOxygenCont;				// ������������������
	float m_filterPressure;				// ��оѹ��

} MutilState, pMutilState;


enum CMD_TYPE  { CMD_BOOL=0, CMD_LONG=1, CMD_FLOAT=2,CMD_MUTIL=3 };       // PLC �����������
typedef struct tagPLCCMD
{         // PLC ����ṹ��

	bool     boolVal;             // �·�����boolֵ
	long     longVal;             // �·�����longֵ
	float    floatVal;            // �·�����floatֵ
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


	template<typename T> 
	long GetMutilState(LPCTSTR szDevice, T *pMutilState); // �����������ݵ�ָ���Ľṹ��,
	
	long WriteMotorPara();                   // д���������PLC�������̷۵����������д��PLC����tjh
	
	CString  strNetID;                      // netid��������ʾ

	long testSetBool(LPCTSTR szDevice, bool bData);
	long testSetLong(LPCTSTR szDevice, long bData);
	long testSetFloat(LPCTSTR szDevice, float bData);

// PLC�������߳���ִ�У������߳�

	CWinThread* pThread;   
	static UINT _cdecl ThreadProcPLC(LPVOID lpParameter);         // PLC�������߳�,���뾲̬


	
	//PLCCMD  BuildCmd(int cmdID, LPCTSTR pName,CMD_TYPE cmdType,bool bval=true,long lval=0,float fval=0.0);  // ����һ������ṹ

	PLCCMD  BuildCmd(int cmdID, LPCTSTR pName, CMD_TYPE cmdType, bool  bval);  // 
	PLCCMD  BuildCmd(int cmdID, LPCTSTR pName, CMD_TYPE cmdType, long  lval);  // ����һ������ṹ,��������long
	PLCCMD  BuildCmd(int cmdID, LPCTSTR pName, CMD_TYPE cmdType, float fval);  // ����һ������ṹ����������float
	PLCCMD  BuildCmd(int cmdID, LPCTSTR pName, CMD_TYPE cmdType );  // ����һ������ṹ����������T

	long    ExecCmd(PLCCMD cmd, bool isAsync=true, bool isForce=false);         // ִ��һ��������󣬲������Ƿ��첽,�Ƿ�ǿ��

private:

	std::queue < PLCCMD > _cmdQueue;					// �������
	long _execCmd( PPLCCMD );							// PLC ִ�з�֧�ж�

	bool _createAddr(LPTSTR  szNetId, int port);


	long		 m_nErr;
	long		 m_nPort;
	AmsAddr      m_oAddr;                  // PLC�豸�ĵ�ַ�ṹ  netID ��port��tjh    
	PAmsAddr     m_pAddr;
	bool         m_bIsRemoteMode;		  // Adsͨ��ģʽ:true��Զ��ģʽ��false������ģʽ

	
	//MotorPara	 m_MotorPara;           // �����ز��������̷۵����tjh������PLC����
	
public:
	// PLC����������
	bool         m_bIsUse;
	MutilState   m_SysVals;            // �������ضԶ�ֵ
	bool         m_IsUpdate;           // �Ƿ��Ѿ����µ���������ֵ����������ʱ=flase��ִ�������=true
};


















#define PLC_RESET               (1)                             // PLC��λ
#define PLC_LIGHT               (2)                             // ���ڿ���
#define PLC_FILTER_BACKBLOW     (3)                             // ��о����
#define PLC_MUTIL_STATE         (4)                             // ������״ֵ̬


#define RESET_PLC_SWITCH		_T("GVL.m_bReset")				 // PLC��λ����
#define	LIGHTING_PLC_SWITCH		_T("MAIN.m_bSwitchOnLighting")	 // ��������
#define	START_FILTER_BACK_BLOW	_T("CYCLE.m_bFilter_Back_Blow")  // ������о���� 1��������
#define	MACHINESTATE			_T("GVL.M_State")				 // PLC��ǩ����������������



