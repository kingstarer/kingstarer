/*---------------------------------------------------------------
//Copyright (C), 2011, Sunrise Co., Ltd.
//Author        :   hch
//Date          :   2011-1-8
//Description   :   ���Ŵ����ʵ���ѯ
//
//Others        :   ��
//History       :   ������ʼ�汾
//---------------------------------------------------------------*/
#include "IM_SRV_IMQryWareDetail.h"

void CIM_SRV_IMQryWareDetail::setBusinessName()
{
    m_rpcname = "IM_SRV_IMQryWareDetail";
    m_businessname = "���Ŵ����ʵ���ѯ";
}

char * CIM_SRV_IMQryWareDetail::ClassName(void)
{
    //�������û�б�����
    return "IM_SRV_IMQryWareDetail";
}   

bool CIM_SRV_IMQryWareDetail::doBusiness(HSendMsg *sendmsg, HRecvMsg * &rcvmsg)
{
    LogHere();
    RecordFlow();

    //�ɹ�ʱ������ݵ�HRecvMsg * &rcvmsg
    HDataNode *rootnode = rcvmsg->GetRootNode();

    bool bRet = false;
    IMQryWareDetailInfo info;
    //������ת����������
    bRet = ParseSrvMsg(sendmsg, info);
    if (!bRet)
    {
        BASE_LOG_ERROR("������ν���ʧ��");
    }

    if (bRet)
    {
        IMQryWareDetailInfoOut outinfo;
        
        //��������߼�
        bRet = m_cicsOpr.IM_IMQryWareDetail(info, outinfo);
        
        if (!bRet)
        {
            BASE_LOG_ERROR("IM_COM_IMQryWareDetail�������ʧ��");
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


bool CIM_SRV_IMQryWareDetail::ParseSrvMsg(HSendMsg *sendmsg, IMQryWareDetailInfo& data)
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
    
    BASE_LOG_DEBUG("%s", "�ʺ�");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.Accoid, pStr, "����ṹ��[�ʺ�]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "��ʼʱ��");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.StartTime, pStr, "����ṹ��[��ʼʱ��]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "����ʱ��");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.EndTime, pStr, "����ṹ��[����ʱ��]"))
    {
        return false;
    }

    BASE_LOG_DEBUG("%s", "��ѯ����");
    curnode = rootnode->DataList[pos++];
    pStr = curnode->GetKey();
    if (!copyToBuffer(data.qryType, pStr, "����ṹ��[��ѯ����]"))
    {
        return false;
    }


    return true;
}

bool CIM_SRV_IMQryWareDetail::ParseComMsg(HRecvMsg *rcvmsg, IMQryWareDetailInfoOut& data)
{
    LogHere();

    HDataNode *node = rcvmsg->GetRootNode();
    string strValue;
    
    BASE_LOG_DEBUG("%s", "ucid����");
    if ( !m_cicsOpr.ParseArrayToNode(node->AddArrayChild(), data.UcidArray) )
    {
        m_iErrorCode = m_cicsOpr.m_iErrorCode;
        m_sErrorStr = m_cicsOpr.m_iErrorCode;
        return false;
    }

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
void CIM_SRV_IMQryWareDetail::setErrorXml(HRecvMsg * rcvmsg)
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


