// feepacker.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "feepacker.h"
#include "filetool.h"
#include "AwkBase.h"
#define BUFFER_SIZE 1024
#define outerrmsg 0 && ::fprintf
#include "FileOperator.h"


const char *LINE_ENDL = "\n";



void usage(char *proname)
{
    cout << "usage:" << proname << " configpath" << endl;
}

string g_srvcode, g_srvname, g_inparam, g_outparam, g_outputdir, g_errnoName, g_errinfoName;
string g_fileprefix;

vector<string> g_vecParamName;
map<string, int> g_mpParamSize;
vector<CXmlFormat> g_vecOutparamFormat;
SrvInfo g_SrvInfo;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        usage(argv[0]);
        return 0;
    }

    CConfigOpr cfgOpr;

    string cfgfile = argv[1];
    if ( -1 == cfgOpr.OpenCfgFile(cfgfile) )
    {
        cerr << "ERR: open file " << cfgfile << " fail!" << endl;
        return -1;
    }

    cout << "读取配置文件参数" << endl;
    cfgOpr.GetValue("info", "srvcode", g_srvcode);
    cfgOpr.GetValue("info", "srvname", g_srvname);
    cfgOpr.GetValue("info", "inparam", g_inparam);
    cfgOpr.GetValue("info", "outparam", g_outparam);
    cfgOpr.GetValue("info", "outputdir", g_outputdir);
    cfgOpr.GetValue("info", "errno", g_errnoName);
    cfgOpr.GetValue("info", "errinfo", g_errinfoName);
    cfgOpr.GetValue("info", "fileprefix", g_fileprefix);

    cout << "读取模板文件" << endl;
    if (!readSrvInfo())
    {
        cerr << "ERR: 初始化信息失败，请检查模板文件是否齐全" << endl;
        return -1;
    }

    cout << "替换服务名，组件名等公共信息" << endl;
    setSrvInfo();

    cout << "生成入参定义" << endl;
    setInparam();

    cout << "生成出参定义" << endl;
    setOutparam();

    cout << "填充错误报文函数" << endl;
    setSetErrorXml();

    cout << "填充组件函数" << endl;
    setComFunc();

    cout << "填充服务函数" << endl;
    setSrvFunc();


    cout << "保存结果" << endl;
    saveSrvInfoToFile();

    cout << "代码文件生成完毕！" << endl;
    return 0;
}

class COneParamDual:public CAwkOneDualItrBase
{
    int dualOneLine(CAwkDoc &Doc)
    {
        char buffer[1024];

        vector<string>& vecParts = Doc.m_vecParts;
        string& outmsg = Doc.getStrParam("outmsg");

        //注释起始位置
        int beginpos = 4;
        int memo = 0;

        if (tolower(vecParts[2]) == "char") //考虑到char *跟char对awk分割的影响
        {
            sprintf(buffer, "\tchar %s[%d]; //%s", 
                vecParts[0].c_str(), atoi(vecParts[4].c_str()) + 1, vecParts[1].c_str());
            outmsg += buffer;

            beginpos = 5;
        }
        else if (tolower(vecParts[2]) == "char*")
        {
            sprintf(buffer, "\tchar %s[%d]; //%s", 
                vecParts[0].c_str(), atoi(vecParts[3].c_str()) + 1, vecParts[1].c_str());
            outmsg += buffer;
        }
        else if (vecParts[2] == "CPArray" || vecParts[2] == "对象数组" 
            || vecParts[2] == "数组" || vecParts[2] == "对象数组（CPArray）")
        {
            sprintf(buffer, "\tvector<%s> %sArray; //%s", 
                vecParts[0].c_str(), vecParts[0].c_str(), vecParts[1].c_str());
            outmsg += buffer;

            //未知对象数组需要登记
            if (find(g_vecParamName.begin(), g_vecParamName.end(), vecParts[0]) 
                == g_vecParamName.end())
            {
                g_vecParamName.push_back(vecParts[0]);
            }
        }
        else
        {
            cerr << "ERR: 未知变量类型[" << vecParts[2] << "]" << endl;
            sprintf(buffer, "\t%s %s; //%s", 
                vecParts[2].c_str(), atoi(vecParts[0].c_str()), vecParts[1].c_str());
            outmsg += buffer;
        }

        //将备注当成注释补上
        while (beginpos < (int)vecParts.size())
        {
            if (vecParts[beginpos] != "")
            {
                if (memo == 0)
                {
                    outmsg += " [ ";
                    memo = 1;
                }
                else
                {
                    outmsg += " ";
                }

                sprintf(buffer, "%s", vecParts[beginpos].c_str());
                outmsg += buffer;
            }

            beginpos++;
        }

        if (memo == 1)
        {
            outmsg += " ]";
        }

        outmsg += "\n";

        //元素个数 此处登记目的是为了后面替换参数检查时需要的参数个数
        int paramSize = atoi(Doc.getStrParam("paramSize").c_str()) + 1;
        Doc.setStrParam("paramSize", toString(paramSize));

        return 0;
    }

