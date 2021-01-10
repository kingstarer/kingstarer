/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   查帐单接口
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_SRV_QueryBill.h"

void CIB_SRV_QueryBill::setBusinessName()
{
    m_rpcname = "IB_SRV_QueryBill";
    m_businessname = "查帐单接口";
}

char * CIB_SRV_QueryBill::ClassName(void)
{
    //这个函数没有被调用
    return "IB_SRV_QueryBill";
}   

bool CIB_SRV_QueryBill::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //成功时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    QueryBillInfo info;
    //将报文转换成组件入参
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("服务入参解析失败");
    }

    if (bRet)
    {
        QueryBillInfoOut outinfo;
        
        //调用组件逻辑
        bRet = m_cicsOpr.IB_QueryBill(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_QueryBill组件调用失败");
            m_iErrorCode = m_cicsOpr.m_iErrorCode;
            m_sErrorStr = m_cicsOpr.m_sErrorStr;
        }
        else
        {
            //将组件出参转换成xml报文
            bRet = ParseComMsg(rcvmsg, outinfo);
            if (!bRet)
            {
                BASE_LOG_ERROR("将组件出参转换成xml报文失败");
            }
        }
    }

    return bRet;
}


bool CIB_SRV_QueryBill::ParseSrvMsg(HSendMsg *sendmsg, QueryBillInfo& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = 9;
    if (!checkParamIn(sendmsg, expectCnt))
    {
        BASE_LOG_ERROR("%s", m_sErrorStr.c_str());
        return false;
    }

    int pos = 0;
    rootnode = sendmsg->GetRootNode();
    string *pStr = NULL;

    HDataNode *curnode;
    
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.acctid, pStr, "帐户ID"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.subsid, pStr, "用户ID"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.StartCycle, pStr, "开始周期"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.EndCycle, pStr, "结束周期"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Status, pStr, "帐单状态"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.IsBadDebt, pStr, "是否显示呆帐费用"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.HowBadDebt, pStr, "怎样对待呆帐费用"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.BillType, pStr, "返回帐单的形式"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.BillFormatNum, pStr, "费用明细代码的组合级别"))
    {
        return false;
    }


    return true;
}

bool CIB_SRV_QueryBill::ParseComMsg(HRecvMsg *rcvmsg, QueryBillInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    BASE_LOG_DEBUG("%s", "帐单数组");
    if ( !m_cicsOpr.ParseArrayToNode(node->AddArrayChild(), data.BillArray) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_iErrorCode;
        return false;
    }

    //封装服务seq节点
    node->AddUseqChild();

    BASE_LOG_DEBUG("%s", "返回码");
    strValue = data.ErrNo;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "返回错误信息");
    strValue = data.ErrInfo;
    node->AddSimpleData(strValue);


    return true;
}

//默认的setErrorXml构造的xml报文为[(空数组), (seq结构体), (错误码), (错误信息)]
//如果需要其它形式的错误报文则需要重载此函数，否则可以不重载
void CIB_SRV_QueryBill::setErrorXml(HRecvMsg * rcvmsg)
{
    //失败时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    //清空数据
    rootnode->FreeChildNode();

    string nullstr;
    //出错则需要在此添加一个空的数组
    rootnode->AddArrayChild();

    rootnode->AddUseqChild(); //封装的服务需要增加seq节点

    string tmpstr = IntToStr(m_iErrorCode); //xlc不支持将返回值传给引用
    rootnode->AddSimpleData( tmpstr );

    rootnode->AddSimpleData( m_sErrorStr );


}


