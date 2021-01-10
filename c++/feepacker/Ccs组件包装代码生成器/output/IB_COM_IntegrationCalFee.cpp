/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   智能网算费接口
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_COM_IntegrationCalFee.h"
#include "view/Server.h"

bool IB_COM_IntegrationCalFee::IB_IntegrationCalFee( const IntegrationCalFeeInfo& data, IntegrationCalFeeInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "IntegrationCalFee";

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

bool IB_COM_IntegrationCalFee::setCallInfo(HRecvMsg *sendmsg, const IntegrationCalFeeInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "用户编号");
    strValue = data.subsid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "业务标识");
    strValue = data.recflag;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "产品申请列表");
    if ( !ParseArrayToNode(node->AddArrayChild(), data.ProdListArray) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "营销方案申请列表");
    if ( !ParseArrayToNode(node->AddArrayChild(), data.YxPlanListArray) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "业务类型");
    strValue = data.TradeType;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "激活时间");
    strValue = data.ActiveTime;
    node->AddSimpleData(strValue);


    return true;
}


bool IB_COM_IntegrationCalFee::getCallResult(HSendMsg *rcvmsg, IntegrationCalFeeInfoOut& res)
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

    BASE_LOG_DEBUG("%s", "算费结果列表");
    if( !getHDataNodeResult(res.CalFeeResultListArray, node->DataList[pos++], 
            "组件结构体[算费结果列表]" ) )
    {
        return false;
    }

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


bool IB_COM_IntegrationCalFee::getHDataNodeResult(vector<ProdList>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //遍历数组元素
    BASE_LOG_DEBUG("结构体数量%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //每个元素期望成员个数
        const int expectParamCnt = 3;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        ProdList item;
        string memname(destname);

        if( !StrToCstr(item.ProdId, *(node->DataList[pos++]->GetKey()), 
            memname + "[产品编号]") )
        {
            return false;
        }

        if( !StrToCstr(item.StratTime, *(node->DataList[pos++]->GetKey()), 
            memname + "[生效时间]") )
        {
            return false;
        }

        if( !StrToCstr(item.StopTime, *(node->DataList[pos++]->GetKey()), 
            memname + "[失效时间]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_IntegrationCalFee::getHDataNodeResult(vector<YxPlanList>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //遍历数组元素
    BASE_LOG_DEBUG("结构体数量%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //每个元素期望成员个数
        const int expectParamCnt = 3;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        YxPlanList item;
        string memname(destname);

        if( !StrToCstr(item.YxPlanId, *(node->DataList[pos++]->GetKey()), 
            memname + "[营销方案编号]") )
        {
            return false;
        }

        if( !StrToCstr(item.StratTime, *(node->DataList[pos++]->GetKey()), 
            memname + "[生效时间]") )
        {
            return false;
        }

        if( !StrToCstr(item.StopTime, *(node->DataList[pos++]->GetKey()), 
            memname + "[失效时间]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_IntegrationCalFee::getHDataNodeResult(vector<CalFeeResultList>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //遍历数组元素
    BASE_LOG_DEBUG("结构体数量%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //每个元素期望成员个数
        const int expectParamCnt = 3;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        CalFeeResultList item;
        string memname(destname);

        if( !StrToCstr(item.acctid, *(node->DataList[pos++]->GetKey()), 
            memname + "[帐单科目名称]") )
        {
            return false;
        }

        if( !StrToCstr(item.recamt, *(node->DataList[pos++]->GetKey()), 
            memname + "[实收金额]") )
        {
            return false;
        }

        if( !StrToCstr(item.subamt, *(node->DataList[pos++]->GetKey()), 
            memname + "[减免金额]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}



bool IB_COM_IntegrationCalFee::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<ProdList>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //产品编号
        strValue = nodeinfo[i].ProdId;
        node->AddSimpleData(strValue);

        //生效时间
        strValue = nodeinfo[i].StratTime;
        node->AddSimpleData(strValue);

        //失效时间
        strValue = nodeinfo[i].StopTime;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_IntegrationCalFee::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<YxPlanList>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //营销方案编号
        strValue = nodeinfo[i].YxPlanId;
        node->AddSimpleData(strValue);

        //生效时间
        strValue = nodeinfo[i].StratTime;
        node->AddSimpleData(strValue);

        //失效时间
        strValue = nodeinfo[i].StopTime;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_IntegrationCalFee::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<CalFeeResultList>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //帐单科目名称
        strValue = nodeinfo[i].acctid;
        node->AddSimpleData(strValue);

        //实收金额
        strValue = nodeinfo[i].recamt;
        node->AddSimpleData(strValue);

        //减免金额
        strValue = nodeinfo[i].subamt;
        node->AddSimpleData(strValue);


    }

    return true;
}



