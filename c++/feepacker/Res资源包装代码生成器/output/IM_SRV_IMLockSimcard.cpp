/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   SIM������
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IM_SRV_IMLockSimcard.h"
#include "view/Server.h"

void CIM_SRV_IMLockSimcard::setBusinessName()
{
    m_rpcname = "IM_SRV_IMLockSimcard";
    m_businessname = "SIM������";
}

char * CIM_SRV_IMLockSimcard::ClassName(void)
{
    //�������û�б�����
    return "IM_SRV_IMLockSimcard";
}   

bool CIM_SRV_IMLockSimcard::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //�ɹ�ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    IMLockSimcardInfo info;
    //������ת����������
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("������ν���ʧ��");
    }

    if (bRet)
    {
        IMLockSimcardInfoOut outinfo;
        
        //��������߼�
        bRet = m_cicsOpr.IM_IMLockSimcard(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IM_COM_IMLockSimcard�������ʧ��");
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


bool CIM_SRV_IMLockSimcard::ParseSrvMsg(HSendMsg *sendmsg, IMLockSimcardInfo& data)
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
    if (!copyToBuffer(data.OrgID, pStr, "��λ����"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.OperID, pStr, "����Ա����"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.ICCID, pStr, "SIM����"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.sRecOID, pStr, "ҵ��������ˮ��"))
    {
        return false;
    }

    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.iPrice, pStr, "ʵ�����ۼ۸�"))
    {
        return false;
    }


    return true;
}

bool CIM_SRV_IMLockSimcard::ParseComMsg(HRecvMsg *rcvmsg, IMLockSimcardInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    //��װ����seq�ڵ�
    node->AddUseqChild();

    BASE_LOG_DEBUG("%s", "�������");
    strValue = data.RetCode;
    node->AddSimpleData(strValue);

    BASE_LOG_DEBUG("%s", "����ʧ��ԭ��");
    strValue = data.ErrMsg;
    node->AddSimpleData(strValue);


    return true;
}

//Ĭ�ϵ�setErrorXml�����xml����Ϊ[(������), (seq�ṹ��), (������), (������Ϣ)]
//�����Ҫ������ʽ�Ĵ���������Ҫ���ش˺�����������Բ�����
void CIM_SRV_IMLockSimcard::setErrorXml(HRecvMsg * rcvmsg)
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


