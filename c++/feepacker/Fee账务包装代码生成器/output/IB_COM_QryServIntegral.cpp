/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   查客户积分信息
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_COM_QryServIntegral.h"
#include "view/Server.h"

bool IB_COM_QryServIntegral::IB_QryServIntegral( const QryServIntegralInfo& data, QryServIntegralInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "QryServIntegral";

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

bool IB_COM_QryServIntegral::setCallInfo(HRecvMsg *sendmsg, const QryServIntegralInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "地区编码");
    strValue = data.Region;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "服务号码");
    strValue = data.Servnumber;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "客户编号");
    strValue = data.CustID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "产品类型");
    strValue = data.Prodtype;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "积分周期");
    strValue = data.Integralcyc;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "全品牌积分类型");
    strValue = data.Brandscoretype;
    node->AddSimpleData(strValue);


    return true;
}


bool IB_COM_QryServIntegral::getCallResult(HSendMsg *rcvmsg, QryServIntegralInfoOut& res)
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

    BASE_LOG_DEBUG("%s", "积分信息数组");
    if( !getHDataNodeResult(res.ACustIntegralArray, node->DataList[pos++], 
            "组件结构体[积分信息数组]" ) )
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


bool IB_COM_QryServIntegral::getHDataNodeResult(vector<ACustIntegral>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //遍历数组元素
    BASE_LOG_DEBUG("结构体数量%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //每个元素期望成员个数
        const int expectParamCnt = 10;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        ACustIntegral item;
        string memname(destname);

        if( !StrToCstr(item.ServNumber, *(node->DataList[pos++]->GetKey()), 
            memname + "[服务号码]") )
        {
            return false;
        }

        if( !StrToCstr(item.Totalintegral, *(node->DataList[pos++]->GetKey()), 
            memname + "[总积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.Integralcyc, *(node->DataList[pos++]->GetKey()), 
            memname + "[积分周期]") )
        {
            return false;
        }

        if( !StrToCstr(item.Availintegral, *(node->DataList[pos++]->GetKey()), 
            memname + "[可用积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.Moveintegral, *(node->DataList[pos++]->GetKey()), 
            memname + "[转移积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.Unavailintegral, *(node->DataList[pos++]->GetKey()), 
            memname + "[失效积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.Saleintegral, *(node->DataList[pos++]->GetKey()), 
            memname + "[购买积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.Updatetime, *(node->DataList[pos++]->GetKey()), 
            memname + "[最新更新时间]") )
        {
            return false;
        }

        if( !StrToCstr(item.Stoptime, *(node->DataList[pos++]->GetKey()), 
            memname + "[失效时间]") )
        {
            return false;
        }

        if( !StrToCstr(item.Brandscoretype, *(node->DataList[pos++]->GetKey()), 
            memname + "[全品牌积分类型]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}



bool IB_COM_QryServIntegral::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<ACustIntegral>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //服务号码
        strValue = nodeinfo[i].ServNumber;
        node->AddSimpleData(strValue);

        //总积分
        strValue = nodeinfo[i].Totalintegral;
        node->AddSimpleData(strValue);

        //积分周期
        strValue = nodeinfo[i].Integralcyc;
        node->AddSimpleData(strValue);

        //可用积分
        strValue = nodeinfo[i].Availintegral;
        node->AddSimpleData(strValue);

        //转移积分
        strValue = nodeinfo[i].Moveintegral;
        node->AddSimpleData(strValue);

        //失效积分
        strValue = nodeinfo[i].Unavailintegral;
        node->AddSimpleData(strValue);

        //购买积分
        strValue = nodeinfo[i].Saleintegral;
        node->AddSimpleData(strValue);

        //最新更新时间
        strValue = nodeinfo[i].Updatetime;
        node->AddSimpleData(strValue);

        //失效时间
        strValue = nodeinfo[i].Stoptime;
        node->AddSimpleData(strValue);

        //全品牌积分类型
        strValue = nodeinfo[i].Brandscoretype;
        node->AddSimpleData(strValue);


    }

    return true;
}



