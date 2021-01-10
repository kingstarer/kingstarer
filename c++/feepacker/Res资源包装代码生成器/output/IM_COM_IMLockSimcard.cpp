/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   SIM卡锁定
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IM_COM_IMLockSimcard.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h> 
#include <vector>


bool IM_COM_IMLockSimcard::IM_IMLockSimcard( const IMLockSimcardInfo& data, IMLockSimcardInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "IMLockSimcard";

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

bool IM_COM_IMLockSimcard::setCallInfo(HRecvMsg *sendmsg, const IMLockSimcardInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "地区编码");
    strValue = data.Region;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "单位编码");
    strValue = data.OrgID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "操作员工号");
    strValue = data.OperID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "SIM卡号");
    strValue = data.ICCID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "业务受理流水号");
    strValue = data.sRecOID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "实际销售价格");
    strValue = data.iPrice;
    node->AddSimpleData(strValue);


    return true;
}


bool IM_COM_IMLockSimcard::getCallResult(HSendMsg *rcvmsg, IMLockSimcardInfoOut& res)
{
    LogHere();

    /*返回参数处理*/
    HDataNode *node = rcvmsg->GetRootNode();
    //期望返回参数个数
    const int expectCnt = 2;

    if (! CheckDataListSize(node, expectCnt, m_rpcname))
    {
        return false;
    }

    int pos = 0;

    BASE_LOG_DEBUG("%s", "错误代码");
    if( !StrToCstr(res.RetCode, *(node->DataList[pos++]->GetKey()), 
            "组件结构体[错误代码]" ) )
    {
        return false;
    }

    //返回错误信息
    HDataNode *currNode = node->DataList[pos++];
    m_sErrorStr = *(currNode->GetKey());
    strncpy_ex(res.ErrMsg, m_sErrorStr.c_str());

    m_iErrorCode = 1;
    return true;
}






