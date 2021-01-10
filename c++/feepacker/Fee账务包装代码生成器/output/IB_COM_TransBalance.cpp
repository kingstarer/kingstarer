/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   余额转移
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_COM_TransBalance.h"
#include "view/Server.h"

bool IB_COM_TransBalance::IB_TransBalance( const TransBalanceInfo& data, TransBalanceInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "TransBalance";

    //Msg Define
    auto_ptr<HRecvMsg> sendmsg ( new HRecvMsg() );              // 发送数据
    auto_ptr<HSendMsg> rcvmsg  ( new HSendMsg() );              // 接受数据


    BASE_LOG_INFO("将结构体转换成xml报文");

    if ( setCallInfo(sendmsg.get(), data) )
    {
        BASE_LOG_INFO("转换成功");
    }
    else
    {
         BASE_LOG_INFO("转换失败");
         return false;
    }
    

    BASE_LOG_INFO("调用远程接口");
    if ( !CallRemoteCics(sendmsg.get(), rcvmsg.get(), res) )
    {
        BASE_LOG_INFO("调用失败");
        return false;
    }

    BASE_LOG_INFO("解析cics返回报文");
    if (!getCallResult(rcvmsg.get(), res))
    {
        BASE_LOG_ERROR("解析cics返回报文失败");
        return false;
    }
    

    BASE_LOG_DEBUG("组件返回码:%d,返回信息:%s", m_iErrorCode, m_sErrorStr.c_str());
    BASE_LOG_INFO( "-------------执行调用CICS接口END:%s-------------- ", m_rpcname );

    return true;
}

bool IB_COM_TransBalance::setCallInfo(HRecvMsg *sendmsg, const TransBalanceInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "缴费流水号");
    strValue = data.Formnum;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "源帐号");
    strValue = data.FromAccoid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "目的帐号");
    strValue = data.ToAccoid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "营业厅");
    strValue = data.Unit;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "操作员");
    strValue = data.Op;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "业务类型");
    strValue = data.busicode;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "缴费变更类型");
    strValue = data.payReason;
    node->AddSimpleData(strValue);


    return true;
}


bool IB_COM_TransBalance::getCallResult(HSendMsg *rcvmsg, TransBalanceInfoOut& res)
{
    LogHere();

    /*返回参数处理*/
    HDataNode *node = rcvmsg->GetRootNode();
    //期望返回参数个数
    const int expectCnt = 3;

    if (! CheckDataListSize(node, expectCnt, m_rpcname))
    {
        return false;
    }

    int pos = 0;

    BASE_LOG_DEBUG("%s", "返回码");
    if( !StrToCstr(res.ErrNo, *(node->DataList[pos++]->GetKey()), 
            "组件结构体[返回码]" ) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "转移金额");
    if( !StrToCstr(res.TransFee, *(node->DataList[pos++]->GetKey()), 
            "组件结构体[转移金额]" ) )
    {
        return false;
    }

    //返回错误信息
    HDataNode *currNode = node->DataList[pos++];
    m_sErrorStr = *(currNode->GetKey());
    strncpy_ex(res.ErrInfo, m_sErrorStr.c_str());

    m_iErrorCode = 1;
    return true;
}






