#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <map>

using namespace std;

// 工具类，暂时不分类，
// MFC中使用控制台窗口打印调试信息,  https://www.cnblogs.com/-Donny/p/6308907.html
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
//C++实现INI文件读写完整代码
//INI文件结点存储结构
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
//键值对结构体
class SubNode
{
public:
    void InsertElement(string key, string value)
    {
        sub_node.insert(pair<string, string>(key, value));
    }
    map<string, string> sub_node;
};

//INI文件操作类， 仅能返回字符串类
class CMyINI
{
public:
    CMyINI();
    ~CMyINI();

public:
    int    ReadINI(string path);                                                   //读取INI文件
    string GetValue(string root, string key);                                   //由根结点和键获取值
    vector<ININode>::size_type GetSize() { return map_ini.size(); }                //获取INI文件的结点数
    vector<ININode>::size_type SetValue(string root, string key, string value);   //设置根结点和键获取值
    int    WriteINI(string path);          //写入INI文件
    void   Clear() { map_ini.clear(); }    //清空
    void   Travel();                      //遍历打印INI文件
private:
    map<string, SubNode> map_ini;     //INI文件内容的存储变量
};
//--------------------------------------------------------------------------------------------


/* 这个C++ 封装的ini操作，可以转换类型,  */

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
= 文件名:CIniFile类定义文件
= 主要功能:可以读取.修改变量数值,可以设置新的组,新的变量
====================================================================*/

#include <afxtempl.h>
//用户接口说明:在成员函数SetVarStr和SetVarInt函数中,当iType等于零,则如果用户制定的参数在ini文件中不存在,
//则就写入新的变量.当iType不等于零,则如果用户制定的参数在ini文件中不存在,就不写入新的变量，而是直接返回FALSE;
class CIniFile
{
public:
    CIniFile();
    virtual ~CIniFile();
private:
    CIniFile(const CIniFile&);
    CIniFile& operator = (const CIniFile&);
public:
    //创建函数
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

