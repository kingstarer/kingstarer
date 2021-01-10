/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-11
//Description   :   ������־��ѯ�ӿ�
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_COM_CCCreateAcctInfo.h"
#include "view/Server.h"

bool IB_COM_CCCreateAcctInfo::IB_CCCreateAcctInfo( const CCCreateAcctInfoInfo& data, CCCreateAcctInfoInfoOut& res )
{
    LogHere();
    RecordFlow();

    m_rpcname = "CCCreateAcctInfo";

    //Msg Define
    auto_ptr<HRecvMsg> sendmsg ( new HRecvMsg() );              // ��������
    auto_ptr<HSendMsg> rcvmsg  ( new HSendMsg() );              // ��������


    BASE_LOG_INFO("���ṹ��ת����xml����");

    if ( setCallInfo(sendmsg.get(), data) )
    {
        BASE_LOG_INFO("ת���ɹ�");
    }
    else
    {
         BASE_LOG_INFO("ת��ʧ��");
         return false;
    }
    

    BASE_LOG_INFO("����Զ�̽ӿ�");
    if ( !CallRemoteCics(sendmsg.get(), rcvmsg.get(), res) )
    {
        BASE_LOG_INFO("����ʧ��");
        return false;
    }

    BASE_LOG_INFO("����cics���ر���");
    if (!getCallResult(rcvmsg.get(), res))
    {
        BASE_LOG_ERROR("����cics���ر���ʧ��");
        return false;
    }
    

    BASE_LOG_DEBUG("���������:%d,������Ϣ:%s", m_iErrorCode, m_sErrorStr.c_str());
    BASE_LOG_INFO( "-------------ִ�е���CICS�ӿ�END:%s-------------- ", m_rpcname );

    return true;
}

bool IB_COM_CCCreateAcctInfo::setCallInfo(HRecvMsg *sendmsg, const CCCreateAcctInfoInfo& data)
{
    LogHere();

    HDataNode *node = sendmsg->GetRootNode();

    string strValue;
    BASE_LOG_DEBUG("%s", "ordid");
    strValue = data.ordid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "formnum");
    strValue = data.formnum;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "contacttype");
    strValue = data.contacttype;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "operid");
    strValue = data.operid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "recdate");
    strValue = data.recdate;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "recdefid");
    strValue = data.recdefid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "idu");
    strValue = data.idu;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "custid");
    strValue = data.custid;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "region");
    strValue = data.region;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "acctlist");
    if ( !ParseArrayToNode(node->AddArrayChild(), data.acctlistArray) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "stltypelist");
    if ( !ParseArrayToNode(node->AddArrayChild(), data.stltypelistArray) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "bmaillist");
    if ( !ParseArrayToNode(node->AddArrayChild(), data.bmaillistArray) )
    {
        return false;
    }


    return true;
}


bool IB_COM_CCCreateAcctInfo::getCallResult(HSendMsg *rcvmsg, CCCreateAcctInfoInfoOut& res)
{
    LogHere();

    /*���ز�������*/
    HDataNode *node = rcvmsg->GetRootNode();
    //�������ز�������
    const int expectCnt = 3;

    if (! CheckDataListSize(node, expectCnt, m_rpcname))
    {
        return false;
    }

    int pos = 0;

    BASE_LOG_DEBUG("%s", "�˻�ID");
    if( !StrToCstr(res.acctid, *(node->DataList[pos++]->GetKey()), 
            "����ṹ��[�˻�ID]" ) )
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "������");
    if( !StrToCstr(res.ErrNo, *(node->DataList[pos++]->GetKey()), 
            "����ṹ��[������]" ) )
    {
        return false;
    }

    //���ش�����Ϣ
    HDataNode *currNode = node->DataList[pos++];
    m_sErrorStr = *(currNode->GetKey());
    strncpy_ex(res.ErrInfo, m_sErrorStr.c_str());

    m_iErrorCode = 1;
    return true;
}


