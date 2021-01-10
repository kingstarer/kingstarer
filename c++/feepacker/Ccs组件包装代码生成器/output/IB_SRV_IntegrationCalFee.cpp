/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   智能网算费接口
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_SRV_IntegrationCalFee.h"

void CIB_SRV_IntegrationCalFee::setBusinessName()
{
    m_rpcname = "IB_SRV_IntegrationCalFee";
    m_businessname = "智能网算费接口";
}

char * CIB_SRV_IntegrationCalFee::ClassName(void)
{
    //这个函数没有被调用
    return "IB_SRV_IntegrationCalFee";
}   

bool CIB_SRV_IntegrationCalFee::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //成功时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    IntegrationCalFeeInfo info;
    //将报文转换成组件入参
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("服务入参解析失败");
    }

    if (bRet)
    {
        IntegrationCalFeeInfoOut outinfo;
        
        //调用组件逻辑
        bRet = m_cicsOpr.IB_IntegrationCalFee(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_IntegrationCalFee组件调用失败");
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


bool CIB_SRV_IntegrationCalFee::ParseSrvMsg(HSendMsg *sendmsg, IntegrationCalFeeInfo& data)
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
    if (!copyToBuffer(data.subsid, pStr, "用户编号"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.recflag, pStr, "业务标识"))
    {
        return false;
    }

    if( !m_cicsOpr.getHDataNodeResult(data.ProdListArray, rootnode->DataList[pos++], 
            "[产品申请列表]" ) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_iErrorCode;
        return false;
    }

    if( !m_cicsOpr.getHDataNodeResult(data.YxPlanListArray, rootnode->DataList[pos++], 
            "[营销方案申请列表]" ) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_iErrorCode;
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.TradeType, pStr, "业务类型"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.ActiveTime, pStr, "激活时间"))
    {
        return false;
    }


    return true;
}

bool CIB_SRV_IntegrationCalFee::ParseComMsg(HRecvMsg *rcvmsg, IntegrationCalFeeInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    BASE_LOG_DEBUG("%s", "算费结果列表");
    if ( !m_cicsOpr.ParseArrayToNode(node->AddArrayChild(), data.CalFeeResultListArray) )
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
void CIB_SRV_IntegrationCalFee::setErrorXml(HRecvMsg * rcvmsg)
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


