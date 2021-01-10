/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   ���Ŵ����ʵ���ѯ
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_SRV_QueryGroupFungiblePayBill.h"

void CIB_SRV_QueryGroupFungiblePayBill::setBusinessName()
{
    m_rpcname = "IB_SRV_QueryGroupFungiblePayBill";
    m_businessname = "���Ŵ����ʵ���ѯ";
}

char * CIB_SRV_QueryGroupFungiblePayBill::ClassName(void)
{
    //�������û�б�����
    return "IB_SRV_QueryGroupFungiblePayBill";
}   

bool CIB_SRV_QueryGroupFungiblePayBill::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //�ɹ�ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    QueryGroupFungiblePayBillInfo info;
    //������ת����������
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("������ν���ʧ��");
    }

    if (bRet)
    {
        QueryGroupFungiblePayBillInfoOut outinfo;
        
        //��������߼�
        bRet = m_cicsOpr.IB_QueryGroupFungiblePayBill(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_QueryGroupFungiblePayBill�������ʧ��");
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


bool CIB_SRV_QueryGroupFungiblePayBill::ParseSrvMsg(HSendMsg *sendmsg, QueryGroupFungiblePayBillInfo& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = 5;
    if (!checkParamIn(sendmsg, expectCnt))
    {
        BASE_LOG_ERROR("%s", m_sErrorStr.c_str());
        return false;
    }

    int pos = 0;
    rootnode = sendmsg->GetRootNode();
    string *pStr = NULL;

    HDataNode *curnode;
    
    BASE_LOG_DEBUG("%s", "��������");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Region, pStr, "����ṹ��[��������]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "��λ����");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.OrgID, pStr, "����ṹ��[��λ����]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "����Ա����");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.OperID, pStr, "����ṹ��[����Ա����]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "��Ʒ���");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.ResType, pStr, "����ṹ��[��Ʒ���]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "��Ʒ���");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.ResSerial, pStr, "����ṹ��[��Ʒ���]"))
    {
        return false;
    }


    return true;
}

bool CIB_SRV_QueryGroupFungiblePayBill::ParseComMsg(HRecvMsg *rcvmsg, QueryGroupFungiblePayBillInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    BASE_LOG_DEBUG("%s", "�����ʵ�����");
    if ( !m_cicsOpr.ParseArrayToNode(node->AddArrayChild(), data.BillAccountArray) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_iErrorCode;
        return false;
    }

    BASE_LOG_DEBUG("%s", "��ѯ��ˮ��");
    strValue = data.SourceNo;
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
void CIB_SRV_QueryGroupFungiblePayBill::setErrorXml(HRecvMsg * rcvmsg)
{
    //ʧ��ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    //�������
    rootnode->FreeChildNode();

    string nullstr;
    //��������Ҫ�ڴ����һ���յ�����
    rootnode->AddArrayChild();

    rootnode->AddSimpleData(nullstr);

    rootnode->AddUseqChild(); //��װ�ķ�����Ҫ����seq�ڵ�

    string tmpstr = IntToStr(m_iErrorCode); //xlc��֧�ֽ�����ֵ��������
    rootnode->AddSimpleData( tmpstr );

    rootnode->AddSimpleData( m_sErrorStr );


}


