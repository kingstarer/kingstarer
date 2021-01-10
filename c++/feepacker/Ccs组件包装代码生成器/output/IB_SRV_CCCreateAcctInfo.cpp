/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   销帐日志查询接口
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_SRV_CCCreateAcctInfo.h"

void CIB_SRV_CCCreateAcctInfo::setBusinessName()
{
    m_rpcname = "IB_SRV_CCCreateAcctInfo";
    m_businessname = "销帐日志查询接口";
}

char * CIB_SRV_CCCreateAcctInfo::ClassName(void)
{
    //这个函数没有被调用
    return "IB_SRV_CCCreateAcctInfo";
}   

bool CIB_SRV_CCCreateAcctInfo::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //成功时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    CCCreateAcctInfoInfo info;
    //将报文转换成组件入参
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("服务入参解析失败");
    }

    if (bRet)
    {
        CCCreateAcctInfoInfoOut outinfo;
        
        //调用组件逻辑
        bRet = m_cicsOpr.IB_CCCreateAcctInfo(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_CCCreateAcctInfo组件调用失败");
            m_iErrorCode = m_cicsOpr.m_iErrorCode;
            m_sErrorStr = m_cicsOpr.m_sErrorStr;
        }
        else
        {
            //将组件出参转换成xml报文
            bRet = ParseComMsg(rcvmsg, outinfo);
            if (!bRet)
            {
                BASE_LOG_ERROR("将组件出参转换成xml报文失败");
            }
        }
    }

    return bRet;
}


bool CIB_SRV_CCCreateAcctInfo::ParseSrvMsg(HSendMsg *sendmsg, CCCreateAcctInfoInfo& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = 12;
    if (!checkParamIn(sendmsg, expectCnt))
    {
        BASE_LOG_ERROR("%s", m_sErrorStr.c_str());
        return false;
    }

    int pos = 0;
    rootnode = sendmsg->GetRootNode();
    string *pStr = NULL;

    HDataNode *curnode;
    
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.ordid, pStr, "ordid"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.formnum, pStr, "formnum"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.contacttype, pStr, "contacttype"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.operid, pStr, "operid"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.recdate, pStr, "recdate"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.recdefid, pStr, "recdefid"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.idu, pStr, "idu"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.custid, pStr, "custid"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.region, pStr, "region"))
    {
        return false;
    }

    if( !m_cicsOpr.getHDataNodeResult(data.acctlistArray, rootnode->DataList[pos++], 
            "[acctlist]" ) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_sErrorStr;
        return false;
    }

    if( !m_cicsOpr.getHDataNodeResult(data.stltypelistArray, rootnode->DataList[pos++], 
            "[stltypelist]" ) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_sErrorStr;
        return false;
    }

    if( !m_cicsOpr.getHDataNodeResult(data.bmaillistArray, rootnode->DataList[pos++], 
            "[bmaillist]" ) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_sErrorStr;
        return false;
    }


    return true;
}

bool CIB_SRV_CCCreateAcctInfo::ParseComMsg(HRecvMsg *rcvmsg, CCCreateAcctInfoInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    BASE_LOG_DEBUG("%s", "账户ID");
    strValue = data.acctid;
    node->AddSimpleData(strValue);

    //封装服务seq节点
    node->AddUseqChild();

    BASE_LOG_DEBUG("%s", "返回码");
    strValue = data.ErrNo;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "返回错误信息");
    strValue = data.ErrInfo;
    node->AddSimpleData(strValue);


    return true;
}

//默认的setErrorXml构造的xml报文为[(空数组), (seq结构体), (错误码), (错误信息)]
//如果需要其它形式的错误报文则需要重载此函数，否则可以不重载
void CIB_SRV_CCCreateAcctInfo::setErrorXml(HRecvMsg * rcvmsg)
{
    //失败时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    //清空数据
    rootnode->FreeChildNode();

    string nullstr;
    rootnode->AddSimpleData(nullstr);

    rootnode->AddUseqChild(); //封装的服务需要增加seq节点

    string tmpstr = IntToStr(m_iErrorCode); //xlc不支持将返回值传给引用
    rootnode->AddSimpleData( tmpstr );

    rootnode->AddSimpleData( m_sErrorStr );


}


