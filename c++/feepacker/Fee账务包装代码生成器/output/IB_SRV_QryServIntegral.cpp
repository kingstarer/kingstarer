/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   ��ͻ�������Ϣ
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_SRV_QryServIntegral.h"

void CIB_SRV_QryServIntegral::setBusinessName()
{
    m_rpcname = "IB_SRV_QryServIntegral";
    m_businessname = "��ͻ�������Ϣ";
}

char * CIB_SRV_QryServIntegral::ClassName(void)
{
    //�������û�б�����
    return "IB_SRV_QryServIntegral";
}   

bool CIB_SRV_QryServIntegral::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //�ɹ�ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    QryServIntegralInfo info;
    //������ת����������
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("������ν���ʧ��");
    }

    if (bRet)
    {
        QryServIntegralInfoOut outinfo;
        
        //��������߼�
        bRet = m_cicsOpr.IB_QryServIntegral(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_QryServIntegral�������ʧ��");
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


bool CIB_SRV_QryServIntegral::ParseSrvMsg(HSendMsg *sendmsg, QryServIntegralInfo& data)
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
    if (!copyToBuffer(data.Region, pStr, "��������"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Servnumber, pStr, "�������"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.CustID, pStr, "�ͻ����"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Prodtype, pStr, "��Ʒ����"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Integralcyc, pStr, "��������"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Brandscoretype, pStr, "ȫƷ�ƻ�������"))
    {
        return false;
    }


    return true;
}

bool CIB_SRV_QryServIntegral::ParseComMsg(HRecvMsg *rcvmsg, QryServIntegralInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    BASE_LOG_DEBUG("%s", "������Ϣ����");
    if ( !m_cicsOpr.ParseArrayToNode(node->AddArrayChild(), data.ACustIntegralArray) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_iErrorCode;
        return false;
    }

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
void CIB_SRV_QryServIntegral::setErrorXml(HRecvMsg * rcvmsg)
{
    //ʧ��ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    //�������
    rootnode->FreeChildNode();

    string nullstr;
    //��������Ҫ�ڴ����һ���յ�����
    rootnode->AddArrayChild();

    rootnode->AddUseqChild(); //��װ�ķ�����Ҫ����seq�ڵ�

    string tmpstr = IntToStr(m_iErrorCode); //xlc��֧�ֽ�����ֵ��������
    rootnode->AddSimpleData( tmpstr );

    rootnode->AddSimpleData( m_sErrorStr );


}


