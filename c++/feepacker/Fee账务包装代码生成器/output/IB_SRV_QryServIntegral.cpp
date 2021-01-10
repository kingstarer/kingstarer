/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   查客户积分信息
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_SRV_QryServIntegral.h"

void CIB_SRV_QryServIntegral::setBusinessName()
{
    m_rpcname = "IB_SRV_QryServIntegral";
    m_businessname = "查客户积分信息";
}

char * CIB_SRV_QryServIntegral::ClassName(void)
{
    //这个函数没有被调用
    return "IB_SRV_QryServIntegral";
}   

bool CIB_SRV_QryServIntegral::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //成功时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    QryServIntegralInfo info;
    //将报文转换成组件入参
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("服务入参解析失败");
    }

    if (bRet)
    {
        QryServIntegralInfoOut outinfo;
        
        //调用组件逻辑
        bRet = m_cicsOpr.IB_QryServIntegral(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_QryServIntegral组件调用失败");
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


bool CIB_SRV_QryServIntegral::ParseSrvMsg(HSendMsg *sendmsg, QryServIntegralInfo& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = 6;
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
    if (!copyToBuffer(data.Region, pStr, "地区编码"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Servnumber, pStr, "服务号码"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.CustID, pStr, "客户编号"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Prodtype, pStr, "产品类型"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Integralcyc, pStr, "积分周期"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Brandscoretype, pStr, "全品牌积分类型"))
    {
        return false;
    }


    return true;
}

bool CIB_SRV_QryServIntegral::ParseComMsg(HRecvMsg *rcvmsg, QryServIntegralInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    BASE_LOG_DEBUG("%s", "积分信息数组");
    if ( !m_cicsOpr.ParseArrayToNode(node->AddArrayChild(), data.ACustIntegralArray) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_iErrorCode;
        return false;
    }

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
void CIB_SRV_QryServIntegral::setErrorXml(HRecvMsg * rcvmsg)
{
    //失败时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    //清空数据
    rootnode->FreeChildNode();

    string nullstr;
    //出错则需要在此添加一个空的数组
    rootnode->AddArrayChild();

    rootnode->AddUseqChild(); //封装的服务需要增加seq节点

    string tmpstr = IntToStr(m_iErrorCode); //xlc不支持将返回值传给引用
    rootnode->AddSimpleData( tmpstr );

    rootnode->AddSimpleData( m_sErrorStr );


}