    int dualBegin(CAwkDoc& Doc)
    {
        Doc.getStrParam("outmsg") = "typedef struct tag_" + Doc.getStrParam("paramname") + "\n{\n";
        return 0;
    }

    int dualEnd(CAwkDoc& Doc)
    {
        //记录数组结构体元素个数
        g_mpParamSize[Doc.getStrParam("paramname")] = atoi(Doc.getStrParam("paramSize").c_str());
        Doc.getStrParam("outmsg") += "} " + Doc.getStrParam("paramname") + ";\n\n";
        return 0;
    }
};

bool setOneParam(const string paramname, string& outmsg)
{
    string filepath = paramname + ".txt";

    COneParamDual dual;
    CAwkDoc doc(&dual, filepath);
    doc.setStrParam("paramname", paramname);
    doc.run();
    outmsg = doc.getStrParam("outmsg");

    return true;
}

bool setInparam()
{
    string allParamdefine;
    g_vecParamName.push_back(g_inparam);

    for (size_t i = g_vecParamName.size() - 1; i < g_vecParamName.size(); i++)
    {
        string outmsg;
        setOneParam(g_vecParamName[i], outmsg);
        //cout << "outmsg = " << outmsg << endl;
        allParamdefine = outmsg + allParamdefine;
    }

    stringReplaceAll(g_SrvInfo.IB_COM_Common_h, "<InParamDefine>", allParamdefine);
    return true;
}

bool setOutparam()
{
    string allParamdefine;
    g_vecParamName.push_back(g_outparam);

    for (size_t i = g_vecParamName.size() - 1; i < g_vecParamName.size(); i++)
    {
        string outmsg;
        setOneParam(g_vecParamName[i], outmsg);
        //cout << "outmsg = " << outmsg << endl;
        allParamdefine = outmsg + allParamdefine;
    }

    stringReplaceAll(g_SrvInfo.IB_COM_Common_h, "<OutParamDefine>", allParamdefine);
    return true;
}

bool readSrvInfo()
{
    char buffer[BUFFER_SIZE];
    getcwd(buffer, sizeof(buffer));

    g_SrvInfo.IB_SRV_Common_cpp = FileToString("IB_SRV_Common.cpp");
    g_SrvInfo.IB_SRV_Common_h = FileToString("IB_SRV_Common.h");
    g_SrvInfo.IB_COM_Common_cpp = FileToString("IB_COM_Common.cpp");
    g_SrvInfo.IB_COM_Common_h = FileToString("IB_COM_Common.h");
    g_SrvInfo.ParseArrayToNode_cpp = FileToString("ParseArrayToNode.cpp");
    g_SrvInfo.getHDataNodeResult_cpp = FileToString("getHDataNodeResult.cpp");
    g_SrvInfo.ParseArrayToNode_h = FileToString("ParseArrayToNode.h");
    g_SrvInfo.getHDataNodeResult_h = FileToString("getHDataNodeResult.h");

    if (g_SrvInfo.IB_COM_Common_cpp == "" || g_SrvInfo.IB_COM_Common_h == "" ||
        g_SrvInfo.IB_COM_Common_cpp == "" || g_SrvInfo.IB_COM_Common_h == "" ||
        g_SrvInfo.ParseArrayToNode_cpp == "" || g_SrvInfo.getHDataNodeResult_cpp == "" ||
        g_SrvInfo.ParseArrayToNode_h == "" || g_SrvInfo.getHDataNodeResult_h == "")
    {
        cerr << "ERR: 存在空模板文件" << endl;
        return false;
    }

    return true;
}

