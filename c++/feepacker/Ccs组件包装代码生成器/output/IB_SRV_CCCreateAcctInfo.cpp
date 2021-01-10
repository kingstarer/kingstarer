/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   ������־��ѯ�ӿ�
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_SRV_CCCreateAcctInfo.h"

void CIB_SRV_CCCreateAcctInfo::setBusinessName()
{
    m_rpcname = "IB_SRV_CCCreateAcctInfo";
    m_businessname = "������־��ѯ�ӿ�";
}

char * CIB_SRV_CCCreateAcctInfo::ClassName(void)
{
    //�������û�б�����
    return "IB_SRV_CCCreateAcctInfo";
}   

bool CIB_SRV_CCCreateAcctInfo::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //�ɹ�ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    CCCreateAcctInfoInfo info;
    //������ת����������
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("������ν���ʧ��");
    }

    if (bRet)
    {
        CCCreateAcctInfoInfoOut outinfo;
        
        //��������߼�
        bRet = m_cicsOpr.IB_CCCreateAcctInfo(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_CCCreateAcctInfo�������ʧ��");
            m_iErrorCode = m_cicsOpr.m_iErrorCode;
            m_sErrorStr = m_cicsOpr.m_sErrorStr;
        }
        else
        {
            //���������ת����xml����
            bRet = ParseComMsg(rcvmsg, outinfo);
            if (!bRet)
            {
                BASE_LOG_ERROR("���������ת����xml����ʧ��");
            }
        }
    }

    return bRet;
}


bool CIB_SRV_CCCreateAcctInfo::ParseSrvMsg(HSendMsg *sendmsg, CCCreateAcctInfoInfo& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = 12;
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
    if (!copyToBuffer(data.ordid, pStr, "ordid"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.formnum, pStr, "formnum"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.contacttype, pStr, "contacttype"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.operid, pStr, "operid"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.recdate, pStr, "recdate"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.recdefid, pStr, "recdefid"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.idu, pStr, "idu"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.custid, pStr, "custid"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.region, pStr, "region"))
    {
        return false;
    }

    if( !m_cicsOpr.getHDataNodeResult(data.acctlistArray, rootnode->DataList[pos++], 
            "[acctlist]" ) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_sErrorStr;
        return false;
    }

    if( !m_cicsOpr.getHDataNodeResult(data.stltypelistArray, rootnode->DataList[pos++], 
            "[stltypelist]" ) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_sErrorStr;
        return false;
    }

    if( !m_cicsOpr.getHDataNodeResult(data.bmaillistArray, rootnode->DataList[pos++], 
            "[bmaillist]" ) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_sErrorStr;
        return false;
    }


    return true;
}

bool CIB_SRV_CCCreateAcctInfo::ParseComMsg(HRecvMsg *rcvmsg, CCCreateAcctInfoInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    BASE_LOG_DEBUG("%s", "�˻�ID");
    strValue = data.acctid;
    node->AddSimpleData(strValue);

    //��װ����seq�ڵ�
    node->AddUseqChild();

    BASE_LOG_DEBUG("%s", "������");
    strValue = data.ErrNo;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "���ش�����Ϣ");
    strValue = data.ErrInfo;
    node->AddSimpleData(strValue);


    return true;
}

//Ĭ�ϵ�setErrorXml�����xml����Ϊ[(������), (seq�ṹ��), (������), (������Ϣ)]
//�����Ҫ������ʽ�Ĵ���������Ҫ���ش˺�����������Բ�����
void CIB_SRV_CCCreateAcctInfo::setErrorXml(HRecvMsg * rcvmsg)
{
    //ʧ��ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    //�������
    rootnode->FreeChildNode();

    string nullstr;
    rootnode->AddSimpleData(nullstr);

    rootnode->AddUseqChild(); //��װ�ķ�����Ҫ����seq�ڵ�

    string tmpstr = IntToStr(m_iErrorCode); //xlc��֧�ֽ�����ֵ��������
    rootnode->AddSimpleData( tmpstr );

    rootnode->AddSimpleData( m_sErrorStr );


}


