/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   集团代付帐单查询
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_SRV_QueryGroupFungiblePayBill.h"

void CIB_SRV_QueryGroupFungiblePayBill::setBusinessName()
{
    m_rpcname = "IB_SRV_QueryGroupFungiblePayBill";
    m_businessname = "集团代付帐单查询";
}

char * CIB_SRV_QueryGroupFungiblePayBill::ClassName(void)
{
    //这个函数没有被调用
    return "IB_SRV_QueryGroupFungiblePayBill";
}   

bool CIB_SRV_QueryGroupFungiblePayBill::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //成功时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    QueryGroupFungiblePayBillInfo info;
    //将报文转换成组件入参
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("服务入参解析失败");
    }

    if (bRet)
    {
        QueryGroupFungiblePayBillInfoOut outinfo;
        
        //调用组件逻辑
        bRet = m_cicsOpr.IB_QueryGroupFungiblePayBill(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_QueryGroupFungiblePayBill组件调用失败");
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


bool CIB_SRV_QueryGroupFungiblePayBill::ParseSrvMsg(HSendMsg *sendmsg, QueryGroupFungiblePayBillInfo& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = 5;
    if (!checkParamIn(sendmsg, expectCnt))
    {
        BASE_LOG_ERROR("%s", m_sErrorStr.c_str());
        return false;
    }

    int pos = 0;
    rootnode = sendmsg->GetRootNode();
    string *pStr = NULL;

    HDataNode *curnode;
    
    BASE_LOG_DEBUG("%s", "地区编码");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Region, pStr, "组件结构体[地区编码]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "单位编码");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.OrgID, pStr, "组件结构体[单位编码]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "操作员工号");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.OperID, pStr, "组件结构体[操作员工号]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "商品类别");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.ResType, pStr, "组件结构体[商品类别]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "商品序号");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.ResSerial, pStr, "组件结构体[商品序号]"))
    {
        return false;
    }


    return true;
}

bool CIB_SRV_QueryGroupFungiblePayBill::ParseComMsg(HRecvMsg *rcvmsg, QueryGroupFungiblePayBillInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    BASE_LOG_DEBUG("%s", "代付帐单数组");
    if ( !m_cicsOpr.ParseArrayToNode(node->AddArrayChild(), data.BillAccountArray) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_iErrorCode;
        return false;
    }

    BASE_LOG_DEBUG("%s", "查询流水号");
    strValue = data.SourceNo;
    node->AddSimpleData(strValue);

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
void CIB_SRV_QueryGroupFungiblePayBill::setErrorXml(HRecvMsg * rcvmsg)
{
    //失败时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    //清空数据
    rootnode->FreeChildNode();

    string nullstr;
    //出错则需要在此添加一个空的数组
    rootnode->AddArrayChild();

    rootnode->AddSimpleData(nullstr);

    rootnode->AddUseqChild(); //封装的服务需要增加seq节点

    string tmpstr = IntToStr(m_iErrorCode); //xlc不支持将返回值传给引用
    rootnode->AddSimpleData( tmpstr );

    rootnode->AddSimpleData( m_sErrorStr );


}