void replaceSrvInfo(const string& toReplace, const string& replaceWith)
{
    stringReplaceAll(g_SrvInfo.getHDataNodeResult_cpp, toReplace, replaceWith);
    stringReplaceAll(g_SrvInfo.getHDataNodeResult_h, toReplace, replaceWith);
    stringReplaceAll(g_SrvInfo.IB_COM_Common_cpp, toReplace, replaceWith);
    stringReplaceAll(g_SrvInfo.IB_COM_Common_h, toReplace, replaceWith);
    stringReplaceAll(g_SrvInfo.IB_SRV_Common_cpp, toReplace, replaceWith);
    stringReplaceAll(g_SrvInfo.IB_SRV_Common_h, toReplace, replaceWith);
    stringReplaceAll(g_SrvInfo.ParseArrayToNode_cpp, toReplace, replaceWith);
    stringReplaceAll(g_SrvInfo.ParseArrayToNode_h, toReplace, replaceWith);
}

bool setSrvInfo()
{
    replaceSrvInfo("<Common>", g_srvcode);
    replaceSrvInfo("<组件名称>", g_srvname);
    replaceSrvInfo("<COMMON>", toupper(g_srvcode));
    replaceSrvInfo("<inparam>", g_inparam);
    replaceSrvInfo("<outparam>", g_outparam);

    return true;
}

bool saveOneFile(const string& filename, const string& filecontent)
{
    CFileOperator opr;
    string filepath = filename;
    stringReplaceAll(filepath, "Common", g_srvcode);
    stringReplace(filepath, "IB_", g_fileprefix);

    filepath = g_outputdir + filepath;
    if (NULL == opr.fopen(filepath.c_str(), "w"))
    {
        cerr << "ERR: cat not open file " << filepath << endl;
        return false;
    }
    opr.fwrite(filecontent.c_str(), 1, filecontent.size());
    opr.fclose();

    bfsvn(filepath);

    return true;
}

bool saveSrvInfoToFile()
{
    if ( !saveOneFile("IB_SRV_Common.cpp", g_SrvInfo.IB_SRV_Common_cpp) )
    {
        return false;
    }

    if ( !saveOneFile("IB_SRV_Common.h", g_SrvInfo.IB_SRV_Common_h) )
    {
        return false;
    }

    if ( !saveOneFile("IB_COM_Common.cpp", g_SrvInfo.IB_COM_Common_cpp) )
    {
        return false;
    }

    if ( !saveOneFile("IB_COM_Common.h", g_SrvInfo.IB_COM_Common_h) )
    {
        return false;
    }

    return true;
}

bool setSrvFunc()
{
    setParseSrvMsg();
    setParseComMsg();
    setParseArrayToNode();

    return true;
}

bool setComFunc()
{
    setSetCallInfo();
    setGetCallResult();
    setGetHDataNodeResult();

    return true;
}



