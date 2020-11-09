#include "pch.h"
#include "CTool.h"


void CTool::OpenConsole() 
{
	FILE* stream1;
	AllocConsole();
	
	freopen_s( &stream1, "CON", "r", stdin);
	freopen_s( &stream1, "CON", "w", stdout);
	freopen_s( &stream1, "CON", "w", stderr);
	cout << "启动控制台..."<<endl;
}
void CTool::CloseConsole()
{
	cout << "关闭控制台" << endl;

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

	FreeConsole();

}

//INI文件操作类,实现

CMyINI::CMyINI()
{
}
CMyINI::~CMyINI()
{
}
//************************************************************************
// 函数名称:    TrimString
// 访问权限:    public 
// 创建日期:        2017/01/05
// 创 建 人:       
// 函数说明:        去除空格
// 函数参数:    string & str    输入的字符串
// 返 回 值:   std::string &   结果字符串
//************************************************************************
string& TrimString(string& str)
{
	string::size_type pos = 0;
	while (str.npos != (pos = str.find(" ")))
		str = str.replace(pos, pos + 1, "");
	return str;
}

//************************************************************************
// 函数名称:    ReadINI
// 访问权限:    public 
// 创建日期:        2017/01/05
// 创 建 人:       
// 函数说明:        读取INI文件，并将其保存到map结构中
// 函数参数:    string path INI文件的路径
// 返 回 值:   int
//************************************************************************
int CMyINI::ReadINI(string path)
{
    ifstream in_conf_file(path.c_str());
    if (!in_conf_file) return 0;
    string str_line = "";
    string str_root = "";
    vector<ININode> vec_ini;
    while (getline(in_conf_file, str_line))
    {
        string::size_type left_pos = 0;
        string::size_type right_pos = 0;
        string::size_type equal_div_pos = 0;
        string str_key = "";
        string str_value = "";
        if ((str_line.npos != (left_pos = str_line.find("["))) && (str_line.npos != (right_pos = str_line.find("]"))))
        {
            //cout << str_line.substr(left_pos+1, right_pos-1) << endl;
            str_root = str_line.substr(left_pos + 1, right_pos - 1);
        }

        if (str_line.npos != (equal_div_pos = str_line.find("=")))
        {
            str_key = str_line.substr(0, equal_div_pos);
            str_value = str_line.substr(equal_div_pos + 1, str_line.size() - 1);
            str_key = TrimString(str_key);
            str_value = TrimString(str_value);
            //cout << str_key << "=" << str_value << endl;
        }

        if ((!str_root.empty()) && (!str_key.empty()) && (!str_value.empty()))
        {
            ININode ini_node(str_root, str_key, str_value);
            vec_ini.push_back(ini_node);
            //cout << vec_ini.size() << endl;
        }
    }
    in_conf_file.close();
    in_conf_file.clear();

    //vector convert to map
    map<string, string> map_tmp;
    for (vector<ININode>::iterator itr = vec_ini.begin(); itr != vec_ini.end(); ++itr)
    {
        map_tmp.insert(pair<string, string>(itr->root, ""));
    }   //提取出根节点
    for (map<string, string>::iterator itr = map_tmp.begin(); itr != map_tmp.end(); ++itr)
    {
#ifdef INIDEBUG
        cout << "根节点： " << itr->first << endl;
#endif  //INIDEBUG
        SubNode sn;
        for (vector<ININode>::iterator sub_itr = vec_ini.begin(); sub_itr != vec_ini.end(); ++sub_itr)
        {
            if (sub_itr->root == itr->first)
            {
#ifdef INIDEBUG
                cout << "键值对： " << sub_itr->key << "=" << sub_itr->value << endl;
#endif  //INIDEBUG
                sn.InsertElement(sub_itr->key, sub_itr->value);
            }
        }
        map_ini.insert(pair<string, SubNode>(itr->first, sn));
    }
    return 1;
}

//************************************************************************
// 函数名称:    GetValue
// 访问权限:    public 
// 创建日期:        2017/01/05
// 创 建 人:       
// 函数说明:        根据给出的根结点和键值查找配置项的值
// 函数参数:    string root     配置项的根结点
// 函数参数:    string key      配置项的键
// 返 回 值:   std::string     配置项的值
//************************************************************************
string CMyINI::GetValue(string root, string key)
{
    map<string, SubNode>::iterator itr = map_ini.find(root);
    map<string, string>::iterator sub_itr = itr->second.sub_node.find(key);
    if (!(sub_itr->second).empty())
        return sub_itr->second;
    return "";
}


