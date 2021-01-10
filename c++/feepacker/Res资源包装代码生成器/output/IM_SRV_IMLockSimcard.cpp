/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   SIM卡锁定
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IM_SRV_IMLockSimcard.h"
#include "view/Server.h"

void CIM_SRV_IMLockSimcard::setBusinessName()
{
    m_rpcname = "IM_SRV_IMLockSimcard";
    m_businessname = "SIM卡锁定";
}

char * CIM_SRV_IMLockSimcard::ClassName(void)
{
    //这个函数没有被调用
    return "IM_SRV_IMLockSimcard";
}   

bool CIM_SRV_IMLockSimcard::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //成功时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    IMLockSimcardInfo info;
    //将报文转换成组件入参
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("服务入参解析失败");
    }

    if (bRet)
    {
        IMLockSimcardInfoOut outinfo;
        
        //调用组件逻辑
        bRet = m_cicsOpr.IM_IMLockSimcard(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IM_COM_IMLockSimcard组件调用失败");
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


bool CIM_SRV_IMLockSimcard::ParseSrvMsg(HSendMsg *sendmsg, IMLockSimcardInfo& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = 6;
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
    if (!copyToBuffer(data.Region, pStr, "地区编码"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.OrgID, pStr, "单位编码"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.OperID, pStr, "操作员工号"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.ICCID, pStr, "SIM卡号"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.sRecOID, pStr, "业务受理流水号"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.iPrice, pStr, "实际销售价格"))
    {
        return false;
    }


    return true;
}

bool CIM_SRV_IMLockSimcard::ParseComMsg(HRecvMsg *rcvmsg, IMLockSimcardInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    //封装服务seq节点
    node->AddUseqChild();

    BASE_LOG_DEBUG("%s", "错误代码");
    strValue = data.RetCode;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "操作失败原因");
    strValue = data.ErrMsg;
    node->AddSimpleData(strValue);


    return true;
}

//默认的setErrorXml构造的xml报文为[(空数组), (seq结构体), (错误码), (错误信息)]
//如果需要其它形式的错误报文则需要重载此函数，否则可以不重载
void CIM_SRV_IMLockSimcard::setErrorXml(HRecvMsg * rcvmsg)
{
    //失败时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    //清空数据
    rootnode->FreeChildNode();

    string nullstr;
    rootnode->AddUseqChild(); //封装的服务需要增加seq节点

    string tmpstr = IntToStr(m_iErrorCode); //xlc不支持将返回值传给引用
    rootnode->AddSimpleData( tmpstr );

    rootnode->AddSimpleData( m_sErrorStr );


}