class CSetCallInfoDual:public CAwkOneDualItrBase
{
    int dualOneLine(CAwkDoc &Doc)
    {
        char buffer[1024];

        vector<string>& vecParts = Doc.m_vecParts;
        string& outmsg = Doc.getStrParam("outmsg");

        //sprintf(buffer, "\t//%s\n", vecParts[1].c_str());
        sprintf(buffer, "\tBASE_LOG_DEBUG(\"%%s\", \"%s\");\n", vecParts[1].c_str());
        outmsg += buffer;

        if (vecParts[2] == "CPArray" || vecParts[2] == "对象数组" 
            || vecParts[2] == "数组" || vecParts[2] == "对象数组（CPArray）")
        {

            sprintf(buffer, "\tif ( !ParseArrayToNode(node->AddArrayChild(), data.%sArray) )\n", 
                vecParts[0].c_str());
            outmsg += buffer;
            outmsg += "\t{\n";
            outmsg += "\t\treturn false;\n";
            outmsg += "\t}\n\n";
        }
        else
        {
            sprintf(buffer, "\tstrValue = data.%s;\n", vecParts[0].c_str());
            outmsg += buffer;

            sprintf(buffer, "\tnode->AddSimpleData(strValue);\n\n");
            outmsg += buffer;
        }

        return 0;
    }

    int dualEnd(CAwkDoc& Doc)
    {
        stringReplaceAll(g_SrvInfo.IB_COM_Common_cpp, "<setCallInfo>", Doc.getStrParam("outmsg"));

        return 0;
    }
};

bool setSetCallInfo()
{

    string filepath = g_inparam + ".txt";

    CSetCallInfoDual lineopr;
    CAwkDoc doc(&lineopr, filepath);
    doc.run();

    return true;
}


class CGetCallResultDual:public CAwkOneDualItrBase
{
    int dualOneLine(CAwkDoc &Doc)
    {
        char buffer[1024];

        vector<string>& vecParts = Doc.m_vecParts;
        string& outmsg = Doc.getStrParam("outmsg");

        if (vecParts[0] == g_errinfoName)
        {
            outmsg += "\t//返回错误信息\n";
            outmsg += "\tHDataNode *currNode = node->DataList[pos++];\n";
            outmsg += "\tm_sErrorStr = *(currNode->GetKey());\n";
            outmsg += "\tstrncpy_ex(res." + g_errinfoName + ", m_sErrorStr.c_str());\n";
        }
        else 
        {
            sprintf(buffer, "\tBASE_LOG_DEBUG(\"%%s\", \"%s\");\n", vecParts[1].c_str());
            outmsg += buffer;

            if (vecParts[2] == "CPArray" || vecParts[2] == "对象数组" 
                || vecParts[2] == "数组" || vecParts[2] == "对象数组（CPArray）")
            {
                sprintf(buffer, "\tif( !getHDataNodeResult(res.%sArray, node->DataList[pos++], \n"
                    "\t\t\t\"组件结构体[%s]\" ) )\n",
                    vecParts[0].c_str(), vecParts[1].c_str());
                outmsg += buffer;
            }
            else
            {
                sprintf(buffer, "\tif( !StrToCstr(res.%s, *(node->DataList[pos++]->GetKey()), \n"
                    "\t\t\t\"组件结构体[%s]\" ) )\n",
                    vecParts[0].c_str(), vecParts[1].c_str());
                outmsg += buffer;
            }

            outmsg += "\t{\n";
            outmsg += "\t\treturn false;\n";
            outmsg += "\t}\n\n";
        }

        return 0;
    }

    int dualEnd(CAwkDoc& Doc)
    {
        stringReplaceAll(g_SrvInfo.IB_COM_Common_cpp, "<getCallResult>", Doc.getStrParam("outmsg"));
        stringReplaceAll(g_SrvInfo.IB_COM_Common_cpp, "<getCallResultCnt>", toString(g_mpParamSize[g_outparam]));

        return 0;
    }
};

bool setGetCallResult()
{
    string filepath = g_outparam + ".txt";

    CGetCallResultDual lineopr;
    CAwkDoc doc(&lineopr, filepath);
    doc.run();

    return true;
}


