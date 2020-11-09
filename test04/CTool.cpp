#include "pch.h"
#include "CTool.h"


void CTool::OpenConsole() 
{
	FILE* stream1;
	AllocConsole();
	
	freopen_s( &stream1, "CON", "r", stdin);
	freopen_s( &stream1, "CON", "w", stdout);
	freopen_s( &stream1, "CON", "w", stderr);
	cout << "��������̨..."<<endl;
}
void CTool::CloseConsole()
{
	cout << "�رտ���̨" << endl;

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);

	FreeConsole();

}

//INI�ļ�������,ʵ��

CMyINI::CMyINI()
{
}
CMyINI::~CMyINI()
{
}
//************************************************************************
// ��������:    TrimString
// ����Ȩ��:    public 
// ��������:        2017/01/05
// �� �� ��:       
// ����˵��:        ȥ���ո�
// ��������:    string & str    ������ַ���
// �� �� ֵ:   std::string &   ����ַ���
//************************************************************************
string& TrimString(string& str)
{
	string::size_type pos = 0;
	while (str.npos != (pos = str.find(" ")))
		str = str.replace(pos, pos + 1, "");
	return str;
}

//************************************************************************
// ��������:    ReadINI
// ����Ȩ��:    public 
// ��������:        2017/01/05
// �� �� ��:       
// ����˵��:        ��ȡINI�ļ��������䱣�浽map�ṹ��
// ��������:    string path INI�ļ���·��
// �� �� ֵ:   int
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
    }   //��ȡ�����ڵ�
    for (map<string, string>::iterator itr = map_tmp.begin(); itr != map_tmp.end(); ++itr)
    {
#ifdef INIDEBUG
        cout << "���ڵ㣺 " << itr->first << endl;
#endif  //INIDEBUG
        SubNode sn;
        for (vector<ININode>::iterator sub_itr = vec_ini.begin(); sub_itr != vec_ini.end(); ++sub_itr)
        {
            if (sub_itr->root == itr->first)
            {
#ifdef INIDEBUG
                cout << "��ֵ�ԣ� " << sub_itr->key << "=" << sub_itr->value << endl;
#endif  //INIDEBUG
                sn.InsertElement(sub_itr->key, sub_itr->value);
            }
        }
        map_ini.insert(pair<string, SubNode>(itr->first, sn));
    }
    return 1;
}

//************************************************************************
// ��������:    GetValue
// ����Ȩ��:    public 
// ��������:        2017/01/05
// �� �� ��:       
// ����˵��:        ���ݸ����ĸ����ͼ�ֵ�����������ֵ
// ��������:    string root     ������ĸ����
// ��������:    string key      ������ļ�
// �� �� ֵ:   std::string     �������ֵ
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
// ��������:    WriteINI
// ����Ȩ��:    public 
// ��������:        2017/01/05
// �� �� ��:       
// ����˵��:        ����XML����Ϣ���ļ���
// ��������:    string path INI�ļ��ı���·��
// �� �� ֵ:   int
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
// ��������:    SetValue
// ����Ȩ��:    public 
// ��������:        2017/01/05
// �� �� ��:       
// ����˵��:        �����������ֵ
// ��������:    string root     ������ĸ��ڵ�
// ��������:    string key      ������ļ�
// ��������:    string value    �������ֵ
// �� �� ֵ:   std::vector<ININode>::size_type   
//************************************************************************
vector<ININode>::size_type CMyINI::SetValue(string root, string key, string value)
{
    map<string, SubNode>::iterator itr = map_ini.find(root);  //����
    if (map_ini.end() != itr)
    {
        //itr->second.sub_node.insert(pair<string, string>(key, value));
        itr->second.sub_node[key] = value;
    }   //���ڵ��Ѿ������ˣ�����ֵ
    else
    {
        SubNode sn;
        sn.InsertElement(key, value);
        map_ini.insert(pair<string, SubNode>(root, sn));
    }   //���ڵ㲻���ڣ����ֵ

    return map_ini.size();
}

//************************************************************************
// ��������:    Travel
// ����Ȩ��:    public 
// ��������:        2017/01/05
// �� �� ��:       
// ����˵��:        ������ӡINI�ļ�
// �� �� ֵ:   void
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

//��д����
/*
CMyINI* p = new CMyINI();
p->ReadINI("Setting.ini");
cout << "\nԭʼINI�ļ����ݣ�" << std::endl;
p->Travel();
p->SetValue("setting", "hehe", "eheh");
cout << "\n���ӽڵ�֮������ݣ�" << std::endl;
p->Travel();
cout << "\n�޸Ľڵ�֮������ݣ�" << std::endl;
p->SetValue("kk", "kk", "2");
p->Travel();
p->WriteINI("Setting.ini");
*/



/* ��� C++ ��װ��ini����������ת������,  */


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

// ��д����
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
= �ļ���:CIniFile�ඨ���ļ�
= ��Ҫ����:���Զ�ȡ.�޸ı�����ֵ,���������µ���,�µı���
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
                if (strSectionList.Find(_T("//")) >= 0)//�ҵ�ע����
                    continue;
                if (strSectionList.Find(strVarName) >= 0)//�ҵ�
                {
                    CString strParam = strVarName + "=" + strVar;
                    //FileContainer.SetAt(i-1,strParam);
                    FileContainer[i - 1] = strParam;
                    return TRUE;
                }
                if (strSectionList.Find(_T("["), 0) >= 0)//��ԭ���ļ���SECTION��,û����Ӧ�ı�����Ҫ��Ӷ���,���������,�±߻��б��section
                {
                    //����������������,���Ȱѵ�ǰ����ֵ��������ƶ�,Ȼ���ڵ�ǰλ�ü����³��ֵ���ֵ
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
                    if (strSectionList.Find(_T("//")) >= 0)//�ҵ�ע����
                        continue;
                    if (strSectionList.Find(strVarName) >= 0)//�ҵ�
                    {
                        return i;
                    }
                    if (strSectionList.Find(_T("["), 0) >= 0)//����һ���������,Ѱ��ʧ��
                    {
                        return -2;
                    }
                }
            }
        }
    }
    return -1;
}