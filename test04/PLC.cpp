#include "pch.h"
#include "PLC.h"
#include "test04Dlg.h"
#include "resource.h"
#include "test04.h"


CPLC::CPLC():m_nErr(0), m_nPort(0)
{
	m_pAddr = &m_oAddr;
	m_bIsRemoteMode = true;
	m_bIsUse = true;
}
CPLC::~CPLC() {

}

long CPLC::Open() 
{
	long result = 0;
	
	CIniReader iniReader(TEXT(".\\RpDevicePara.ini"));
	LPTSTR szNetID = iniReader.ReadString(_T("TwinCat ͨѶ����"), _T("AdsNetId"), _T("0.0.0.0.1.1"));
	
	strNetID = szNetID;           // ����netid�ַ���
	//m_nPort = AdsPortOpenEx();
	m_nPort = AdsPortOpen();
	if(m_nPort==0)	return 0;    // �򿪶˿�ʧ��

	if (m_bIsRemoteMode) {       // Զ��ģʽ
		result = _createAddr(szNetID, 851) ? 1 : 0; // ���ط� 0
		   
	}else{
		result = AdsGetLocalAddress(m_pAddr);        // ����ģʽ,���� 0
		if(result) cerr << "Error: AdsGetLocalAddress: " << result << '\n';
	}

	return result;
	
}

long CPLC::Close() 
{
	return AdsPortClose();
}

// �����첽ִ��Ч��
long CPLC::testSetBool(LPCTSTR szDevice, bool bData)
{
	printf( "�������ʼִ�С�����������%ls,״̬= %d \n",szDevice,bData);
	
	for (int i = 0; i < 10000; i++)
		for (int j = 0; j < 160000; j++);
			
	printf("ִ����ɡ�����\n");
	return 1;
}

long CPLC::testSetLong(LPCTSTR szDevice, long bData)
{
	printf("�������ʼִ�С�����%ls,����ֵ= %d\n", szDevice, bData);
	for (int i = 0; i < 10000; i++)
		for (int j = 0; j < 160000; j++)	;
		
	printf("ִ����ɡ�����\n");
	return 1;
}

long CPLC::testSetFloat(LPCTSTR szDevice, float bData)
{
	printf("�������ʼִ�С�����%ls ����ֵ = %5.2f\n", szDevice, bData);
	for (int i = 0; i < 10000; i++)
		for (int j = 0; j < 160000; j++);
			
	printf("ִ����ɡ�����\n");
	return 1;
}

long CPLC::SetDeviceBool(LPCTSTR szDevice, bool bData) 
{
	// д���ر���
	long result = 0;
	ULONG  lHdlVar;
	
	//�õ����
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar), &lHdlVar, sizeof(szDevice), (void *)szDevice);
	//��ADSдֵ
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(bData), &bData);
	//�ͷž��
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);

	if (result != 0)  printf("д����ֵ��PLC����%d\n ",result);
	return result;
}



long CPLC::SetDeviceFloat(LPCTSTR szDevice, float fData)
{
	long result = 0;
	ULONG     lHdlVar;

	//�õ����
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar), &lHdlVar, sizeof(szDevice), (void*)szDevice);
	//��ADSдֵ
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(fData), &fData);
	//�ͷž��
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);

	if (result != 0)  printf("д����ֵ��PLC����%d\n ", result);
	return result;
}

long CPLC::SetDeviceLong(LPCTSTR szDevice, long lData) 
{
	long result = 0;
	ULONG     lHdlVar;

	//�õ����
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar), &lHdlVar, sizeof(szDevice), (void*)szDevice);
	//��ADSдֵ
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(lData), &lData);
	//�ͷž��
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);

	if (result != 0)  printf("д����ֵ��PLC����%d\n ", result);
	return result;
}


long CPLC::GetDeviceLong(LPCTSTR szDevice, long* lplData) 
{
	long result = 0;
	ULONG     lHdlVar;

	//�õ����
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar),
		&lHdlVar, sizeof(szDevice), (void*)szDevice);
	if (result) goto err;
	//����ֵ
	result = AdsSyncReadReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(lplData), lplData);
	if (result) goto err;

	//�ͷž��
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);
	if (result) goto err;

	return 0;  // PLC ��д��������
err:
	if (result != 0)  printf("������ֵ��PLC����%d\n ", result);
	return result;
}


