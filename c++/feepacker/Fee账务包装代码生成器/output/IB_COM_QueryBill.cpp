/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   查帐单接口
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_COM_QueryBill.h"
#include "view/Server.h"

bool IB_COM_QueryBill::IB_QueryBill( const QueryBillInfo& data, QueryBillInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "QueryBill";

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

bool IB_COM_QueryBill::setCallInfo(HRecvMsg *sendmsg, const QueryBillInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "帐户ID");
    strValue = data.acctid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "用户ID");
    strValue = data.subsid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "开始周期");
    strValue = data.StartCycle;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "结束周期");
    strValue = data.EndCycle;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "帐单状态");
    strValue = data.Status;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "是否显示呆帐费用");
    strValue = data.IsBadDebt;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "怎样对待呆帐费用");
    strValue = data.HowBadDebt;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "返回帐单的形式");
    strValue = data.BillType;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "费用明细代码的组合级别");
    strValue = data.BillFormatNum;
    node->AddSimpleData(strValue);


    return true;
}


bool IB_COM_QueryBill::getCallResult(HSendMsg *rcvmsg, QueryBillInfoOut& res)
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

    BASE_LOG_DEBUG("%s", "帐单数组");
    if( !getHDataNodeResult(res.BillArray, node->DataList[pos++], 
            "组件结构体[帐单数组]" ) )
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


