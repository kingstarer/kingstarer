/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   积分转移
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_COM_MoveIntegral.h"
#include "view/Server.h"

bool IB_COM_MoveIntegral::IB_MoveIntegral( const MoveIntegralInfo& data, MoveIntegralInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "MoveIntegral";

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

bool IB_COM_MoveIntegral::setCallInfo(HRecvMsg *sendmsg, const MoveIntegralInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "业务流水号");
    strValue = data.Businum;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "转出客户编码");
    strValue = data.Outcustid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "转出服务号码");
    strValue = data.OutServnumber;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "转出产品类型");
    strValue = data.OutProdtype;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "地区编码");
    strValue = data.Region;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "转入客户编码");
    strValue = data.Incustid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "转入服务号码");
    strValue = data.InServnumber;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "转入产品类型");
    strValue = data.InProdtype;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "操作员");
    strValue = data.Oprcode;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "积分变更原因");
    strValue = data.Intchgrsn;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "转移积分");
    strValue = data.Moveintegral;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "积分周期");
    strValue = data.Movecycle;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "营业厅");
    strValue = data.Unit;
    node->AddSimpleData(strValue);


    return true;
}


bool IB_COM_MoveIntegral::getCallResult(HSendMsg *rcvmsg, MoveIntegralInfoOut& res)
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

    BASE_LOG_DEBUG("%s", "返回码");
    if( !StrToCstr(res.ErrNo, *(node->DataList[pos++]->GetKey()), 
            "组件结构体[返回码]" ) )
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






