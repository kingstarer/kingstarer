/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   <组件名称>
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_SRV_<Common>.h"

void CIB_SRV_<Common>::setBusinessName()
{
    m_rpcname = "IB_SRV_<Common>";
    m_businessname = "<组件名称>";
}

char * CIB_SRV_<Common>::ClassName(void)
{
    //这个函数没有被调用
    return "IB_SRV_<Common>";
}	

bool CIB_SRV_<Common>::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //成功时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

	bool bRet = false;
    <inparam> info;
    //将报文转换成组件入参
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("服务入参解析失败");
    }

    if (bRet)
    {
        <outparam> outinfo;
        
        //调用组件逻辑
        bRet = m_cicsOpr.IB_<Common>(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_<Common>组件调用失败");
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
        
        if (!bRet || !strncmp(outinfo.ErrNo, "0", sizeof(outinfo.ErrNo)) )
        {
            BASE_LOG_WARN("cics调用不成功,业务回滚");
            CicsRollBack();
            if (bRet)
            {
                outinfo.ErrNo[sizeof(outinfo.ErrNo) - 1] = '\0';
                BASE_LOG_ERROR("服务返回码[%s]", outinfo.ErrNo);
            }
        }
    }

    return bRet;
}


bool CIB_SRV_<Common>::ParseSrvMsg(HSendMsg *sendmsg, <inparam>& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = <ParseSrvMsgCnt>;
    if (!checkParamIn(sendmsg, expectCnt))
    {
        BASE_LOG_ERROR("%s", m_sErrorStr.c_str());
        return false;
    }

    int pos = 0;
    rootnode = sendmsg->GetRootNode();
    string *pStr = NULL;

    HDataNode *curnode;
    
<ParseSrvMsg>
	return true;
}

bool CIB_SRV_<Common>::ParseComMsg(HRecvMsg *rcvmsg, <outparam>& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
<ParseComMsg>
    return true;
}

//默认的setErrorXml构造的xml报文为[(空数组), (seq结构体), (错误码), (错误信息)]
//如果需要其它形式的错误报文则需要重载此函数，否则可以不重载
void CIB_SRV_<Common>::setErrorXml(HRecvMsg * rcvmsg)
{
    //失败时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    //清空数据
    rootnode->FreeChildNode();

    string nullstr;
<setErrorXml>
}


