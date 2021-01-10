/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   集团代付帐单查询
//
//Others        :   无
//History       :   新增初始版本
//---------------------------------------------------------------*/
#include "IM_SRV_QueryGroupFungiblePayBill.h"

void CIM_SRV_QueryGroupFungiblePayBill::setBusinessName()
{
    m_rpcname = "IM_SRV_QueryGroupFungiblePayBill";
    m_businessname = "集团代付帐单查询";
}

char * CIM_SRV_QueryGroupFungiblePayBill::ClassName(void)
{
    //这个函数没有被调用
    return "IM_SRV_QueryGroupFungiblePayBill";
}   

bool CIM_SRV_QueryGroupFungiblePayBill::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //成功时打包数据到HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    InParam info;
    //将报文转换成组件入参
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("服务入参解析失败");
    }

    if (bRet)
    {
        OutParam outinfo;
        IM_COM_QueryGroupFungiblePayBill cicsOpr;
        //调用组件逻辑
        bRet = cicsOpr.IM_QueryGroupFungiblePayBill(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IM_COM_QueryGroupFungiblePayBill组件调用失败");
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


bool CIM_SRV_QueryGroupFungiblePayBill::ParseSrvMsg(HSendMsg *sendmsg, InParam& data)
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

bool CIM_SRV_QueryGroupFungiblePayBill::ParseComMsg(HRecvMsg *rcvmsg, OutParam& data)
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
void CIM_SRV_QueryGroupFungiblePayBill::setErrorXml(HRecvMsg * rcvmsg)
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

bool CIM_SRV_QueryGroupFungiblePayBill::ParseArrayToNode(HDataNode *arraynode, const vector<ALog>& nodeinfo)
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


