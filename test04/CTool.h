#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <map>

using namespace std;

// �����࣬��ʱ�����࣬
// MFC��ʹ�ÿ���̨���ڴ�ӡ������Ϣ,  https://www.cnblogs.com/-Donny/p/6308907.html
//
//
//
//---------------------------------------------------------------------------------

class CTool
{
public:
	static void OpenConsole();
	static void CloseConsole();

};


//---------------------------------------------------------------------------
//C++ʵ��INI�ļ���д��������
//INI�ļ����洢�ṹ
class ININode
{
public:
    ININode(string root, string key, string value)
    {
        this->root = root;
        this->key = key;
        this->value = value;
    }
    string root;
    string key;
    string value;
};
//��ֵ�Խṹ��
class SubNode
{
public:
    void InsertElement(string key, string value)
    {
        sub_node.insert(pair<string, string>(key, value));
    }
    map<string, string> sub_node;
};

//INI�ļ������࣬ ���ܷ����ַ�����
class CMyINI
{
public:
    CMyINI();
    ~CMyINI();

public:
    int    ReadINI(string path);                                                   //��ȡINI�ļ�
    string GetValue(string root, string key);                                   //�ɸ����ͼ���ȡֵ
    vector<ININode>::size_type GetSize() { return map_ini.size(); }                //��ȡINI�ļ��Ľ����
    vector<ININode>::size_type SetValue(string root, string key, string value);   //���ø����ͼ���ȡֵ
    int    WriteINI(string path);          //д��INI�ļ�
    void   Clear() { map_ini.clear(); }    //���
    void   Travel();                      //������ӡINI�ļ�
private:
    map<string, SubNode> map_ini;     //INI�ļ����ݵĴ洢����
};
//--------------------------------------------------------------------------------------------


/* ���C++ ��װ��ini����������ת������,  */

class CIniReader
{
public:
    CIniReader(LPCTSTR szFileName);
    int ReadInteger(LPCTSTR szSection, LPCTSTR szKey, int iDefaultValue);
    float ReadFloat(LPCTSTR szSection, LPCTSTR szKey, float fltDefaultValue);
    bool ReadBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolDefaultValue);
    LPTSTR ReadString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefaultValue);
private:
    TCHAR m_szFileName[255];
};

class CIniWriter
{
public:
    CIniWriter(LPCTSTR szFileName);
    void WriteInteger(LPCTSTR szSection, LPCTSTR szKey, int iValue);
    void WriteFloat(LPCTSTR szSection, LPCTSTR szKey, float fltValue);
    void WriteBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolValue);
    void WriteString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue);
private:
    TCHAR m_szFileName[255];
};













/*==================================================================
= �ļ���:CIniFile�ඨ���ļ�
= ��Ҫ����:���Զ�ȡ.�޸ı�����ֵ,���������µ���,�µı���
====================================================================*/

#include <afxtempl.h>
//�û��ӿ�˵��:�ڳ�Ա����SetVarStr��SetVarInt������,��iType������,������û��ƶ��Ĳ�����ini�ļ��в�����,
//���д���µı���.��iType��������,������û��ƶ��Ĳ�����ini�ļ��в�����,�Ͳ�д���µı���������ֱ�ӷ���FALSE;
class CIniFile
{
public:
    CIniFile();
    virtual ~CIniFile();
private:
    CIniFile(const CIniFile&);
    CIniFile& operator = (const CIniFile&);
public:
    //��������
    BOOL Create(const CString& strFileName);
   
    BOOL GetVarInt(const CString&, const CString&, int&);

    BOOL GetVarStr(const CString&, const CString&, CString&);

    BOOL SetVarInt(const CString&, const CString&, const int&, const int iType = 1);

    BOOL SetVarStr(const CString&, const CString&, const CString&, const int iType = 1);
private:
    BOOL GetVar(const CString&, const CString&, CString&);
    BOOL SetVar(const CString&, const CString&, const CString&, const int iType = 1);
    int  SearchLine(const CString&, const CString&);
private:
    CArray<CString> FileContainer;
    BOOL       bFileExsit;
    CStdioFile stfFile;
    CString    strInIFileName;
};

