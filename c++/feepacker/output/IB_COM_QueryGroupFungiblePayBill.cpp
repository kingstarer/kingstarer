/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   集团代付帐单查询
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_COM_QueryGroupFungiblePayBill.h"
#include "view/Server.h"


bool IB_COM_QueryGroupFungiblePayBill::IB_QueryGroupFungiblePayBill( const QueryGroupFungiblePayBillInfo& data, QueryGroupFungiblePayBillInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "QueryGroupFungiblePayBill";

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

bool IB_COM_QueryGroupFungiblePayBill::setCallInfo(HRecvMsg *sendmsg, const QueryGroupFungiblePayBillInfo& data)
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

    BASE_LOG_DEBUG("%s", "商品类别");
    strValue = data.ResType;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "商品序号");
    strValue = data.ResSerial;
    node->AddSimpleData(strValue);


    return true;
}


bool IB_COM_QueryGroupFungiblePayBill::getCallResult(HSendMsg *rcvmsg, QueryGroupFungiblePayBillInfoOut& res)
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

    BASE_LOG_DEBUG("%s", "代付帐单数组");
    if( !getHDataNodeResult(res.BillAccountArray, node->DataList[pos++], 
            "组件结构体[代付帐单数组]" ) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "查询流水号");
    if( !StrToCstr(res.SourceNo, *(node->DataList[pos++]->GetKey()), 
            "组件结构体[查询流水号]" ) )
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


bool IB_COM_QueryGroupFungiblePayBill::getHDataNodeResult(vector<BillAccount>& nodeinfo, 
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
        BillAccount item;
        string memname(destname);

        if( !StrToCstr(item.GroupID, *(node->DataList[pos++]->GetKey()), 
            memname + "[集团编码]") )
        {
            return false;
        }

        if( !StrToCstr(item.Cycle, *(node->DataList[pos++]->GetKey()), 
            memname + "[帐务周期]") )
        {
            return false;
        }

        if( !StrToCstr(item.Totalfee, *(node->DataList[pos++]->GetKey()), 
            memname + "[总费用（所有费用项的合计）]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.CustMemberBillArray, node->DataList[pos++], 
            memname + "[费用明细]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryGroupFungiblePayBill::getHDataNodeResult(vector<CustMemberBill>& nodeinfo, 
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
        CustMemberBill item;
        string memname(destname);

        if( !StrToCstr(item.ServNum, *(node->DataList[pos++]->GetKey()), 
            memname + "[代付的产品号码]") )
        {
            return false;
        }

        if( !StrToCstr(item.Name, *(node->DataList[pos++]->GetKey()), 
            memname + "[代付的产品名称]") )
        {
            return false;
        }

        if( !StrToCstr(item.FungibleRate, *(node->DataList[pos++]->GetKey()), 
            memname + "[代付额度]") )
        {
            return false;
        }

        if( !StrToCstr(item.RealPayFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[实际代付金额]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}



bool IB_COM_QueryGroupFungiblePayBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<BillAccount>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //集团编码
        strValue = nodeinfo[i].GroupID;
        node->AddSimpleData(strValue);

        //帐务周期
        strValue = nodeinfo[i].Cycle;
        node->AddSimpleData(strValue);

        //总费用（所有费用项的合计）
        strValue = nodeinfo[i].Totalfee;
        node->AddSimpleData(strValue);

        //费用明细
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].CustMemberBillArray) )
        {
            return false;
        }

    }

    return true;
}

bool IB_COM_QueryGroupFungiblePayBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<CustMemberBill>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //代付的产品号码
        strValue = nodeinfo[i].ServNum;
        node->AddSimpleData(strValue);

        //代付的产品名称
        strValue = nodeinfo[i].Name;
        node->AddSimpleData(strValue);

        //代付额度
        strValue = nodeinfo[i].FungibleRate;
        node->AddSimpleData(strValue);

        //实际代付金额
        strValue = nodeinfo[i].RealPayFee;
        node->AddSimpleData(strValue);


    }

    return true;
}



