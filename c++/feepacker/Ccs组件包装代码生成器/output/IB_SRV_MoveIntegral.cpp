/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   ����ת��
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IB_SRV_MoveIntegral.h"

void CIB_SRV_MoveIntegral::setBusinessName()
{
    m_rpcname = "IB_SRV_MoveIntegral";
    m_businessname = "����ת��";
}

char * CIB_SRV_MoveIntegral::ClassName(void)
{
    //�������û�б�����
    return "IB_SRV_MoveIntegral";
}   

bool CIB_SRV_MoveIntegral::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //�ɹ�ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    MoveIntegralInfo info;
    //������ת����������
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("������ν���ʧ��");
    }

    if (bRet)
    {
        MoveIntegralInfoOut outinfo;
        
        //��������߼�
        bRet = m_cicsOpr.IB_MoveIntegral(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IB_COM_MoveIntegral�������ʧ��");
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


bool CIB_SRV_MoveIntegral::ParseSrvMsg(HSendMsg *sendmsg, MoveIntegralInfo& data)
{
    LogHere();

    HDataNode *rootnode = sendmsg->GetRootNode();
    const int expectCnt = 13;
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
    if (!copyToBuffer(data.Businum, pStr, "ҵ����ˮ��"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Outcustid, pStr, "ת���ͻ�����"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.OutServnumber, pStr, "ת���������"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.OutProdtype, pStr, "ת����Ʒ����"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Region, pStr, "��������"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Incustid, pStr, "ת��ͻ�����"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.InServnumber, pStr, "ת��������"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.InProdtype, pStr, "ת���Ʒ����"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Oprcode, pStr, "����Ա"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Intchgrsn, pStr, "���ֱ��ԭ��"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Moveintegral, pStr, "ת�ƻ���"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Movecycle, pStr, "��������"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Unit, pStr, "Ӫҵ��"))
    {
        return false;
    }


    return true;
}

bool CIB_SRV_MoveIntegral::ParseComMsg(HRecvMsg *rcvmsg, MoveIntegralInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
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
void CIB_SRV_MoveIntegral::setErrorXml(HRecvMsg * rcvmsg)
{
    //ʧ��ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    //�������
    rootnode->FreeChildNode();

    string nullstr;
    rootnode->AddUseqChild(); //��װ�ķ�����Ҫ����seq�ڵ�

    string tmpstr = IntToStr(m_iErrorCode); //xlc��֧�ֽ�����ֵ��������
    rootnode->AddSimpleData( tmpstr );

    rootnode->AddSimpleData( m_sErrorStr );


}