long CPLC::GetDeviceFloat(LPCTSTR szDevice, float* fpfData) 
{
	// ���뿪�ر���
	long result = 0;
	ULONG  lHdlVar;

	//�õ����
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar),
		&lHdlVar, sizeof(szDevice), (void*)szDevice);
	if (result) goto err;
	//����ֵ
	result = AdsSyncReadReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(fpfData), fpfData);
	if (result) goto err;
	//�ͷž��
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);
	if (result) goto err;

	return 0;  // PLC ��д��������
err:
	if (result != 0)  printf("������ֵ��PLC����%d\n ", result);
	return result;
}

long CPLC::GetDeviceBool(LPCTSTR szDevice, bool* bpbData)
{
	// ���뿪�ر���
	long result = 0;
	ULONG  lHdlVar;

	//�õ����
	result = AdsSyncReadWriteReq(m_pAddr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(lHdlVar),
		&lHdlVar, sizeof(szDevice), (void*)szDevice);
	if (result) goto err;
	//����ֵ
	result = AdsSyncReadReq(m_pAddr, ADSIGRP_SYM_VALBYHND, lHdlVar, sizeof(bpbData), bpbData);
	if (result) goto err;
	//�ͷž��
	result = AdsSyncWriteReq(m_pAddr, ADSIGRP_SYM_RELEASEHND, 0, sizeof(lHdlVar), &lHdlVar);
	if (result) goto err;
	
	return 0;  // PLC ��д��������
err:
	if (result != 0)  printf("������ֵ��PLC����%d\n ", result);
	return result;
}

long CPLC::ReadMachineState()
{
	return 0;             // �����������ݵ�ָ���Ľṹ��,
}

long CPLC::WriteMotorPara()
{
	return 0;        // д���������PLC�������̷۵����������д��PLC����tjh
}

// �� netid�ַ���������������ַ
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

// PLC ������߳�
UINT _cdecl CPLC::ThreadProcPLC(LPVOID lpParameter)
{
	Ctest04Dlg * pDlg = (Ctest04Dlg*)lpParameter; //��Ȼ��Ҫ���ݶ���ָ�룬��Ϊstatic�ؼ���û��thisָ��
	//int a = pDlg->m_nPort;
	CString str;
	pDlg->GetDlgItemTextW(IDC_STATIC_NETID,str);
	//AfxMessageBox(str);
	printf("PLC �����߳�����������\n");
	while (thePLC.m_bIsUse) {
        
		Sleep(300);
		// ��ʾ�����б�
		
		

		//thePLC.test(START_FILTER_BACK_BLOW, true);
		
		if (thePLC.m_cmdQueue.empty())  continue;
		
		PLCCMD cmd = thePLC.m_cmdQueue.front();
		
		str = cmd.name;
		pDlg->m_cmdList.AddString(str);

		thePLC.ExecCmd( &cmd );

		
		thePLC.m_cmdQueue.pop();
		
	}
	

	printf("PLC �����̹߳ر�...\n");        // �߳����е��������Զ��ر��߳�OK
	return 0;
}


long CPLC::ExecCmd( PPLCCMD pcmd ) 
{
	long result = 0;
	
	CString str(pcmd->name);
	//str = pcmd->name;

	switch ( pcmd->varType )
	{

	case CMD_BOOL:	
		//result = testSetBool((LPCTSTR)str, pcmd->boolVal);  break;
		result = SetDeviceBool((LPCTSTR)str, pcmd->boolVal); break;

	case CMD_LONG:	result = testSetLong((LPCTSTR)str, pcmd->longVal);break;

	case CMD_FLOAT:	result = testSetFloat((LPCTSTR)str, pcmd->floatVal);break;
    
	default:break;
	}
    
	return result;
}

// ����һ�������ṹ��
PLCCMD  CPLC::BuildCmd(int cmdID, const LPCTSTR pName, CMD_TYPE cmdType, bool bval=false, long lval=0, float fval=0.0f)
{
	PLCCMD  cmd;

	cmd.cmdID = cmdID;                       // ����ID��
	sprintf_s( cmd.name,"%ls", pName);       // PLC ��ǩ��
	cmd.varType = cmdType;
	// ��ͬ�������������
	if(cmd.varType==CMD_BOOL)	    cmd.boolVal  = bval;
	else if(cmd.varType==CMD_LONG)  cmd.longVal  = lval;
	else if(cmd.varType==CMD_FLOAT) cmd.floatVal = fval;

	return cmd;
}