class CGetHDataNodeResultDual:public CAwkOneDualItrBase
{
    int dualOneLine(CAwkDoc &Doc)
    {
        char buffer[1024];

        vector<string>& vecParts = Doc.m_vecParts;
        string& outmsg = Doc.getStrParam("outmsg");

        if (vecParts[2] == "CPArray" || vecParts[2] == "对象数组" 
            || vecParts[2] == "数组" || vecParts[2] == "对象数组（CPArray）")
        {
            sprintf(buffer, "\t\tif( !getHDataNodeResult(item.%sArray, node->DataList[pos++], \n"
                "\t\t\tmemname + \"[%s]\") )\n", 
                vecParts[0].c_str(), vecParts[1].c_str());
            outmsg += buffer;
        }
        else
        {
            sprintf(buffer, "\t\tif( !StrToCstr(item.%s, *(node->DataList[pos++]->GetKey()), \n"
                "\t\t\tmemname + \"[%s]\") )\n",
                vecParts[0].c_str(), vecParts[1].c_str());
            outmsg += buffer;
        }

        outmsg += "\t\t{\n";
        outmsg += "\t\t\treturn false;\n";
        outmsg += "\t\t}\n\n";

        return 0;
    }
};


bool setOneGetHDataNodeResult(string& resultImplement, const string& paramname)
{

    string filepath = paramname + ".txt";

    CGetHDataNodeResultDual lineopr;
    CAwkDoc doc(&lineopr, filepath);
    doc.run();
    stringReplaceAll(resultImplement, "<getHDataNodeResult>", doc.getStrParam("outmsg"));

    return true;
}

bool setGetHDataNodeResult()
{
    string alldefine;
    string allimplement;

    string resultDefine;
    string resultImplement;

    for (size_t i = 0; i < g_vecParamName.size(); i++)
    {
        if (g_vecParamName[i] == g_inparam || g_vecParamName[i] == g_outparam)
        {
            continue;
        }

        resultDefine = g_SrvInfo.getHDataNodeResult_h;
        resultImplement = g_SrvInfo.getHDataNodeResult_cpp;
        stringReplaceAll(resultDefine, "<StuName>", g_vecParamName[i]);
        stringReplaceAll(resultImplement, "<StuName>", g_vecParamName[i]);
        stringReplaceAll(resultImplement, "<getHDataNodeResultCnt>", toString(g_mpParamSize[g_vecParamName[i]]));

        setOneGetHDataNodeResult(resultImplement, g_vecParamName[i]);

        alldefine += resultDefine;
        allimplement += resultImplement;
    }

    stringReplaceAll(g_SrvInfo.IB_COM_Common_h, "<getHDataNodeResultDefine>", alldefine);
    stringReplaceAll(g_SrvInfo.IB_COM_Common_cpp, "<getHDataNodeResultDefineImplement>", allimplement);
    return true;
}

class CParseSrvMsgDual:public CAwkOneDualItrBase
{
    int dualOneLine(CAwkDoc &Doc)
    {
        char buffer[1024];

        vector<string>& vecParts = Doc.m_vecParts;
        string& outmsg = Doc.getStrParam("outmsg");

        if (vecParts[2] == "CPArray" || vecParts[2] == "对象数组" 
            || vecParts[2] == "数组" || vecParts[2] == "对象数组（CPArray）")
        {
            sprintf(buffer, "\tif( !m_cicsOpr.getHDataNodeResult(data.%sArray, rootnode->DataList[pos++], \n"
                "\t\t\t\"[%s]\" ) )\n",
                vecParts[0].c_str(), vecParts[1].c_str());
            outmsg += buffer;
            outmsg += "\t{\n";
            outmsg += "\t\tm_iErrorCode = m_cicsOpr.m_iErrorCode;\n";
            outmsg += "\t\tm_sErrorStr = m_cicsOpr.m_sErrorStr;\n";
            outmsg += "\t\treturn false;\n";
            outmsg += "\t}\n\n";
        }
        else
        {
            outmsg += "\tcurnode = rootnode->DataList[pos++];\n";
            outmsg += "\tpStr = curnode->GetKey();\n";

            sprintf(buffer, "\tif (!copyToBuffer(data.%s, pStr, \"%s\"))\n", 
                vecParts[0].c_str(), vecParts[1].c_str());
            outmsg += buffer;
            outmsg += "\t{\n";
            outmsg += "\t\treturn false;\n";
            outmsg += "\t}\n\n";
        }

        return 0;
    }