bool IB_COM_QueryBill::getHDataNodeResult(vector<Bill>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //遍历数组元素
    BASE_LOG_DEBUG("结构体数量%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //每个元素期望成员个数
        const int expectParamCnt = 16;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        Bill item;
        string memname(destname);

        if( !StrToCstr(item.Accoid, *(node->DataList[pos++]->GetKey()), 
            memname + "[帐号]") )
        {
            return false;
        }

        if( !StrToCstr(item.Subsoid, *(node->DataList[pos++]->GetKey()), 
            memname + "[用户号]") )
        {
            return false;
        }

        if( !StrToCstr(item.Cycle, *(node->DataList[pos++]->GetKey()), 
            memname + "[帐务周期]") )
        {
            return false;
        }

        if( !StrToCstr(item.Totalfee, *(node->DataList[pos++]->GetKey()), 
            memname + "[总费用（费用明细的合计）]") )
        {
            return false;
        }

        if( !StrToCstr(item.ShouldPay, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月应缴]") )
        {
            return false;
        }

        if( !StrToCstr(item.LeftInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月末剩余积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.isWrtOff, *(node->DataList[pos++]->GetKey()), 
            memname + "[销帐标识]") )
        {
            return false;
        }

        if( !StrToCstr(item.Paidamt, *(node->DataList[pos++]->GetKey()), 
            memname + "[已销费用]") )
        {
            return false;
        }

        if( !StrToCstr(item.TipsInfo, *(node->DataList[pos++]->GetKey()), 
            memname + "[推荐信息]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.FeeDetailArray, node->DataList[pos++], 
            memname + "[费用明细]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.SpDetailArray, node->DataList[pos++], 
            memname + "[代收信息费明细]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.BalanceDetailArray, node->DataList[pos++], 
            memname + "[余额明细]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.IntDetailArray, node->DataList[pos++], 
            memname + "[积分明细]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.MonDataArray, node->DataList[pos++], 
            memname + "[当月费用结构数组]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.SixmonDataArray, node->DataList[pos++], 
            memname + "[半年费用趋势数据数组]") )
        {
            return false;
        }

        if( !getHDataNodeResult(item.TipsDataArray, node->DataList[pos++], 
            memname + "[省钱秘笈数据数组]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<FeeDetail>& nodeinfo, 
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
        FeeDetail item;
        string memname(destname);

        if( !StrToCstr(item.BillItem, *(node->DataList[pos++]->GetKey()), 
            memname + "[是否帐单项]") )
        {
            return false;
        }

        if( !StrToCstr(item.FeeTypeId, *(node->DataList[pos++]->GetKey()), 
            memname + "[费用代码]") )
        {
            return false;
        }

        if( !StrToCstr(item.Feename, *(node->DataList[pos++]->GetKey()), 
            memname + "[费用名称]") )
        {
            return false;
        }

        if( !StrToCstr(item.Fee, *(node->DataList[pos++]->GetKey()), 
            memname + "[费用金额]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<SpDetail>& nodeinfo, 
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
        SpDetail item;
        string memname(destname);

        if( !StrToCstr(item.SpName, *(node->DataList[pos++]->GetKey()), 
            memname + "[服务商名称]") )
        {
            return false;
        }

        if( !StrToCstr(item.BusCode, *(node->DataList[pos++]->GetKey()), 
            memname + "[服务代码]") )
        {
            return false;
        }

        if( !StrToCstr(item.BusName, *(node->DataList[pos++]->GetKey()), 
            memname + "[订购业务名称]") )
        {
            return false;
        }

        if( !StrToCstr(item.Fee, *(node->DataList[pos++]->GetKey()), 
            memname + "[费用金额]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<BalanceDetail>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //遍历数组元素
    BASE_LOG_DEBUG("结构体数量%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //每个元素期望成员个数
        const int expectParamCnt = 13;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        BalanceDetail item;
        string memname(destname);

        if( !StrToCstr(item.BeforeAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月初结余]") )
        {
            return false;
        }

        if( !StrToCstr(item.ZSAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[赠费]") )
        {
            return false;
        }

        if( !StrToCstr(item.DiscAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[优惠]") )
        {
            return false;
        }

        if( !StrToCstr(item.BackFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[退费]") )
        {
            return false;
        }

        if( !StrToCstr(item.PaybyAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[他人代付]") )
        {
            return false;
        }

        if( !StrToCstr(item.InvalidAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月失效费用]") )
        {
            return false;
        }

        if( !StrToCstr(item.DepositAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月沉淀话费]") )
        {
            return false;
        }

        if( !StrToCstr(item.ThisMonBack, *(node->DataList[pos++]->GetKey()), 
            memname + "[分月返还]") )
        {
            return false;
        }

        if( !StrToCstr(item.ThisMonPay, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月累计已交费]") )
        {
            return false;
        }

        if( !StrToCstr(item.Totalfee, *(node->DataList[pos++]->GetKey()), 
            memname + "[费用合计]") )
        {
            return false;
        }

        if( !StrToCstr(item.AfterAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月末结余]") )
        {
            return false;
        }

        if( !StrToCstr(item.ShouldPay, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月末应缴]") )
        {
            return false;
        }

        if( !StrToCstr(item.WillBackAmt, *(node->DataList[pos++]->GetKey()), 
            memname + "[分月未返还金额]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<IntDetail>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //遍历数组元素
    BASE_LOG_DEBUG("结构体数量%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //每个元素期望成员个数
        const int expectParamCnt = 8;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        IntDetail item;
        string memname(destname);

        if( !StrToCstr(item.MonInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.ConsumInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月消费积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.AwardInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月奖励积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.BeforeAvlInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[上月末剩余可兑换积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.AwardedInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月已兑换积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.DonateInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月捐赠积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.LeftInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[本月末剩余积分]") )
        {
            return false;
        }

        if( !StrToCstr(item.CleanInt, *(node->DataList[pos++]->GetKey()), 
            memname + "[年底清零积分]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<MonData>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //遍历数组元素
    BASE_LOG_DEBUG("结构体数量%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //每个元素期望成员个数
        const int expectParamCnt = 8;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        MonData item;
        string memname(destname);

        if( !StrToCstr(item.BaseFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[月基本费]") )
        {
            return false;
        }

        if( !StrToCstr(item.IncreFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[增值业务费]") )
        {
            return false;
        }

        if( !StrToCstr(item.ComFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[通信费]") )
        {
            return false;
        }

        if( !StrToCstr(item.SpFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[代收费]") )
        {
            return false;
        }

        if( !StrToCstr(item.PaybyFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[代他人付费]") )
        {
            return false;
        }

        if( !StrToCstr(item.ReceiveFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[补收费]") )
        {
            return false;
        }

        if( !StrToCstr(item.ViolaFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[违约金]") )
        {
            return false;
        }

        if( !StrToCstr(item.OtherFee, *(node->DataList[pos++]->GetKey()), 
            memname + "[其他]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<SixmonData>& nodeinfo, 
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
        SixmonData item;
        string memname(destname);

        if( !StrToCstr(item.BillCyc, *(node->DataList[pos++]->GetKey()), 
            memname + "[周期]") )
        {
            return false;
        }

        if( !StrToCstr(item.Fee, *(node->DataList[pos++]->GetKey()), 
            memname + "[费用]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_QueryBill::getHDataNodeResult(vector<TipsData>& nodeinfo, 
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
        TipsData item;
        string memname(destname);

        if( !StrToCstr(item.RealUse, *(node->DataList[pos++]->GetKey()), 
            memname + "[实际使用]") )
        {
            return false;
        }

        if( !StrToCstr(item.TipsUse, *(node->DataList[pos++]->GetKey()), 
            memname + "[套餐月费]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}



bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<Bill>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //帐号
        strValue = nodeinfo[i].Accoid;
        node->AddSimpleData(strValue);

        //用户号
        strValue = nodeinfo[i].Subsoid;
        node->AddSimpleData(strValue);

        //帐务周期
        strValue = nodeinfo[i].Cycle;
        node->AddSimpleData(strValue);

        //总费用（费用明细的合计）
        strValue = nodeinfo[i].Totalfee;
        node->AddSimpleData(strValue);

        //本月应缴
        strValue = nodeinfo[i].ShouldPay;
        node->AddSimpleData(strValue);

        //本月末剩余积分
        strValue = nodeinfo[i].LeftInt;
        node->AddSimpleData(strValue);

        //销帐标识
        strValue = nodeinfo[i].isWrtOff;
        node->AddSimpleData(strValue);

        //已销费用
        strValue = nodeinfo[i].Paidamt;
        node->AddSimpleData(strValue);

        //推荐信息
        strValue = nodeinfo[i].TipsInfo;
        node->AddSimpleData(strValue);

        //费用明细
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].FeeDetailArray) )
        {
            return false;
        }
        //代收信息费明细
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].SpDetailArray) )
        {
            return false;
        }
        //余额明细
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].BalanceDetailArray) )
        {
            return false;
        }
        //积分明细
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].IntDetailArray) )
        {
            return false;
        }
        //当月费用结构数组
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].MonDataArray) )
        {
            return false;
        }
        //半年费用趋势数据数组
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].SixmonDataArray) )
        {
            return false;
        }
        //省钱秘笈数据数组
        if ( !ParseArrayToNode(node->AddArrayChild(), nodeinfo[i].TipsDataArray) )
        {
            return false;
        }

    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<FeeDetail>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //是否帐单项
        strValue = nodeinfo[i].BillItem;
        node->AddSimpleData(strValue);

        //费用代码
        strValue = nodeinfo[i].FeeTypeId;
        node->AddSimpleData(strValue);

        //费用名称
        strValue = nodeinfo[i].Feename;
        node->AddSimpleData(strValue);

        //费用金额
        strValue = nodeinfo[i].Fee;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<SpDetail>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //服务商名称
        strValue = nodeinfo[i].SpName;
        node->AddSimpleData(strValue);

        //服务代码
        strValue = nodeinfo[i].BusCode;
        node->AddSimpleData(strValue);

        //订购业务名称
        strValue = nodeinfo[i].BusName;
        node->AddSimpleData(strValue);

        //费用金额
        strValue = nodeinfo[i].Fee;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<BalanceDetail>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //本月初结余
        strValue = nodeinfo[i].BeforeAmt;
        node->AddSimpleData(strValue);

        //赠费
        strValue = nodeinfo[i].ZSAmt;
        node->AddSimpleData(strValue);

        //优惠
        strValue = nodeinfo[i].DiscAmt;
        node->AddSimpleData(strValue);

        //退费
        strValue = nodeinfo[i].BackFee;
        node->AddSimpleData(strValue);

        //他人代付
        strValue = nodeinfo[i].PaybyAmt;
        node->AddSimpleData(strValue);

        //本月失效费用
        strValue = nodeinfo[i].InvalidAmt;
        node->AddSimpleData(strValue);

        //本月沉淀话费
        strValue = nodeinfo[i].DepositAmt;
        node->AddSimpleData(strValue);

        //分月返还
        strValue = nodeinfo[i].ThisMonBack;
        node->AddSimpleData(strValue);

        //本月累计已交费
        strValue = nodeinfo[i].ThisMonPay;
        node->AddSimpleData(strValue);

        //费用合计
        strValue = nodeinfo[i].Totalfee;
        node->AddSimpleData(strValue);

        //本月末结余
        strValue = nodeinfo[i].AfterAmt;
        node->AddSimpleData(strValue);

        //本月末应缴
        strValue = nodeinfo[i].ShouldPay;
        node->AddSimpleData(strValue);

        //分月未返还金额
        strValue = nodeinfo[i].WillBackAmt;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<IntDetail>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //本月积分
        strValue = nodeinfo[i].MonInt;
        node->AddSimpleData(strValue);

        //本月消费积分
        strValue = nodeinfo[i].ConsumInt;
        node->AddSimpleData(strValue);

        //本月奖励积分
        strValue = nodeinfo[i].AwardInt;
        node->AddSimpleData(strValue);

        //上月末剩余可兑换积分
        strValue = nodeinfo[i].BeforeAvlInt;
        node->AddSimpleData(strValue);

        //本月已兑换积分
        strValue = nodeinfo[i].AwardedInt;
        node->AddSimpleData(strValue);

        //本月捐赠积分
        strValue = nodeinfo[i].DonateInt;
        node->AddSimpleData(strValue);

        //本月末剩余积分
        strValue = nodeinfo[i].LeftInt;
        node->AddSimpleData(strValue);

        //年底清零积分
        strValue = nodeinfo[i].CleanInt;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<MonData>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //月基本费
        strValue = nodeinfo[i].BaseFee;
        node->AddSimpleData(strValue);

        //增值业务费
        strValue = nodeinfo[i].IncreFee;
        node->AddSimpleData(strValue);

        //通信费
        strValue = nodeinfo[i].ComFee;
        node->AddSimpleData(strValue);

        //代收费
        strValue = nodeinfo[i].SpFee;
        node->AddSimpleData(strValue);

        //代他人付费
        strValue = nodeinfo[i].PaybyFee;
        node->AddSimpleData(strValue);

        //补收费
        strValue = nodeinfo[i].ReceiveFee;
        node->AddSimpleData(strValue);

        //违约金
        strValue = nodeinfo[i].ViolaFee;
        node->AddSimpleData(strValue);

        //其他
        strValue = nodeinfo[i].OtherFee;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<SixmonData>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //周期
        strValue = nodeinfo[i].BillCyc;
        node->AddSimpleData(strValue);

        //费用
        strValue = nodeinfo[i].Fee;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_QueryBill::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<TipsData>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //实际使用
        strValue = nodeinfo[i].RealUse;
        node->AddSimpleData(strValue);

        //套餐月费
        strValue = nodeinfo[i].TipsUse;
        node->AddSimpleData(strValue);


    }

    return true;
}