bool IB_COM_CCCreateAcctInfo::getHDataNodeResult(vector<acctlist>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 18;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        acctlist item;
        string memname(destname);

        if( !StrToCstr(item.acctid, *(node->DataList[pos++]->GetKey()), 
            memname + "[acctid]") )
        {
            return false;
        }

        if( !StrToCstr(item.prepaytype, *(node->DataList[pos++]->GetKey()), 
            memname + "[prepaytype]") )
        {
            return false;
        }

        if( !StrToCstr(item.acctname, *(node->DataList[pos++]->GetKey()), 
            memname + "[acctname]") )
        {
            return false;
        }

        if( !StrToCstr(item.accttype, *(node->DataList[pos++]->GetKey()), 
            memname + "[accttype]") )
        {
            return false;
        }

        if( !StrToCstr(item.status, *(node->DataList[pos++]->GetKey()), 
            memname + "[status]") )
        {
            return false;
        }

        if( !StrToCstr(item.acct_idu, *(node->DataList[pos++]->GetKey()), 
            memname + "[acct_idu]") )
        {
            return false;
        }

        if( !StrToCstr(item.orgid, *(node->DataList[pos++]->GetKey()), 
            memname + "[orgid]") )
        {
            return false;
        }

        if( !StrToCstr(item.invoicetel, *(node->DataList[pos++]->GetKey()), 
            memname + "[invoicetel]") )
        {
            return false;
        }

        if( !StrToCstr(item.notifytype, *(node->DataList[pos++]->GetKey()), 
            memname + "[notifytype]") )
        {
            return false;
        }

        if( !StrToCstr(item.notifyvalue, *(node->DataList[pos++]->GetKey()), 
            memname + "[notifyvalue]") )
        {
            return false;
        }

        if( !StrToCstr(item.invprinttype, *(node->DataList[pos++]->GetKey()), 
            memname + "[invprinttype]") )
        {
            return false;
        }

        if( !StrToCstr(item.overdraft, *(node->DataList[pos++]->GetKey()), 
            memname + "[overdraft]") )
        {
            return false;
        }

        if( !StrToCstr(item.notes, *(node->DataList[pos++]->GetKey()), 
            memname + "[notes]") )
        {
            return false;
        }

        if( !StrToCstr(item.region, *(node->DataList[pos++]->GetKey()), 
            memname + "[region]") )
        {
            return false;
        }

        if( !StrToCstr(item.custid, *(node->DataList[pos++]->GetKey()), 
            memname + "[custid]") )
        {
            return false;
        }

        if( !StrToCstr(item.groupacctid, *(node->DataList[pos++]->GetKey()), 
            memname + "[groupacctid]") )
        {
            return false;
        }

        if( !StrToCstr(item.controlscheme, *(node->DataList[pos++]->GetKey()), 
            memname + "[controlscheme]") )
        {
            return false;
        }

        if( !StrToCstr(item.hbprintinv, *(node->DataList[pos++]->GetKey()), 
            memname + "[hbprintinv]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_CCCreateAcctInfo::getHDataNodeResult(vector<stltypelist>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 15;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        stltypelist item;
        string memname(destname);

        if( !StrToCstr(item.oid, *(node->DataList[pos++]->GetKey()), 
            memname + "[oid]") )
        {
            return false;
        }

        if( !StrToCstr(item.paytype, *(node->DataList[pos++]->GetKey()), 
            memname + "[paytype]") )
        {
            return false;
        }

        if( !StrToCstr(item.bankid, *(node->DataList[pos++]->GetKey()), 
            memname + "[bankid]") )
        {
            return false;
        }

        if( !StrToCstr(item.bankacct, *(node->DataList[pos++]->GetKey()), 
            memname + "[bankacct]") )
        {
            return false;
        }

        if( !StrToCstr(item.accttype, *(node->DataList[pos++]->GetKey()), 
            memname + "[accttype]") )
        {
            return false;
        }

        if( !StrToCstr(item.acctname, *(node->DataList[pos++]->GetKey()), 
            memname + "[acctname]") )
        {
            return false;
        }

        if( !StrToCstr(item.payorder, *(node->DataList[pos++]->GetKey()), 
            memname + "[payorder]") )
        {
            return false;
        }

        if( !StrToCstr(item.certid, *(node->DataList[pos++]->GetKey()), 
            memname + "[certid]") )
        {
            return false;
        }

        if( !StrToCstr(item.contractid, *(node->DataList[pos++]->GetKey()), 
            memname + "[contractid]") )
        {
            return false;
        }

        if( !StrToCstr(item.drawtype, *(node->DataList[pos++]->GetKey()), 
            memname + "[drawtype]") )
        {
            return false;
        }

        if( !StrToCstr(item.drawamt, *(node->DataList[pos++]->GetKey()), 
            memname + "[drawamt]") )
        {
            return false;
        }

        if( !StrToCstr(item.trigamt, *(node->DataList[pos++]->GetKey()), 
            memname + "[trigamt]") )
        {
            return false;
        }

        if( !StrToCstr(item.status, *(node->DataList[pos++]->GetKey()), 
            memname + "[status]") )
        {
            return false;
        }

        if( !StrToCstr(item.startdate, *(node->DataList[pos++]->GetKey()), 
            memname + "[startdate]") )
        {
            return false;
        }

        if( !StrToCstr(item.settletype_idu, *(node->DataList[pos++]->GetKey()), 
            memname + "[settletype_idu]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}

bool IB_COM_CCCreateAcctInfo::getHDataNodeResult(vector<bmaillist>& nodeinfo, 
                IN HDataNode* currNode, const string& destname)
{
    LogHere();

    //��������Ԫ��
    BASE_LOG_DEBUG("�ṹ������%d", (int)currNode->DataList.size());
    for (size_t i = 0; i < currNode->DataList.size(); i++)
    {
        //ÿ��Ԫ��������Ա����
        const int expectParamCnt = 12;
        HDataNode *node = currNode->DataList[i];

        if (! CheckDataListSize(node, expectParamCnt, destname.c_str()))
        {
            return false;
        }

        int pos = 0;
        bmaillist item;
        string memname(destname);

        if( !StrToCstr(item.oid, *(node->DataList[pos++]->GetKey()), 
            memname + "[oid]") )
        {
            return false;
        }

        if( !StrToCstr(item.region, *(node->DataList[pos++]->GetKey()), 
            memname + "[region]") )
        {
            return false;
        }

        if( !StrToCstr(item.billtype, *(node->DataList[pos++]->GetKey()), 
            memname + "[billtype]") )
        {
            return false;
        }

        if( !StrToCstr(item.langtype, *(node->DataList[pos++]->GetKey()), 
            memname + "[langtype]") )
        {
            return false;
        }

        if( !StrToCstr(item.mailtype, *(node->DataList[pos++]->GetKey()), 
            memname + "[mailtype]") )
        {
            return false;
        }

        if( !StrToCstr(item.mailaddr, *(node->DataList[pos++]->GetKey()), 
            memname + "[mailaddr]") )
        {
            return false;
        }

        if( !StrToCstr(item.postcode, *(node->DataList[pos++]->GetKey()), 
            memname + "[postcode]") )
        {
            return false;
        }

        if( !StrToCstr(item.linkman, *(node->DataList[pos++]->GetKey()), 
            memname + "[linkman]") )
        {
            return false;
        }

        if( !StrToCstr(item.linkphone, *(node->DataList[pos++]->GetKey()), 
            memname + "[linkphone]") )
        {
            return false;
        }

        if( !StrToCstr(item.status, *(node->DataList[pos++]->GetKey()), 
            memname + "[status]") )
        {
            return false;
        }

        if( !StrToCstr(item.ca_billmail_idu, *(node->DataList[pos++]->GetKey()), 
            memname + "[ca_billmail_idu]") )
        {
            return false;
        }

        if( !StrToCstr(item.statusdate, *(node->DataList[pos++]->GetKey()), 
            memname + "[statusdate]") )
        {
            return false;
        }


        nodeinfo.push_back(item);
    }

    return true;
}



bool IB_COM_CCCreateAcctInfo::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<acctlist>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //acctid
        strValue = nodeinfo[i].acctid;
        node->AddSimpleData(strValue);

        //prepaytype
        strValue = nodeinfo[i].prepaytype;
        node->AddSimpleData(strValue);

        //acctname
        strValue = nodeinfo[i].acctname;
        node->AddSimpleData(strValue);

        //accttype
        strValue = nodeinfo[i].accttype;
        node->AddSimpleData(strValue);

        //status
        strValue = nodeinfo[i].status;
        node->AddSimpleData(strValue);

        //acct_idu
        strValue = nodeinfo[i].acct_idu;
        node->AddSimpleData(strValue);

        //orgid
        strValue = nodeinfo[i].orgid;
        node->AddSimpleData(strValue);

        //invoicetel
        strValue = nodeinfo[i].invoicetel;
        node->AddSimpleData(strValue);

        //notifytype
        strValue = nodeinfo[i].notifytype;
        node->AddSimpleData(strValue);

        //notifyvalue
        strValue = nodeinfo[i].notifyvalue;
        node->AddSimpleData(strValue);

        //invprinttype
        strValue = nodeinfo[i].invprinttype;
        node->AddSimpleData(strValue);

        //overdraft
        strValue = nodeinfo[i].overdraft;
        node->AddSimpleData(strValue);

        //notes
        strValue = nodeinfo[i].notes;
        node->AddSimpleData(strValue);

        //region
        strValue = nodeinfo[i].region;
        node->AddSimpleData(strValue);

        //custid
        strValue = nodeinfo[i].custid;
        node->AddSimpleData(strValue);

        //groupacctid
        strValue = nodeinfo[i].groupacctid;
        node->AddSimpleData(strValue);

        //controlscheme
        strValue = nodeinfo[i].controlscheme;
        node->AddSimpleData(strValue);

        //hbprintinv
        strValue = nodeinfo[i].hbprintinv;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_CCCreateAcctInfo::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<stltypelist>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //oid
        strValue = nodeinfo[i].oid;
        node->AddSimpleData(strValue);

        //paytype
        strValue = nodeinfo[i].paytype;
        node->AddSimpleData(strValue);

        //bankid
        strValue = nodeinfo[i].bankid;
        node->AddSimpleData(strValue);

        //bankacct
        strValue = nodeinfo[i].bankacct;
        node->AddSimpleData(strValue);

        //accttype
        strValue = nodeinfo[i].accttype;
        node->AddSimpleData(strValue);

        //acctname
        strValue = nodeinfo[i].acctname;
        node->AddSimpleData(strValue);

        //payorder
        strValue = nodeinfo[i].payorder;
        node->AddSimpleData(strValue);

        //certid
        strValue = nodeinfo[i].certid;
        node->AddSimpleData(strValue);

        //contractid
        strValue = nodeinfo[i].contractid;
        node->AddSimpleData(strValue);

        //drawtype
        strValue = nodeinfo[i].drawtype;
        node->AddSimpleData(strValue);

        //drawamt
        strValue = nodeinfo[i].drawamt;
        node->AddSimpleData(strValue);

        //trigamt
        strValue = nodeinfo[i].trigamt;
        node->AddSimpleData(strValue);

        //status
        strValue = nodeinfo[i].status;
        node->AddSimpleData(strValue);

        //startdate
        strValue = nodeinfo[i].startdate;
        node->AddSimpleData(strValue);

        //settletype_idu
        strValue = nodeinfo[i].settletype_idu;
        node->AddSimpleData(strValue);


    }

    return true;
}

bool IB_COM_CCCreateAcctInfo::ParseArrayToNode(HDataNode *arraynode, 
                    const vector<bmaillist>& nodeinfo)
{
    LogHere();

    string strValue;
    bool bRet = true;

    for (size_t i = 0; i < nodeinfo.size(); i++)
    {
        HDataNode *node = arraynode->AddStuListChild();

        //oid
        strValue = nodeinfo[i].oid;
        node->AddSimpleData(strValue);

        //region
        strValue = nodeinfo[i].region;
        node->AddSimpleData(strValue);

        //billtype
        strValue = nodeinfo[i].billtype;
        node->AddSimpleData(strValue);

        //langtype
        strValue = nodeinfo[i].langtype;
        node->AddSimpleData(strValue);

        //mailtype
        strValue = nodeinfo[i].mailtype;
        node->AddSimpleData(strValue);

        //mailaddr
        strValue = nodeinfo[i].mailaddr;
        node->AddSimpleData(strValue);

        //postcode
        strValue = nodeinfo[i].postcode;
        node->AddSimpleData(strValue);

        //linkman
        strValue = nodeinfo[i].linkman;
        node->AddSimpleData(strValue);

        //linkphone
        strValue = nodeinfo[i].linkphone;
        node->AddSimpleData(strValue);

        //status
        strValue = nodeinfo[i].status;
        node->AddSimpleData(strValue);

        //ca_billmail_idu
        strValue = nodeinfo[i].ca_billmail_idu;
        node->AddSimpleData(strValue);

        //statusdate
        strValue = nodeinfo[i].statusdate;
        node->AddSimpleData(strValue);


    }

    return true;
}