    int dualEnd(CAwkDoc& Doc)
    {
        stringReplaceAll(g_SrvInfo.IB_SRV_Common_cpp, "<ParseSrvMsg>", Doc.getStrParam("outmsg"));
        stringReplaceAll(g_SrvInfo.IB_SRV_Common_cpp, "<ParseSrvMsgCnt>", toString(g_mpParamSize[g_inparam]));

        return 0;
    }
};

bool setParseSrvMsg()
{
    string filepath = g_inparam + ".txt";
    CParseSrvMsgDual lineopr;
    CAwkDoc doc(&lineopr, filepath);
    doc.run();

    return true;
}


class CParseComMsgDual:public CAwkOneDualItrBase
{
    int dualOneLine(CAwkDoc &Doc)
    {
        char buffer[1024];

        vector<string>& vecParts = Doc.m_vecParts;
        string& outmsg = Doc.getStrParam("outmsg");

        if (vecParts[0] == g_errnoName)
        {
            outmsg += "\t//封装服务seq节点\n";
            outmsg += "\tnode->AddUseqChild();\n\n";
        }

        sprintf(buffer, "\tBASE_LOG_DEBUG(\"%%s\", \"%s\");\n", vecParts[1].c_str());
        outmsg += buffer;

        if (vecParts[2] == "CPArray" || vecParts[2] == "对象数组" 
            || vecParts[2] == "数组" || vecParts[2] == "对象数组（CPArray）")
        {

            sprintf(buffer, "\tif ( !m_cicsOpr.ParseArrayToNode(node->AddArrayChild(), data.%sArray) )\n", 
                vecParts[0].c_str());
            outmsg += buffer;
            outmsg += "\t{\n";
            outmsg += "\t\tm_iErrorCode = m_cicsOpr.m_iErrorCode;\n";
            outmsg += "\t\tm_sErrorStr = m_cicsOpr.m_sErrorStr;\n";
            outmsg += "\t\treturn false;\n";
            outmsg += "\t}\n\n";
        }
        else
        {
            sprintf(buffer, "\tstrValue = data.%s;\n",
                vecParts[0].c_str());
            outmsg += buffer;
            outmsg += "\tnode->AddSimpleData(strValue);\n\n";
        }

        return 0;
    }
};

bool setParseComMsg()
{
    string filepath = g_outparam + ".txt";

    CParseComMsgDual lineopr;
    CAwkDoc doc(&lineopr, filepath);

    doc.run();
    stringReplaceAll(g_SrvInfo.IB_SRV_Common_cpp, "<ParseComMsg>", doc.m_strParams["outmsg"]);

    return true;
}

class CParseArrayToNodeDual:public CAwkOneDualItrBase
{
    int dualOneLine(CAwkDoc &Doc)
    {
        vector<string>& vecParts = Doc.m_vecParts;
        string& outmsg = Doc.getStrParam("outmsg");

        char buffer[1024];
        sprintf(buffer, "\t\t//%s\n", vecParts[1].c_str());
        outmsg += buffer;

        if (vecParts[2] == "CPArray" || vecParts[2] == "对象数组" 
            || vecParts[2] == "数组" || vecParts[2] == "对象数组（CPArray）")
        {

            sprintf(buffer, "\t\tif ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].%sArray) )\n", 
                vecParts[0].c_str());
            outmsg += buffer;
            outmsg += "\t\t{\n";
            outmsg += "\t\t\treturn false;\n";
            outmsg += "\t\t}\n";
        }
        else
        {
            sprintf(buffer, "\t\tstrValue = nodeinfo[i].%s;\n",
                vecParts[0].c_str());
            outmsg += buffer;
            outmsg += "\t\tnode->AddSimpleData(strValue);\n\n";
        }

        return 0;
    }
};

bool setOneParseArrayToNode(string& resultImplement, const string& paramname)
{
    string filepath = paramname + ".txt";

    CParseArrayToNodeDual lineopr;
    CAwkDoc doc(&lineopr, filepath);

    doc.run();
    stringReplaceAll(resultImplement, "<ParseArrayToNode>", doc.m_strParams["outmsg"]);

    return true;
}

