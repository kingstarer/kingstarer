/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   充值券包资源预定
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IM_SRV_IMTicketReserve.h"
#include "view/Server.h"

void CIM_SRV_IMTicketReserve::setBusinessName()
{
    m_rpcname = "IM_SRV_IMTicketReserve";
    m_businessname = "充值券包资源预定";
}

char * CIM_SRV_IMTicketReserve::ClassName(void)
{
    //这个函数没有被调用
    return "IM_SRV_IMTicketReserve";
}   

bool CIM_SRV_IMTicketReserve::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //成功时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    IMTicketReserveInfo info;
    //将报文转换成组件入参
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("服务入参解析失败");
    }

    if (bRet)
    {
        IMTicketReserveInfoOut outinfo;
        
        //调用组件逻辑
        bRet = m_cicsOpr.IM_IMTicketReserve(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IM_COM_IMTicketReserve组件调用失败");
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


bool CIM_SRV_IMTicketReserve::ParseSrvMsg(HSendMsg *sendmsg, IMTicketReserveInfo& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = 8;
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
    if (!copyToBuffer(data.RecOID, pStr, "业务流水号"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Quantity, pStr, "冲值券数量"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Restype, pStr, "冲值券资源类型"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Agentid, pStr, "代理商编码"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.PubKey, pStr, "销售员解密公钥"))
    {
        return false;
    }


    return true;
}

bool CIM_SRV_IMTicketReserve::ParseComMsg(HRecvMsg *rcvmsg, IMTicketReserveInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    BASE_LOG_DEBUG("%s", "文件名称");
    strValue = data.FileName;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "充值券包信息列表");
    if ( !m_cicsOpr.ParseArrayToNode(node->AddArrayChild(), data.PacketInfoArray) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_sErrorStr;
        return false;
    }

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
void CIM_SRV_IMTicketReserve::setErrorXml(HRecvMsg * rcvmsg)
{
    //失败时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    //清空数据
    rootnode->FreeChildNode();

    string nullstr;
    rootnode->AddSimpleData(nullstr);

    //出错则需要在此添加一个空的数组
    rootnode->AddArrayChild();

    rootnode->AddUseqChild(); //封装的服务需要增加seq节点

    string tmpstr = IntToStr(m_iErrorCode); //xlc不支持将返回值传给引用
    rootnode->AddSimpleData( tmpstr );

    rootnode->AddSimpleData( m_sErrorStr );


}


