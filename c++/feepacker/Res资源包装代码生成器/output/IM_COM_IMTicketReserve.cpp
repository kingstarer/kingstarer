/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   充值券包资源预定
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IM_COM_IMTicketReserve.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h> 
#include <vector>


bool IM_COM_IMTicketReserve::IM_IMTicketReserve( const IMTicketReserveInfo& data, IMTicketReserveInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "IMTicketReserve";

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

bool IM_COM_IMTicketReserve::setCallInfo(HRecvMsg *sendmsg, const IMTicketReserveInfo& data)
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

    BASE_LOG_DEBUG("%s", "业务流水号");
    strValue = data.RecOID;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "冲值券数量");
    strValue = data.Quantity;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "冲值券资源类型");
    strValue = data.Restype;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "代理商编码");
    strValue = data.Agentid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "销售员解密公钥");
    strValue = data.PubKey;
    node->AddSimpleData(strValue);


    return true;
}


bool IM_COM_IMTicketReserve::getCallResult(HSendMsg *rcvmsg, IMTicketReserveInfoOut& res)
{
    LogHere();

    /*返回参数处理*/
    HDataNode *node = rcvmsg->GetRootNode();
    //期望返回参数个数
    const int expectCnt = 4;

    if (! CheckDataListSize(node, expectCnt, m_rpcname))
    {
        return false;
    }

    int pos = 0;

    BASE_LOG_DEBUG("%s", "文件名称");
    if( !StrToCstr(res.FileName, *(node->DataList[pos++]->GetKey()), 
            "组件结构体[文件名称]" ) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "充值券包信息列表");
    if( !getHDataNodeResult(res.PacketInfoArray, node->DataList[pos++], 
            "组件结构体[充值券包信息列表]" ) )
    {
        return false;
    }

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


bool IM_COM_IMTicketReserve::getHDataNodeResult(vector<PacketInfo>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //遍历数组元素
    BASE_LOG_DEBUG("结构体数量%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //每个元素期望成员个数
        const int expectParamCnt = 4;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        PacketInfo item;
        string memname(destname);

        if( !StrToCstr(item.PkgSeq, *(node->DataList[pos++]->GetKey()), 
            memname + "[充值券包编号]") )
        {
            return false;
        }

        if( !StrToCstr(item.PkgKing, *(node->DataList[pos++]->GetKey()), 
            memname + "[包种类]") )
        {
            return false;
        }

        if( !StrToCstr(item.PkgSeqEnd, *(node->DataList[pos++]->GetKey()), 
            memname + "[批次号]") )
        {
            return false;
        }

        if( !StrToCstr(item.ParValue, *(node->DataList[pos++]->GetKey()), 
            memname + "[充值券包的单价]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}



bool IM_COM_IMTicketReserve::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<PacketInfo>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //充值券包编号
        strValue = nodeinfo[i].PkgSeq;
        node->AddSimpleData(strValue);

        //包种类
        strValue = nodeinfo[i].PkgKing;
        node->AddSimpleData(strValue);

        //批次号
        strValue = nodeinfo[i].PkgSeqEnd;
        node->AddSimpleData(strValue);

        //充值券包的单价
        strValue = nodeinfo[i].ParValue;
        node->AddSimpleData(strValue);


    }

    return true;
}