bool setParseArrayToNode()
{
    string alldefine;
    string allimplement;

    string resultDefine;
    string resultImplement;

    for (size_t i = 0; i < g_vecParamName.size(); i++)
    {
        if (g_vecParamName[i] == g_inparam || g_vecParamName[i] == g_outparam)
        {
            continue;
        }

        resultDefine = g_SrvInfo.ParseArrayToNode_h;
        resultImplement = g_SrvInfo.ParseArrayToNode_cpp;
        stringReplaceAll(resultDefine, "<StuName>", g_vecParamName[i]);
        stringReplaceAll(resultImplement, "<StuName>", g_vecParamName[i]);

        setOneParseArrayToNode(resultImplement, g_vecParamName[i]);

        alldefine += resultDefine;
        allimplement += resultImplement;
    }

    stringReplaceAll(g_SrvInfo.IB_COM_Common_h, "<ParseArrayToNodeDefine>", alldefine);
    stringReplaceAll(g_SrvInfo.IB_COM_Common_cpp, "<ParseArrayToNodeDefineImplement>", allimplement);
    return true;
}

class CParamTypeDual:public CAwkOneDualItrBase
{

    virtual int dualOneLine(CAwkDoc &Doc)
    {
        if (Doc.m_line == "")
        {
            return 0;
        }

        vector<string>& vecParts = Doc.m_vecParts;
        if (vecParts.size() < 3)
        {
            vecParts.resize(3);
        }

        if (vecParts[0] == g_errnoName)
        {
            CXmlFormat type(CXmlFormat::ERRNO);
            g_vecOutparamFormat.push_back(type);
        }
        else if (vecParts[0] == g_errinfoName)
        {
            CXmlFormat type(CXmlFormat::ERRINFO);
            g_vecOutparamFormat.push_back(type);
        }
        else if (vecParts[2] == "CPArray" || vecParts[2] == "对象数组" 
            || vecParts[2] == "数组" || vecParts[2] == "对象数组（CPArray）")
        {
            CXmlFormat type(CXmlFormat::ARRAY);
            g_vecOutparamFormat.push_back(type);
        }
        else
        {
            CXmlFormat type(CXmlFormat::DATA);
            g_vecOutparamFormat.push_back(type);
        }

        return 0;
    }

    virtual int dualEnd(CAwkDoc& Doc)
    {
        string xmlcode;

        for (size_t i = 0; i < g_vecOutparamFormat.size(); i++)
        {
            if (g_vecOutparamFormat[i].m_type == CXmlFormat::ARRAY)
            {
                xmlcode += "\t//出错则需要在此添加一个空的数组\n";
                xmlcode += "\trootnode->AddArrayChild();\n\n";
            }
            else if (g_vecOutparamFormat[i].m_type == CXmlFormat::ERRINFO)
            {
                xmlcode += "\trootnode->AddSimpleData( m_sErrorStr );\n\n";
            }
            else if (g_vecOutparamFormat[i].m_type == CXmlFormat::ERRNO)
            {
                xmlcode += "\trootnode->AddUseqChild(); //封装的服务需要增加seq节点\n\n";

                xmlcode += "\tstring tmpstr = IntToStr(m_iErrorCode);"
                    " //xlc不支持将返回值传给引用\n";
                xmlcode += "\trootnode->AddSimpleData( tmpstr );\n\n";
            }
            else
            {
                xmlcode += "\trootnode->AddSimpleData(nullstr);\n\n";
            }
        }

        stringReplaceAll(g_SrvInfo.IB_SRV_Common_cpp, "<setErrorXml>", xmlcode);

        return 0;
    }
};

bool setSetErrorXml()
{
    string filepath = g_outparam + ".txt";
    CParamTypeDual paramTypeDual;
    CAwkDoc paramTypeDoc(&paramTypeDual, filepath);

    return paramTypeDoc.run();
}

