/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   充值券包销售回退
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IM_COM_IMUnBagSale.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <stdlib.h> 
#include <vector>


bool IM_COM_IMUnBagSale::IM_IMUnBagSale( const IMUnBagSaleInfo& data, IMUnBagSaleInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "IMUnBagSale";

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

bool IM_COM_IMUnBagSale::setCallInfo(HRecvMsg *sendmsg, const IMUnBagSaleInfo& data)
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

    BASE_LOG_DEBUG("%s", "代理商编码");
    strValue = data.Agentid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "冲值券包编号列表");
    if ( !ParseArrayToNode(node->AddArrayChild(), data.BagSalePackArray) )
    {
        return false;
    }


    return true;
}


bool IM_COM_IMUnBagSale::getCallResult(HSendMsg *rcvmsg, IMUnBagSaleInfoOut& res)
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

    BASE_LOG_DEBUG("%s", "错误信息列表");
    if( !getHDataNodeResult(res.BagSaleErrInfoArray, node->DataList[pos++], 
            "组件结构体[错误信息列表]" ) )
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


bool IM_COM_IMUnBagSale::getHDataNodeResult(vector<BagSalePack>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //遍历数组元素
    BASE_LOG_DEBUG("结构体数量%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //每个元素期望成员个数
        const int expectParamCnt = 2;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        BagSalePack item;
        string memname(destname);

        if( !StrToCstr(item.PkgSeqBegin, *(node->DataList[pos++]->GetKey()), 
            memname + "[包起始编号]") )
        {
            return false;
        }

        if( !StrToCstr(item.PkgSeqEnd, *(node->DataList[pos++]->GetKey()), 
            memname + "[包结束编号]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IM_COM_IMUnBagSale::getHDataNodeResult(vector<BagSaleErrInfo>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //遍历数组元素
    BASE_LOG_DEBUG("结构体数量%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //每个元素期望成员个数
        const int expectParamCnt = 2;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        BagSaleErrInfo item;
        string memname(destname);

        if( !StrToCstr(item.PkgSeq, *(node->DataList[pos++]->GetKey()), 
            memname + "[包编号]") )
        {
            return false;
        }

        if( !StrToCstr(item.ErrMsg, *(node->DataList[pos++]->GetKey()), 
            memname + "[错误信息]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}



bool IM_COM_IMUnBagSale::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<BagSalePack>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //包起始编号
        strValue = nodeinfo[i].PkgSeqBegin;
        node->AddSimpleData(strValue);

        //包结束编号
        strValue = nodeinfo[i].PkgSeqEnd;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IM_COM_IMUnBagSale::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<BagSaleErrInfo>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //包编号
        strValue = nodeinfo[i].PkgSeq;
        node->AddSimpleData(strValue);

        //错误信息
        strValue = nodeinfo[i].ErrMsg;
        node->AddSimpleData(strValue);


    }

    return true;
}



