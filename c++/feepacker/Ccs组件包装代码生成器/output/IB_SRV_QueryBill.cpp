/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   ���ʵ��ӿ�
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_SRV_QueryBill.h"

void CIB_SRV_QueryBill::setBusinessName()
{
    m_rpcname = "IB_SRV_QueryBill";
    m_businessname = "���ʵ��ӿ�";
}

char * CIB_SRV_QueryBill::ClassName(void)
{
    //�������û�б�����
    return "IB_SRV_QueryBill";
}   

bool CIB_SRV_QueryBill::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //�ɹ�ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    QueryBillInfo info;
    //������ת����������
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("������ν���ʧ��");
    }

    if (bRet)
    {
        QueryBillInfoOut outinfo;
        
        //��������߼�
        bRet = m_cicsOpr.IB_QueryBill(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_QueryBill�������ʧ��");
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


bool CIB_SRV_QueryBill::ParseSrvMsg(HSendMsg *sendmsg, QueryBillInfo& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = 9;
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
    if (!copyToBuffer(data.acctid, pStr, "�ʻ�ID"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.subsid, pStr, "�û�ID"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.StartCycle, pStr, "��ʼ����"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.EndCycle, pStr, "��������"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Status, pStr, "�ʵ�״̬"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.IsBadDebt, pStr, "�Ƿ���ʾ���ʷ���"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.HowBadDebt, pStr, "�����Դ����ʷ���"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.BillType, pStr, "�����ʵ�����ʽ"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.BillFormatNum, pStr, "������ϸ�������ϼ���"))
    {
        return false;
    }


    return true;
}

bool CIB_SRV_QueryBill::ParseComMsg(HRecvMsg *rcvmsg, QueryBillInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    BASE_LOG_DEBUG("%s", "�ʵ�����");
    if ( !m_cicsOpr.ParseArrayToNode(node->AddArrayChild(), data.BillArray) )
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
void CIB_SRV_QueryBill::setErrorXml(HRecvMsg * rcvmsg)
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


