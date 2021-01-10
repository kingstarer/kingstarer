/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   余额流动历史（帐目变更日志）查询接口
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IB_SRV_QryBalanceFlowLog.h"

void CIB_SRV_QryBalanceFlowLog::setBusinessName()
{
    m_rpcname = "IB_SRV_QryBalanceFlowLog";
    m_businessname = "余额流动历史（帐目变更日志）查询接口";
}

char * CIB_SRV_QryBalanceFlowLog::ClassName(void)
{
    //这个函数没有被调用
    return "IB_SRV_QryBalanceFlowLog";
}	

bool CIB_SRV_QryBalanceFlowLog::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //成功时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

	bool bRet = false;
    QryBalanceFlowLogInfo info;
    //将报文转换成组件入参
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("服务入参解析失败");
    }

    if (bRet)
    {
        QryBalanceFlowLogInfoOut outinfo;
        IB_COM_QryBalanceFlowLog cicsOpr;
        //调用组件逻辑
        bRet = cicsOpr.IB_QryBalanceFlowLog(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_QryBalanceFlowLog组件调用失败");
            m_iErrorCode = cicsOpr.m_iErrorCode;
            m_sErrorStr = cicsOpr.m_sErrorStr;
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


bool CIB_SRV_QryBalanceFlowLog::ParseSrvMsg(HSendMsg *sendmsg, QryBalanceFlowLogInfo& data)
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
    
	curnode = rootnode->DataList[pos++];
	pStr = curnode->GetKey();
	if (!copyToBuffer(data.Accoid, pStr, "帐号"))
	{
		return false;
	}

	curnode = rootnode->DataList[pos++];
	pStr = curnode->GetKey();
	if (!copyToBuffer(data.StartTime, pStr, "开始时间"))
	{
		return false;
	}

	curnode = rootnode->DataList[pos++];
	pStr = curnode->GetKey();
	if (!copyToBuffer(data.EndTime, pStr, "结束时间"))
	{
		return false;
	}

	curnode = rootnode->DataList[pos++];
	pStr = curnode->GetKey();
	if (!copyToBuffer(data.qryType, pStr, "查询类型"))
	{
		return false;
	}


	return true;
}

bool CIB_SRV_QryBalanceFlowLog::ParseComMsg(HRecvMsg *rcvmsg, QryBalanceFlowLogInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
	//流动信息数组
	if ( !ParseArrayToNode(node->AddArrayChild(), data.ALogArray) )
	{
		return false;
	}

	//封装服务seq节点
	node->AddUseqChild();

	//返回码
	strValue = data.ErrNo;
	node->AddSimpleData(strValue);

	//返回错误信息
	strValue = data.ErrInfo;
	node->AddSimpleData(strValue);


    return true;
}

//默认的setErrorXml构造的xml报文为[(空数组), (seq结构体), (错误码), (错误信息)]
//如果需要其它形式的错误报文则需要重载此函数，否则可以不重载
 void CIB_SRV_QryBalanceFlowLog::setErrorXml(HRecvMsg * rcvmsg)
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

bool CIB_SRV_QryBalanceFlowLog::ParseArrayToNode(HDataNode *arraynode, const vector<ALog>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

		//帐目（科目）代码
		strValue = nodeinfo[i].SubjectId;
		node->AddSimpleData(strValue);

		//科目开始周期
		strValue = nodeinfo[i].StartCycle;
		node->AddSimpleData(strValue);

		//科目结束周期
		strValue = nodeinfo[i].EndCycle;
		node->AddSimpleData(strValue);

		//流动时间
		strValue = nodeinfo[i].FlowTime;
		node->AddSimpleData(strValue);

		//流动方向
		strValue = nodeinfo[i].FlowDirection;
		node->AddSimpleData(strValue);

		//流动类型
		strValue = nodeinfo[i].FlowType;
		node->AddSimpleData(strValue);

		//变更原因
		strValue = nodeinfo[i].FlowTypeName;
		node->AddSimpleData(strValue);

		//流动金额
		strValue = nodeinfo[i].FlowAmt;
		node->AddSimpleData(strValue);

		//流动后剩余
		strValue = nodeinfo[i].FlowNoBalance;
		node->AddSimpleData(strValue);

		//发起人工号
		strValue = nodeinfo[i].OperID;
		node->AddSimpleData(strValue);

		//流水号
		strValue = nodeinfo[i].Formnum;
		node->AddSimpleData(strValue);

		//来源
		strValue = nodeinfo[i].Source;
		node->AddSimpleData(strValue);


    }

    return true;
}

