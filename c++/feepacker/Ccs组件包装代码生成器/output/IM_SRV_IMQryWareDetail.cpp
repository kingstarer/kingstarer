/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   集团代付帐单查询
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IM_SRV_IMQryWareDetail.h"

void CIM_SRV_IMQryWareDetail::setBusinessName()
{
    m_rpcname = "IM_SRV_IMQryWareDetail";
    m_businessname = "集团代付帐单查询";
}

char * CIM_SRV_IMQryWareDetail::ClassName(void)
{
    //这个函数没有被调用
    return "IM_SRV_IMQryWareDetail";
}   

bool CIM_SRV_IMQryWareDetail::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //成功时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    IMQryWareDetailInfo info;
    //将报文转换成组件入参
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("服务入参解析失败");
    }

    if (bRet)
    {
        IMQryWareDetailInfoOut outinfo;
        
        //调用组件逻辑
        bRet = m_cicsOpr.IM_IMQryWareDetail(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IM_COM_IMQryWareDetail组件调用失败");
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


bool CIM_SRV_IMQryWareDetail::ParseSrvMsg(HSendMsg *sendmsg, IMQryWareDetailInfo& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = 4;
    if (!checkParamIn(sendmsg, expectCnt))
    {
        BASE_LOG_ERROR("%s", m_sErrorStr.c_str());
        return false;
    }

    int pos = 0;
    rootnode = sendmsg->GetRootNode();
    string *pStr = NULL;

    HDataNode *curnode;
    
    BASE_LOG_DEBUG("%s", "帐号");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Accoid, pStr, "组件结构体[帐号]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "开始时间");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.StartTime, pStr, "组件结构体[开始时间]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "结束时间");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.EndTime, pStr, "组件结构体[结束时间]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "查询类型");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.qryType, pStr, "组件结构体[查询类型]"))
    {
        return false;
    }


    return true;
}

bool CIM_SRV_IMQryWareDetail::ParseComMsg(HRecvMsg *rcvmsg, IMQryWareDetailInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    BASE_LOG_DEBUG("%s", "ucid数组");
    if ( !m_cicsOpr.ParseArrayToNode(node->AddArrayChild(), data.UcidArray) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_iErrorCode;
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
void CIM_SRV_IMQryWareDetail::setErrorXml(HRecvMsg * rcvmsg)
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


