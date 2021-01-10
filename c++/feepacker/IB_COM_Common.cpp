/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   <组件名称>
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_COM_<Common>.h"
#include "view/Server.h"


bool IB_COM_<Common>::IB_<Common>( const <inparam>& data, <outparam>& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "<Common>";

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
        BASE_LOG_ERROR("转换失败");
        return false;
    }
    

    BASE_LOG_INFO("调用远程接口");
    if ( !CallRemoteCics(sendmsg.get(), rcvmsg.get(), res) )
    {
        BASE_LOG_ERROR("调用失败");
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

bool IB_COM_<Common>::setCallInfo(HRecvMsg *sendmsg, const <inparam>& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
<setCallInfo>
    return true;
}


bool IB_COM_<Common>::getCallResult(HSendMsg *rcvmsg, <outparam>& res)
{
    LogHere();

    /*返回参数处理*/
    HDataNode *node = rcvmsg->GetRootNode();
    //期望返回参数个数
    const int expectCnt = <getCallResultCnt>;

    if (! CheckDataListSize(node, expectCnt, m_rpcname))
    {
        return false;
    }

    int pos = 0;

<getCallResult>
    m_iErrorCode = 1;
    return true;
}


<getHDataNodeResultDefineImplement>

<ParseArrayToNodeDefineImplement>