//************************************************************************
// 函数名称:    WriteINI
// 访问权限:    public 
// 创建日期:        2017/01/05
// 创 建 人:       
// 函数说明:        保存XML的信息到文件中
// 函数参数:    string path INI文件的保存路径
// 返 回 值:   int
//************************************************************************
int CMyINI::WriteINI(string path)
{
    ofstream out_conf_file(path.c_str());
    if (!out_conf_file)
        return -1;
    //cout << map_ini.size() << endl;
    for (map<string, SubNode>::iterator itr = map_ini.begin(); itr != map_ini.end(); ++itr)
    {
        //cout << itr->first << endl;
        out_conf_file << "[" << itr->first << "]" << endl;
        for (map<string, string>::iterator sub_itr = itr->second.sub_node.begin(); sub_itr != itr->second.sub_node.end(); ++sub_itr)
        {
            //cout << sub_itr->first << "=" << sub_itr->second << endl;
            out_conf_file << sub_itr->first << "=" << sub_itr->second << endl;
        }
    }

    out_conf_file.close();
    out_conf_file.clear();

    return 1;
}

//************************************************************************
// 函数名称:    SetValue
// 访问权限:    public 
// 创建日期:        2017/01/05
// 创 建 人:       
// 函数说明:        设置配置项的值
// 函数参数:    string root     配置项的根节点
// 函数参数:    string key      配置项的键
// 函数参数:    string value    配置项的值
// 返 回 值:   std::vector<ININode>::size_type   
//************************************************************************
vector<ININode>::size_type CMyINI::SetValue(string root, string key, string value)
{
    map<string, SubNode>::iterator itr = map_ini.find(root);  //查找
    if (map_ini.end() != itr)
    {
        //itr->second.sub_node.insert(pair<string, string>(key, value));
        itr->second.sub_node[key] = value;
    }   //根节点已经存在了，更新值
    else
    {
        SubNode sn;
        sn.InsertElement(key, value);
        map_ini.insert(pair<string, SubNode>(root, sn));
    }   //根节点不存在，添加值

    return map_ini.size();
}

//************************************************************************
// 函数名称:    Travel
// 访问权限:    public 
// 创建日期:        2017/01/05
// 创 建 人:       
// 函数说明:        遍历打印INI文件
// 返 回 值:   void
//************************************************************************
void CMyINI::Travel()
{
    for (map<string, SubNode>::iterator itr = this->map_ini.begin(); itr != this->map_ini.end(); ++itr)
    {
        //root
        cout << "[" << itr->first << "]" << endl;
        for (map<string, string>::iterator itr1 = itr->second.sub_node.begin(); itr1 != itr->second.sub_node.end();
            ++itr1)
        {
            cout << " " << itr1->first << " = " << itr1->second << endl;
        }
    }

}

//读写测试
/*
CMyINI* p = new CMyINI();
p->ReadINI("Setting.ini");
cout << "\n原始INI文件内容：" << std::endl;
p->Travel();
p->SetValue("setting", "hehe", "eheh");
cout << "\n增加节点之后的内容：" << std::endl;
p->Travel();
cout << "\n修改节点之后的内容：" << std::endl;
p->SetValue("kk", "kk", "2");
p->Travel();
p->WriteINI("Setting.ini");
*/



/* 这个 C++ 封装的ini操作，可以转换类型,  */


CIniReader::CIniReader(LPCTSTR szFileName)
{
    memset(m_szFileName, 0x00, sizeof(m_szFileName));
    memcpy(m_szFileName, szFileName, _tcslen(szFileName) * sizeof(TCHAR));
}
int CIniReader::ReadInteger(LPCTSTR szSection, LPCTSTR szKey, int iDefaultValue)
{
    int iResult = GetPrivateProfileInt(szSection, szKey, iDefaultValue, m_szFileName);
    return iResult;
}
float CIniReader::ReadFloat(LPCTSTR szSection, LPCTSTR szKey, float fltDefaultValue)
{
    TCHAR szResult[255];
    TCHAR szDefault[255];
    float fltResult;
    _stprintf_s(szDefault, 255, TEXT("%f"), fltDefaultValue);
    GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
    fltResult = (float)_tstof(szResult);
    return fltResult;
}
bool CIniReader::ReadBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolDefaultValue)
{
    TCHAR szResult[255];
    TCHAR szDefault[255];
    bool bolResult;
    _stprintf_s(szDefault, 255, TEXT("%s"), bolDefaultValue ? TEXT("True") : TEXT("False"));
    GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName);
    bolResult = (_tcscmp(szResult, TEXT("True")) == 0 ||
        _tcscmp(szResult, TEXT("true")) == 0) ? true : false;
    return bolResult;
}
LPTSTR CIniReader::ReadString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefaultValue)
{
    LPTSTR szResult = new TCHAR[255];
    memset(szResult, 0x00, sizeof(szResult));
    GetPrivateProfileString(szSection, szKey, szDefaultValue, szResult, 255, m_szFileName);
    return szResult;
}


CIniWriter::CIniWriter(LPCTSTR szFileName)
{
    memset(m_szFileName, 0x00, sizeof(m_szFileName));
    memcpy(m_szFileName, szFileName, _tcslen(szFileName) * sizeof(TCHAR));
}
void CIniWriter::WriteInteger(LPCTSTR szSection, LPCTSTR szKey, int iValue)
{
    TCHAR szValue[255];
    _stprintf_s(szValue, 255, TEXT("%d"), iValue);
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}
void CIniWriter::WriteFloat(LPCTSTR szSection, LPCTSTR szKey, float fltValue)
{
    TCHAR szValue[255];
    _stprintf_s(szValue, 255, TEXT("%f"), fltValue);
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}
void CIniWriter::WriteBoolean(LPCTSTR szSection, LPCTSTR szKey, bool bolValue)
{
    TCHAR szValue[255];
    _stprintf_s(szValue, 255, TEXT("%s"), bolValue ? TEXT("True") : TEXT("False"));
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}
void CIniWriter::WriteString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue)
{
    WritePrivateProfileString(szSection, szKey, szValue, m_szFileName);
}

// 读写测试
/*
#if defined(UNICODE) || defined(_UNICODE)
#define tcout std::wcout
#else
#define tcout std::cout
#endif
#include <iostream>
#include <windows.h>
#include "IniWriter.h"
#include "IniReader.h"

int _tmain(int argc, _TCHAR* argv[])
{
    CIniWriter iniWriter(TEXT(".\\initest.ini"));
    iniWriter.WriteString(TEXT("Setting"), TEXT("Name"), TEXT("jianxx"));
    iniWriter.WriteInteger(TEXT("Setting"), TEXT("Age"), 27);
    iniWriter.WriteFloat(TEXT("Setting"), TEXT("Height"), 1.82f);
    iniWriter.WriteBoolean(TEXT("Setting"), TEXT("Marriage"), false);
    CIniReader iniReader(TEXT(".\\initest.ini"));
    LPTSTR szName = iniReader.ReadString(TEXT("Setting"), TEXT("Name"), TEXT(""));
    int iAge = iniReader.ReadInteger(TEXT("Setting"), TEXT("Age"), 25);
    float fltHieght = iniReader.ReadFloat(TEXT("Setting"), TEXT("Height"), 1.80f);
    bool bMarriage = iniReader.ReadBoolean(TEXT("Setting"), TEXT("Marriage"), true);
    tcout << "Name:" << szName << std::endl
        << "Age:" << iAge << std::endl
        << "Height:" << fltHieght << std::endl
        << "Marriage:" << bMarriage << std::endl;
    delete szName;
    return 1;
}*/


/*==================================================================
= 文件名:CIniFile类定义文件
= 主要功能:可以读取.修改变量数值,可以设置新的组,新的变量
====================================================================*/

CIniFile::CIniFile() :bFileExsit(FALSE)
{
}
CIniFile::~CIniFile()
{
    if (bFileExsit)
    {
        if (stfFile.Open(strInIFileName, CFile::modeCreate | CFile::modeWrite) && FileContainer.GetSize() > 0)
        {
            CString strParam;
            for (int i = 0; i < FileContainer.GetSize(); i++)
            {
                strParam = FileContainer[i];
                // stfFile.WriteString(strParam);
                stfFile.WriteString(strParam + _T("\n"));
            }
        }
        stfFile.Close();
    }
    if (FileContainer.GetSize() > 0)
    {
        FileContainer.RemoveAll();
    }
}
BOOL CIniFile::Create(const CString& strFileName)
{
    bFileExsit = FALSE;
    strInIFileName = strFileName;
    if (!stfFile.Open(strFileName, CFile::modeRead))
    {
        return bFileExsit;
    }
    CString strFileLine;
    while (bFileExsit = stfFile.ReadString(strFileLine))
    {
        if (bFileExsit == FALSE)
            return bFileExsit;
        FileContainer.Add(strFileLine);
    }
    stfFile.Close();
    bFileExsit = TRUE;
    return bFileExsit;
}
BOOL CIniFile::GetVar(const CString& strSection, const CString& strVarName, CString& strReturnValue)
{
    if (bFileExsit == FALSE || FileContainer.GetSize() < 0)
        return bFileExsit;
    int iLine = SearchLine(strSection, strVarName);
    if (iLine > 0)
    {
        CString strParam = FileContainer[iLine - 1];
        strReturnValue = strParam.Mid(strParam.Find(_T("=")) + 1);
        return TRUE;
    }
    return FALSE;
}

BOOL CIniFile::GetVarStr(const CString& strSection, const CString& strVarName, CString& strReturnValue)
{
    return(GetVar(strSection, strVarName, strReturnValue));
}

BOOL CIniFile::GetVarInt(const CString& strSection, const CString& strVarName, int& iValue)
{
    CString strReturnVar;
    if (GetVar(strSection, strVarName, strReturnVar))
    {
        strReturnVar.TrimLeft();
        int iLen = strReturnVar.GetLength();
        iValue = _tstoi(strReturnVar.GetBuffer(iLen));
        return TRUE;
    }
    return TRUE;
}

BOOL CIniFile::SetVar(const CString& strSection, const CString& strVarName, const CString& strVar, const int iType)
{
    if (bFileExsit == FALSE)
        return bFileExsit;
    if (FileContainer.GetSize() == 0)
    {
        FileContainer.Add(_T("[") + strSection + _T("]"));
        FileContainer.Add(strVarName + _T("=") + strVar);
        return TRUE;
    }
    int i = 0;
    int iFileLines = (int)FileContainer.GetSize();
    //for(pInterator;pInterator != FileContainer.end();++pInterator)
    //{
    while (i < iFileLines)
    {
        CString strValue = FileContainer.GetAt(i++);
        strValue.TrimLeft();
        if ((strValue.Find(_T("[")) >= 0) && (strValue.Find(strSection) >= 0))
        {
            while (i < iFileLines)
            {
                CString strSectionList = FileContainer[i++];
                strSectionList.TrimLeft();
                if (strSectionList.Find(_T("//")) >= 0)//找到注释行
                    continue;
                if (strSectionList.Find(strVarName) >= 0)//找到
                {
                    CString strParam = strVarName + "=" + strVar;
                    //FileContainer.SetAt(i-1,strParam);
                    FileContainer[i - 1] = strParam;
                    return TRUE;
                }
                if (strSectionList.Find(_T("["), 0) >= 0)//在原来文件的SECTION中,没有相应的变量需要添加而且,这种情况下,下边还有别的section
                {
                    //处理流程是这样的,首先把当前的数值依次向后移动,然后在当前位置加入新出现的数值
                    if (iType != 0)
                        return FALSE;
                    CString strParam;
                    FileContainer.Add(strParam);
                    int iPre = (int)(FileContainer.GetSize() - 1);
                    while (iPre >= i)
                    {
                        CString strBehind = FileContainer[iPre - 1];
                        FileContainer[iPre] = strBehind;
                        iPre--;
                    }
                    strParam = strVarName + _T("=") + strVar;
                    FileContainer.SetAt(i - 1, strParam);
                    return TRUE;
                }
                if (i == iFileLines && iType == 0)
                {
                    FileContainer.Add(strVarName + _T("=") + strVar);
                    return TRUE;
                }
            }
        }
    }
    if (iType == 0)
    {
        FileContainer.Add(_T("[") + strSection + _T("]"));
        FileContainer.Add(strVarName + _T("=") + strVar);
    }
    return TRUE;
}
BOOL CIniFile::SetVarStr(const CString& strSection, const CString& strVarName, const CString& strValue, const int iType)
{
    return SetVar(strSection, strVarName, strValue, iType);
}
BOOL CIniFile::SetVarInt(const CString& strSection, const CString& strVarName, const int& iValue, const int iType)
{
    CString strVar;
    strVar.Format(_T("%d"), iValue);
    return (SetVar(strSection, strVarName, strVar, iType));
}
int CIniFile::SearchLine(const CString& strSection, const CString& strVarName)
{
    if (FileContainer.GetSize() > 0)
    {
        int i = 0;
        int iFileLines = (int)FileContainer.GetSize();
        while (i < iFileLines)
        {
            CString strValue = FileContainer[i++];
            strValue.TrimLeft();
            if (strValue.Find(_T("[")) >= 0 && strValue.Find(strSection, 1) >= 0)
            {
                while (i < iFileLines)
                {
                    CString strSectionList = FileContainer[i++];
                    strSectionList.TrimLeft();
                    if (strSectionList.Find(_T("//")) >= 0)//找到注释行
                        continue;
                    if (strSectionList.Find(strVarName) >= 0)//找到
                    {
                        return i;
                    }
                    if (strSectionList.Find(_T("["), 0) >= 0)//另外一个段落出现,寻找失败
                    {
                        return -2;
                    }
                }
            }
        }
    }
    return -1;
}